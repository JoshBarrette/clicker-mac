#include "examples.h"

#include <imgui.h>
#include <imgui_impl_opengl3_loader.h>

#include "global.h"

void example_window() {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");  // Display some text (you can use a format strings too)

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                      // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&context->clear_color);  // Edit 3 floats representing a color

    if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / context->io->Framerate, context->io->Framerate);
    ImGui::End();

    glClearColor(context->clear_color.x * context->clear_color.w, context->clear_color.y * context->clear_color.w, context->clear_color.z * context->clear_color.w, context->clear_color.w);
}