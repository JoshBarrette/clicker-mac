#include <ApplicationServices/ApplicationServices.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <_stdlib.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_sdl3.h>

#include <iostream>

#include "examples.h"
#include "global.h"
#include "init.h"
#include "mouse.h"
#include "window_input.h"

CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    using namespace std;

    int64_t sourceState = CGEventGetIntegerValueField(event, kCGEventSourceStateID);
    if (sourceState == 0) return event;

    switch (type) {
        case kCGEventLeftMouseUp:
            cout << "Mouse up: " << sourceState << endl;
            // click.should_click = false;
            break;
        case kCGEventLeftMouseDown:
            cout << "Mouse down: " << sourceState << endl;
            // click.should_click = !click.should_click;
            break;
        default:
            break;
    }

    return event;
}

int main() {
    init(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Create an event tap
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventLeftMouseDown) | CGEventMaskBit(kCGEventLeftMouseUp),
        eventCallback,
        nullptr);

    if (!eventTap) {
        std::cerr << "Failed to create event tap." << std::endl;
        return 1;
    }

    // Create a run loop source for the event tap
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    // Run the run loop
    CFRunLoopMode mode = kCFRunLoopDefaultMode;

    // Main loop
    bool done = false;
    while (!done) {
        handle_input(done);

        // Wait if minimized and start the Dear ImGui frame
        if (SDL_GetWindowFlags(context.window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (click.should_click) {
            left_click();
        }

        example_window();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)context.io->DisplaySize.x, (int)context.io->DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(context.window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(context.gl_context);
    SDL_DestroyWindow(context.window);
    SDL_Quit();
}