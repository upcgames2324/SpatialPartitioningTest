#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleRenderExercise.h"
#include <numeric>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <SDL.h>
#include <SDL_cpuinfo.h>

ModuleEditor::ModuleEditor()
{
    imGuiContext = nullptr;
    logs.reserve(VECTOR_SIZE);
    elapsedTime.reserve(VECTOR_SIZE);
    fpsLog.reserve(VECTOR_SIZE);
    windowFlags = 0;
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

float ModuleEditor::GetLastRenderTime() const
{
    return elapsedTime.size() > 0 ? elapsedTime[elapsedTime.size() - 1] : 1;
}


void ModuleEditor::ShowUpperMenu() const
{
    static int modelLoaded = 0;
    static bool show_window_console = false;
    static bool show_window_performance = false;
    static bool show_window_system_info = false;
    static bool show_help_about = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("Load")) {
                ImGui::RadioButton("Baker house", &modelLoaded, 0);
                ImGui::RadioButton("Duck", &modelLoaded, 1);
                ImGui::RadioButton("Patricio", &modelLoaded, 2);

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Windows")) // <-- Append!
            {
                if (ImGui::MenuItem("Console", NULL, &show_window_console)) {}
                if (ImGui::MenuItem("Performance", "Ctrl+P", &show_window_performance)) {}
                if (ImGui::MenuItem("System info", "Ctrl+I", &show_window_system_info)) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Github")) {
                system("start https://github.com/Bermellet/RoadToEngine");
            }
            if (ImGui::MenuItem("About", NULL, &show_help_about)) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Print
    if (show_window_console) { ShowWindowConsole(show_window_console); }
    if (show_window_performance) { ShowWindowPerformance(show_window_performance); }
    if (show_window_system_info) { ShowWindowSystemInfo(show_window_system_info); }
    if (show_help_about) { ShowHelpAbout(show_help_about); }
    App->GetModuleRender()->LoadPredefinedModel(modelLoaded);
}

void ModuleEditor::ShowWindowConsole(bool& show_window_console) const {
    ImGui::Begin("Console", &show_window_console, windowFlags);

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

void ModuleEditor::ShowWindowSystemInfo(bool& show_window_system_info) const {
    ImGui::Begin("System info", &show_window_system_info, windowFlags);

    char title[100];
    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    sprintf_s(title, 100, "SDL version: %i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
    ImGui::Text(title);
    ImGui::Separator();
    sprintf_s(title, 100, "CPUs: %i (L1 Cache: %i bytes)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
    ImGui::Text(title);
    sprintf_s(title, 100, "System RAM: %.1fGb", SDL_GetSystemRAM() / 1024.f);
    ImGui::Text(title);

    ImGui::End();
}

void ModuleEditor::ShowHelpAbout(bool& show_help_about) const
{
    if (ImGui::Begin("About", &show_help_about, windowFlags)) {
        ImGui::Text("RoadToEngine");
        ImGui::Separator();
        ImGui::Text("3D Engine using OpenGL");
        ImGui::Text("By Jordi Nieto (Bermellet)");
        ImGui::Separator();
        ImGui::Text("Libraries: OpenGL, SDL, Glew, MathGeoLib, ImGui, tinygltf");
        ImGui::Separator();
        ImGui::Text("GNU GENERAL PUBLIC LICENSE");
    }
    ImGui::End();
}
