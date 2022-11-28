#include "gui_context.h"
#include "gui_functions.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <iostream>
// #include <stdexcept>

namespace three_dudes {
GUIContext::GUIContext(const std::string& window_title, unsigned short width,
                       unsigned short height)
    : glsl_version{GUIContext::chooseGL()}
    , window{GUIContext::initialiseGLFW(window_title, width, height)}
    , io{GUIContext::initialiseIMGUI()} {
    ImPlot::CreateContext();
}
GUIContext::~GUIContext() {
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
void GUIContext::loop() {
    glfwPollEvents();
    buildGUI();
    render();
}
bool GUIContext::isActive() const {
    // return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    // !glfwWindowShouldClose(window);
    return !glfwWindowShouldClose(window);
}
void GUIContext::errorCallbackGLFW(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << ") " << description << std::endl;
}
const char* GUIContext::chooseGL() {
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif
    // const char* glsl_version = "#version 410";
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    return glsl_version;
}
GLFWwindow* GUIContext::initialiseGLFW(const std::string& window_title,
                                       unsigned short width,
                                       unsigned short height) {
    glfwSetErrorCallback(GUIContext::errorCallbackGLFW);

    // Initialise GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise GLFW");
    }

    // Set other GLFW Window Hints here
    //    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    //    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    //    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    //    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    //    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // uncomment the following line to create a floating window.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window with graphics context
    GLFWwindow* window =
        glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Enable vsync - TODO: Check
    glfwSwapInterval(1);

    return window;
}
ImGuiIO& GUIContext::initialiseIMGUI() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& getIo = ImGui::GetIO();
    (void)getIo;
    getIo.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
    // Gamepad Controls
    getIo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    getIo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable
    //  Multi-Viewport
    //   / Platform Windows
    //  io.ConfigViewportsNoAutoMerge = true;
    //  io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGUI style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (getIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // TODO: Look into fonts...
    return getIo;
}
void GUIContext::buildGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Initialise docking...
    GUIFunctions::setupDockspace();

    // Main user GUI
    GUIFunctions::mainGUI();
}
void GUIContext::render() {
    ImVec4 clear_color = ImVec4(0.29f, 0.29f, 0.38f, 0.4f);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call
    //  glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
}
} // namespace three_dudes
