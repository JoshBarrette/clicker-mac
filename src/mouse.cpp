#include "mouse.hpp"

#include <ApplicationServices/ApplicationServices.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "global.hpp"

void left_click() {
    using namespace std;

    // if (!*(context.click_checked)) return;

    cout << "Clicking..." << endl;

    CGEventRef ourEvent = CGEventCreate(NULL);
    CGPoint point = CGEventGetLocation(ourEvent);
    CFRelease(ourEvent);

    CGEventRef click_down = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseDown,
        point,
        kCGMouseButtonLeft);
    CGEventRef click_up = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseUp,
        point,
        kCGMouseButtonLeft);

    CGEventPost(kCGHIDEventTap, click_down);
    std::this_thread::sleep_for(std::chrono::milliseconds(context.click_delay_ms));
    CGEventPost(kCGHIDEventTap, click_up);

    CFRelease(click_down);
    CFRelease(click_up);
}

CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    using namespace std;

    int64_t sourceState = CGEventGetIntegerValueField(event, kCGEventSourceStateID);
    if (sourceState == 0) return event;

    switch (type) {
        case kCGEventLeftMouseUp:
            cout << "Mouse up: " << sourceState << endl;
            *(context.should_click) = false;
            break;
        case kCGEventLeftMouseDown:
            cout << "Mouse down: " << sourceState << endl;
            *(context.should_click) = true;
            break;
        default:
            break;
    }

    return event;
}

void start_click_thread() {
    while (!*(context.done)) {
        if (*(context.should_click) && *(context.click_checked)) {
            left_click();
        }
    }
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