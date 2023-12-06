#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

ModuleEditor::ModuleEditor()
{
    imGuiContext = nullptr;
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

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->getContext());
    ImGui_ImplOpenGL3_Init("#version 440");

    ImGui::StyleColorsDark();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
    ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    //ImGui::ShowDemoWindow();
    
    ShowUpperMenu();
    ShowWindowConsole();
    ShowWindowPerformance();

    // Render frame before swapping buffers (OpenGL PostUpdate)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
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
    logs.push_back(log);
}


void ModuleEditor::ShowUpperMenu() const
{
    static bool show_menu_about = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Github")) {
                //Navigate("https://github.com/Bermellet/3D-Engine-Glew");
            }

            if (ImGui::MenuItem("About", NULL, &show_menu_about)) {
                bool openAbout = false;
                
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Print
    if (show_menu_about) {
        if (ImGui::Begin("About", &show_menu_about, ImGuiWindowFlags_AlwaysAutoResize)) {
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
}

void ModuleEditor::ShowWindowConsole() const {
    ImGui::Begin("Console");

    for (const char* log : logs) {
        ImGui::TextUnformatted(log);
    }

    ImGui::End();
}

void ModuleEditor::ShowWindowPerformance() const {
    ImGui::Begin("Performance");


    ImGui::End();
}
