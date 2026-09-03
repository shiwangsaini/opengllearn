#include "Gui.h"


Gui::Gui(GLFWwindow* window)
{
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::CreateFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void Gui::Render()
{
    ImGui::Render();
}
