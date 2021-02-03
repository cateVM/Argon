/*
* Compile as .Exe (Executable)
* x86 release
*/
#include "imgui_d\imgui.h"
#include "imgui_d\imgui_internal.h"
#include "imgui_d\Backends\imgui_impl_glfw.h"
#include "imgui_d\Backends\imgui_impl_opengl2.h"
#include "TextEditor.h"
#include "Injector.hpp"
#include <iostream> // astroz <3
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include <tchar.h>
#include <Windows.h> // for namepipes
ImFont* EditorFont;
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
LPCTSTR lpszPipename = _T("\\\\.\\pipe\\Argon");
bool Attached = false;

HWND FoundProgram() {
    HWND HwNDFound = nullptr;
    HWND MintestMenuWindow = FindWindowA(NULL, "Minetest 5.3.0 [Main Menu]");
    HWND MinetestGameWindow = FindWindowA(NULL, "Minetest 5.3.0 [OpenGL 4.6.0]");
    if (MintestMenuWindow) {
        HwNDFound = MintestMenuWindow;
    }
    if (MinetestGameWindow) {
        HwNDFound = MinetestGameWindow;
    }
    return HwNDFound;
}
float ImGuiWidth = 430.f;
float ImGuiHight = 220.f;
int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
   glfwWindowHint(GLFW_VISIBLE, 0x0);
    GLFWwindow* window = glfwCreateWindow(1, 1, "Argon_INVIS", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync BTW you can disable this if you'd like by passing in 0

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

   
    ImGuiStyle& style1 = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style1.WindowRounding = 0.0f;
        style1.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGuiStyle* style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.96f, 0.76f, 0.76f, 0.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.76f, 0.76f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.75f, 0.79f, 0.50f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.96f, 0.76f, 0.76f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.96f, 0.76f, 0.76f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.05f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.75f, 0.79f, 0.50f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.96f, 0.76f, 0.76f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused] = ImVec4(1.00f, 0.75f, 0.79f, 0.50f);
    style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.00f, 0.75f, 0.79f, 0.50f);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.96f, 0.76f, 0.76f, 1.00f);
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;

    EditorFont = io.Fonts->AddFontFromFileTTF("firacode.ttf", 13, &config);
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    TextEditor debug_s;
    auto langg = TextEditor::LanguageDefinition::Lua();
    debug_s.SetLanguageDefinition(langg);
    debug_s.SetShowWhitespaces(false);
    
    // render loop

    while (!glfwWindowShouldClose(window))
    {
        
        glfwPollEvents();

        
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        {
            
            ImGui::Begin("Argon",NULL,ImGuiWindowFlags_NoScrollbar);
            ImGui::PushFont(EditorFont);
 
            ImVec2 Size = ImGui::GetWindowSize();
            if (Size.x < ImGuiWidth) {
                ImGui::SetWindowSize(ImVec2(ImGuiWidth, Size.y));
            }
            if (Size.y < ImGuiHight) {
                ImGui::SetWindowSize(ImVec2(Size.x, ImGuiHight));
            }
            if (!FoundProgram) { Attached = false; }
            if (ImGui::BeginPopupModal("NPCStream not found, or not injected!")) {
                ImGui::Text("NPCStream not detected (did you inject the DLL?)");
                if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            if (ImGui::BeginPopupModal("Already Injected")) {
                ImGui::Text("You're already injected!");
                if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            if (ImGui::BeginPopupModal("Mintest not found!")) {
                ImGui::Text("Minetest was not found! Please open minetest");
                if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            debug_s.Render("Executor",ImVec2(Size.x - 16, Size.y - 54));
            if (ImGui::Button("Execute")) {

                HANDLE hPipe;
                DWORD dwWritten;


                hPipe = CreateFile(TEXT("\\\\.\\pipe\\Argon"),
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    NULL);
                if (hPipe != INVALID_HANDLE_VALUE)
                {
                    printf("Executed\n");
                    WriteFile(hPipe,
                        debug_s.GetText().c_str(),
                        debug_s.GetText().size(),
                        &dwWritten,
                        NULL);

                    CloseHandle(hPipe);
                }
                else {
                    /*
                    * The pipe is busy
                    * or 
                    * doesnt exist did you inject the dll?
                    */
                    //printf("Something went wrong...\n");
                    ImGui::OpenPopup("NPCStream not found, or not injected!");
                }
            }
            ImGui::SameLine();
            /*
              Attach button
              Attaching
            */
            if (ImGui::Button("Attach")) {
                if(Attached) { 
                    ImGui::OpenPopup("Already injected");
                }
                else {
                    if (!FoundProgram()) {
                        ImGui::OpenPopup("Mintest not found!");
                    }
                   else {
                        TCHAR buffer[MAX_PATH] = { 0 };
                        std::string Path = Injector::GetArgonDllPath();
                        printf(Path.c_str());
                        std::string Responce = Injector::Inject(FoundProgram(), Path);
                        if (Responce == "Injected") {
                            printf("Sucessfully Injected\n");
                            MessageBox(NULL, _T("Sucessfully Injected!\n"), _T("Info"), MB_OK | MB_ICONINFORMATION);
                            Attached = true;
                        }
                        else {
                            std::cout << Responce << std::endl;
                            
                        }
                    }
                }
            }
            ImGui::PopFont();
            ImGui::End();
        }

       

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

 
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
