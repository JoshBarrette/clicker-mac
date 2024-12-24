#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <_stdlib.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_sdl3.h>

#include <iostream>
#include <thread>

#include "apple_hardware.hpp"
#include "examples.hpp"
#include "global.hpp"
#include "init.hpp"
#include "queue.hpp"
#include "structs.hpp"
#include "ui_windows.hpp"
#include "window_input.hpp"

int main() {
    init();

    std::thread click_thread = start_click_thread();

    while (!*(context.done)) {
        if (++context.tick_counter >= 100) {
            context.tick_counter %= 100;
            context.cps_queue.push(context.click_count);
            if (context.click_count == 0) {
                context.ui_cps_counter = 0;
            } else {
                context.ui_cps_counter = context.click_count;
            }
            context.click_count = 0;
        }
        handle_input();

        // Wait if minimized and start the Dear ImGui frame
        {
            if (SDL_GetWindowFlags(context.window) & SDL_WINDOW_MINIMIZED) {
                SDL_Delay(10);
                continue;
            }
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            ImGui::PushFont(fonts.default_font);
        }

        // Draw
        {
            main_window();
        }

        {
            // ImGui::Begin("Debug");

            // ImGui::Text("Simple Line Plot:");

            // ImGui::End();
        }

        // Rendering
        {
            ImGui::PopFont();
            ImGui::Render();
            glViewport(0, 0, (int)context.io->DisplaySize.x, (int)context.io->DisplaySize.y);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(context.window);
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    *(context.done) = true;
    click_thread.join();

    SDL_GL_DestroyContext(context.gl_context);
    SDL_DestroyWindow(context.window);
    SDL_Quit();
}