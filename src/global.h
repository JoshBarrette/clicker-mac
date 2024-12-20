#pragma once

#include <SDL3/SDL_video.h>
#include <imgui.h>

#include <atomic>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

struct Click {
    bool should_click = false;
};

inline Click* click = new Click;

struct AppContext {
    SDL_Window* window = NULL;
    ImGuiIO* io = NULL;
    SDL_GLContext gl_context = NULL;

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
};

inline AppContext* context = new AppContext;