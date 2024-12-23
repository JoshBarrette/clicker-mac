#pragma once

#include <SDL3/SDL_video.h>
#include <imgui.h>

#include <thread>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

struct Click {
    bool should_click = false;
};

inline Click click = {false};

struct AppContext {
    // Clicker options
    int click_delay_ms = 20;
    bool* click_checked = new bool(false);

    // UI
    bool* done = new bool(false);
    bool* should_click = new bool(false);

    SDL_Window* window = NULL;
    ImGuiIO* io = NULL;
    SDL_GLContext gl_context = NULL;

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
};

inline AppContext context = {};