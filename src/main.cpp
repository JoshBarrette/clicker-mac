#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <_stdlib.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_sdl3.h>

#include <iostream>
#include <thread>

#include "examples.h"
#include "global.h"
#include "init.h"
#include "mouse.h"
#include "structs.h"
#include "window_input.h"

int main() {
    init(WINDOW_WIDTH, WINDOW_HEIGHT);

    start_click_reading();
    std::thread click_thread(start_click_thread);

    while (!context.done) {
        handle_input();

        // Wait if minimized and start the Dear ImGui frame
        if (SDL_GetWindowFlags(context.window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

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

    click_thread.join();

    SDL_GL_DestroyContext(context.gl_context);
    SDL_DestroyWindow(context.window);
    SDL_Quit();
}