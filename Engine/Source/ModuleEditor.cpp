#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include <numeric>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

ModuleEditor::ModuleEditor()
{
    imGuiContext = nullptr;
    logs.reserve(VECTOR_SIZE);
    elapsedTime.reserve(VECTOR_SIZE);
    fpsLog.reserve(VECTOR_SIZE);
    timer = 0;
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
    imGuiContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->getContext());
    ImGui_ImplOpenGL3_Init("#version 440");

    ImGui::StyleColorsDark();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
    timer = SDL_GetPerformanceCounter();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
    ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    //ImGui::ShowDemoWindow();
    
    ShowUpperMenu();
    /*ShowWindowConsole();
    ShowWindowPerformance();*/

    // Render frame before swapping buffers (OpenGL PostUpdate)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    Uint64 endTime = SDL_GetPerformanceCounter();
    float elapsed = (endTime - timer) / (float)SDL_GetPerformanceFrequency();
    if (elapsedTime.size() >= VECTOR_SIZE) {
        elapsedTime.erase(elapsedTime.begin());
    }
    if (fpsLog.size() >= VECTOR_SIZE) {
        fpsLog.erase(fpsLog.begin());
    }
    elapsedTime.push_back(elapsed*1000);
    fpsLog.push_back(1.0f / elapsed);

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}

void ModuleEditor::AddLog(const char* log)
{
    if (logs.size() >= VECTOR_SIZE) {
        logs.erase(logs.begin());
    }
    logs.push_back(log);
}


void ModuleEditor::ShowUpperMenu() const
{
    static bool show_window_performance = false;
    static bool show_menu_about = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Windows")) // <-- Append!
            {
                if (ImGui::MenuItem("Performance", "Ctrl+P", &show_window_performance)) {}

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Github")) {
                //Navigate("https://github.com/Bermellet/3D-Engine-Glew");
            }

            if (ImGui::MenuItem("About", NULL, &show_menu_about)) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Print
    if (show_window_performance) {
        ShowWindowPerformance(show_window_performance);
    }
    if (show_menu_about) {
        ShowMenuAbout(show_menu_about);
    }
}

void ModuleEditor::ShowMenuAbout(bool& show_menu_about) const
{
    if (ImGui::Begin("About", &show_menu_about, windowFlags)) {
        ImGui::Text("RoadToEngine");
        ImGui::Separator();
        ImGui::Text("3D Engine using OpenGL");
        ImGui::Text("By Jordi Nieto (Bermellet)");
        ImGui::Separator();
        ImGui::Text("Libraries: OpenGL, SDL, Glew, MathGeoLib, ImGui, tinygltf"); // TODO
        ImGui::Separator();
        ImGui::Text("GNU GENERAL PUBLIC LICENSE");
    }
    ImGui::End();
}

void ModuleEditor::ShowWindowConsole() const {
    ImGui::Begin("Console");

    for (const char* log : logs) {
        ImGui::TextUnformatted(log);
    }

    ImGui::End();
}

void ModuleEditor::ShowWindowPerformance(bool& show_window_performance) const {
    ImGui::Begin("Performance", &show_window_performance, windowFlags);
    
    char title[25];
    sprintf_s(title, 25, "Average FPS: %.1f", std::accumulate(fpsLog.begin(), fpsLog.end(), 0.0) / max(fpsLog.size(), 1));
    ImGui::Text(title);
    sprintf_s(title, 25, "Average ms: %.1f", std::accumulate(elapsedTime.begin(), elapsedTime.end(), 0.0) / max(elapsedTime.size(), 1));
    ImGui::Text(title);
    sprintf_s(title, 25, "Framerate %.1f", fpsLog[fpsLog.size() - 1]);
    ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310,100));
    sprintf_s(title, 25, "Milliseconds %0.1f", elapsedTime[elapsedTime.size() - 1]);
    ImGui::PlotHistogram("##milliseconds", &elapsedTime[0], elapsedTime.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

    ImGui::End();
}
