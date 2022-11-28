#pragma once

#ifndef ONE_CHART_GUI_CONTEXT_H
#define ONE_CHART_GUI_CONTEXT_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <string>

#define GL_SILENCE_DEPRECATION

namespace three_dudes {
class GUIContext {
public:
    GUIContext(const std::string& window_title, unsigned short width,
               unsigned short height);
    ~GUIContext();
    void loop();
    bool isActive() const;

private:
    GLFWwindow* window;
    const char* glsl_version;
    ImGuiIO& io;
    static void errorCallbackGLFW(int error, const char* description);
    static const char* chooseGL();
    static GLFWwindow* initialiseGLFW(const std::string& window_title,
                                      unsigned short width,
                                      unsigned short height);
    ImGuiIO& initialiseIMGUI();
    void buildGUI();
    void render();
};
} // namespace three_dudes

#endif // ONE_CHART_GUI_CONTEXT_H