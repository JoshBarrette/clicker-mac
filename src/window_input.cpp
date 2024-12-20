#include "window_input.h"

#include <SDL3/SDL_events.h>
#include <imgui_impl_sdl3.h>

#include "global.h"
#include "init.h"

void handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type) {
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    context.done = true;
                }
            case SDL_EVENT_QUIT:
                context.done = true;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(context.window)) {
                    context.done = true;
                }
        }
    }
}