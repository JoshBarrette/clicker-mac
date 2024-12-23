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

inline bool dummy1 = false;
inline bool dummy2 = false;
struct AppContext {
    SDL_Window* window = NULL;
    ImGuiIO* io = NULL;
    SDL_GLContext gl_context = NULL;

    bool& done = dummy1;
    bool& should_click = dummy2;
    int click_delay_ms = 20;

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
};

inline AppContext context = {};