#include "mouse.h"

#include <ApplicationServices/ApplicationServices.h>

#include <iostream>

#include "global.h"

void left_click() {
    using namespace std;

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
    CGEventPost(kCGHIDEventTap, click_up);

    CFRelease(click_down);
    CFRelease(click_up);
}