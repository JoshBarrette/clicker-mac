#include "apple_hardware.hpp"

#include <ApplicationServices/ApplicationServices.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "global.hpp"

void left_click() {
    using namespace std;

    CGEventRef ourEvent = CGEventCreate(nullptr);
    CGPoint point = CGEventGetLocation(ourEvent);
    CFRelease(ourEvent);

    CGEventRef click_down = CGEventCreateMouseEvent(
        nullptr, kCGEventLeftMouseDown,
        point,
        kCGMouseButtonLeft);
    CGEventRef click_up = CGEventCreateMouseEvent(
        nullptr, kCGEventLeftMouseUp,
        point,
        kCGMouseButtonLeft);

    CGEventPost(kCGHIDEventTap, click_down);
    std::this_thread::sleep_for(std::chrono::milliseconds(context.click_delay_ms));
    CGEventPost(kCGHIDEventTap, click_up);

    CFRelease(click_down);
    CFRelease(click_up);
}

CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    using namespace std;

    int64_t sourceState = CGEventGetIntegerValueField(event, kCGEventSourceStateID);
    if (sourceState == 0) return event;

    switch (type) {
        case kCGEventLeftMouseUp:
            // cout << "Mouse up: " << sourceState << endl;
            *(context.should_click) = false;
            break;
        case kCGEventLeftMouseDown:
            // cout << "Mouse down: " << sourceState << endl;
            *(context.should_click) = true;
            break;
        default:
            break;
    }

    return event;
}

void start_click_reading() {
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventLeftMouseDown) | CGEventMaskBit(kCGEventLeftMouseUp),
        eventCallback,
        nullptr);

    if (!eventTap) {
        std::cerr << "Failed to create event tap." << std::endl;
        exit(1);
    }

    // Create a run loop source for the event tap
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    // Run the run loop
    CFRunLoopMode mode = kCFRunLoopDefaultMode;
}

std::thread start_click_thread() {
    start_click_reading();
    return std::thread([]() {
        while (!*(context.done)) {
#ifdef DEBUG_CLICK
            if (*(context.should_click) && *(context.click_checked)) {
                left_click();
                context.click_count++;
            }
#else
            if (*(context.should_click) && *(context.click_checked) && minecraft_focused()) {
                left_click();
                context.click_count++;
            }
#endif
        }
    });
}

std::string get_focused_window_name() {
    // Create a system-wide accessibility object
    AXUIElementRef systemWideElement = AXUIElementCreateSystemWide();

    // Get the application that currently has focus
    AXUIElementRef focusedApp = nullptr;
    AXError appError = AXUIElementCopyAttributeValue(systemWideElement, kAXFocusedApplicationAttribute, (CFTypeRef *)&focusedApp);

    if (appError != kAXErrorSuccess || !focusedApp) {
        if (focusedApp) CFRelease(focusedApp);
        CFRelease(systemWideElement);
        return "Error: Unable to get focused application.";
    }

    // Get the focused window of the application
    AXUIElementRef focusedWindow = nullptr;
    AXError windowError = AXUIElementCopyAttributeValue(focusedApp, kAXFocusedWindowAttribute, (CFTypeRef *)&focusedWindow);

    CFRelease(focusedApp);

    if (windowError != kAXErrorSuccess || !focusedWindow) {
        if (focusedWindow) CFRelease(focusedWindow);
        CFRelease(systemWideElement);
        return "Error: Unable to get focused window.";
    }

    // Get the title of the focused window
    CFStringRef windowTitle = nullptr;
    AXError titleError = AXUIElementCopyAttributeValue(focusedWindow, kAXTitleAttribute, (CFTypeRef *)&windowTitle);

    CFRelease(focusedWindow);
    CFRelease(systemWideElement);

    if (titleError != kAXErrorSuccess || !windowTitle) {
        if (windowTitle) CFRelease(windowTitle);
        return "Error: Unable to get window title.";
    }

    char titleBuffer[256];
    if (CFStringGetCString(windowTitle, titleBuffer, sizeof(titleBuffer), kCFStringEncodingUTF8)) {
        CFRelease(windowTitle);
        return std::string(titleBuffer);
    } else {
        CFRelease(windowTitle);
        return "Error: Unable to convert window title to string.";
    }
}

bool minecraft_focused() {
    std::string window_name = get_focused_window_name();
    std::string mc = "Minecraft";

    if (window_name.length() < mc.length()) {
        return false;
    }

    for (int i = 0; i < mc.length(); i++) {
        if (mc[i] != window_name[i]) {
            return false;
        }
    }

    return true;
}
