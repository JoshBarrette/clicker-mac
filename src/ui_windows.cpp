#include "ui_windows.hpp"

#include <imgui.h>

#include <string>

#include "global.hpp"

void main_window() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({WINDOW_WIDTH, WINDOW_HEIGHT});
    int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoTitleBar;
    ImGui::Begin("Main Window", 0, flags);

    // Center the title
    const char title[] = "Clicker";
    float win_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((win_width - text_width) * 0.5f);

    // Fonts and colors
    ImGui::PushFont(fonts.impact_title);
    ImGui::TextColored({255, 0, 0, 255}, title);
    ImGui::PopFont();
    ImGui::PushFont(fonts.default_font);

    ImGui::Checkbox("Click", context.click_checked);

    if (*(context.click_checked)) {
        ImGui::SameLine();
        ImGui::Text(": enabled");
    }

    ImGui::PopFont();
    ImGui::End();
}