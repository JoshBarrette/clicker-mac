#pragma once

#include <SDL3/SDL_video.h>
#include <imgui.h>

#include <thread>
#include <vector>

#include "queue.hpp"

#define DEBUG_CLICK

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

struct AppContext {
    // Clicker options
    int click_delay_ms = 20;
    bool* click_checked = new bool(false);

    // UI
    bool* done = new bool(false);
    bool* should_click = new bool(false);
    Queue cps_queue;
    int tick_counter = 0;
    int click_count = 0;
    int ui_cps_counter = 0;

    SDL_Window* window = nullptr;
    ImGuiIO* io = nullptr;
    SDL_GLContext gl_context = nullptr;

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
};
inline AppContext context = {};

struct Fonts {
    ImFont* impact_title = nullptr;
    ImFont* default_font = nullptr;
};
inline Fonts fonts = {};