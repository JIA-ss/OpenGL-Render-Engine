#include "DebugSystem.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "System.h"
#include "system/WindowSystem.h"

void DebugSystem::Init()
{

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    GLFWwindow* window = SystemManager::Instance()->GetSystem<::WindowSystem>()->getGLFWwindow();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void DebugSystem::DrawGUI()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DrawSceneHierarchy();
    DrawResourceHierarchy();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugSystem::DrawSceneHierarchy()
{
    bool show_another_window = true;
    if (show_another_window)
    {
        ImGui::Begin("Hierarchy", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("This is Scene Hierarchy");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}
void DebugSystem::DrawResourceHierarchy()
{
    bool show_another_window = true;
    if (show_another_window)
    {
        ImGui::Begin("Resources", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("This is Resources Hierarchy");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}