#include "gui_functions.h"
#include <imgui.h>
#include <implot.h>
#include <implot_demo.cpp>

namespace three_dudes {
void GUIFunctions::setupDockspace() {
    // docking options
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    // TODO: Currently no way to turn padding or fullscreen on or off, do I want
    // that?
    //  fullscreen and docking are hard coded...
    static bool fullscreen = true;
    static bool padding = false;
    if (fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus
                        | ImGuiWindowFlags_NoNavFocus;
    } else {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }
    if (!padding) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    }

    // main dockspace
    ImGui::Begin("Main Dockspace Window", nullptr /*for now TODO: enable var*/,
                 window_flags);
    if (!padding) {
        ImGui::PopStyleVar();
    }
    if (fullscreen) {
        ImGui::PopStyleVar(2);
    }

    // submit dockspace...
    ImGuiID dockspace_id = ImGui::GetID("Dockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}
void GUIFunctions::mainGUI() {
    if (ImGui::BeginMainMenuBar()) {
        // diagnostics
        static bool show_imgui_metrics = false;
        static bool show_implot_metrics = false;
        static bool show_imgui_style_editor = false;
        static bool show_implot_style_editor = false;
        static bool show_implot_benchmark = false;
        // demos
        static bool show_demo_imgui = false;
        static bool show_demo_implot = false;
        // chart
        static bool show_chart_window = false;

        // diagnostics
        if (show_imgui_metrics) {
            ImGui::ShowMetricsWindow(&show_imgui_metrics);
        }
        if (show_implot_metrics) {
            ImPlot::ShowMetricsWindow(&show_implot_metrics);
        }
        if (show_imgui_style_editor) {
            ImGui::Begin("Style Editor (ImGui)", &show_imgui_style_editor);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
        if (show_implot_style_editor) {
            ImGui::SetNextWindowSize(ImVec2(415, 762), ImGuiCond_Appearing);
            ImGui::Begin("Style Editor (ImPlot)", &show_implot_style_editor);
            ImPlot::ShowStyleEditor();
            ImGui::End();
        }
        if (show_implot_benchmark) {
            ImGui::SetNextWindowSize(ImVec2(530, 740), ImGuiCond_Appearing);
            ImGui::Begin("ImPlot Benchmark Tool", &show_implot_benchmark);
            // ImPlot::ShowBenchmarkTool();
            ImGui::End();
            return;
        }
        // demos
        if (show_demo_imgui) {
            ImGui::ShowDemoWindow(&show_demo_imgui);
        }
        if (show_demo_implot) {
            ImPlot::ShowDemoWindow(&show_demo_implot);
        }
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
            }
            if (ImGui::BeginMenu("Open Recent")) {
                ImGui::MenuItem("fish_hat.c");
                ImGui::MenuItem("fish_hat.inl");
                ImGui::MenuItem("fish_hat.h");
                if (ImGui::BeginMenu("More..")) {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    if (ImGui::BeginMenu("Recurse..")) {
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
            }
            if (ImGui::MenuItem("Save As..")) {
            }

            ImGui::Separator();
            if (ImGui::BeginMenu("Options")) {
                static bool enabled = true;
                ImGui::MenuItem("Enabled", "", &enabled);
                ImGui::BeginChild("child", ImVec2(0, 60), true);
                for (int i = 0; i < 10; i++)
                    ImGui::Text("Scrolling Text %d", i);
                ImGui::EndChild();
                static float f = 0.5f;
                static int n = 0;
                ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                ImGui::InputFloat("Input", &f, 0.1f);
                ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Colors")) {
                float sz = ImGui::GetTextLineHeight();
                for (int i = 0; i < ImGuiCol_COUNT; i++) {
                    const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                    ImVec2 p = ImGui::GetCursorScreenPos();
                    ImGui::GetWindowDrawList()->AddRectFilled(
                        p, ImVec2(p.x + sz, p.y + sz),
                        ImGui::GetColorU32((ImGuiCol)i));
                    ImGui::Dummy(ImVec2(sz, sz));
                    ImGui::SameLine();
                    ImGui::MenuItem(name);
                }
                ImGui::EndMenu();
            }

            // Here we demonstrate appending again to the "Options" menu (which
            // we already created above) Of course in this demo it is a little
            // bit silly that this function calls BeginMenu("Options") twice. In
            // a real code-base using it would make senses to use this feature
            // from very different code locations.
            if (ImGui::BeginMenu("Options")) // <-- Append!
            {
                static bool b = true;
                ImGui::Checkbox("SomeOption", &b);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Disabled", false)) // Disabled
            {
                IM_ASSERT(0);
            }
            if (ImGui::MenuItem("Checked", nullptr, true)) {
            }
            if (ImGui::MenuItem("Quit", "Alt+F4")) {
                // glfwSetWindowShouldClose(window, GLFW_FALSE);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {
            }
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
            } // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {
            }
            if (ImGui::MenuItem("Copy", "CTRL+C")) {
            }
            if (ImGui::MenuItem("Paste", "CTRL+V")) {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::BeginMenu("ImGui")) {
                if (ImGui::MenuItem("Demo", nullptr, &show_demo_imgui, true)) {
                }
                if (ImGui::MenuItem("Metrics", nullptr, &show_imgui_metrics,
                                    true)) {
                }
                if (ImGui::MenuItem("Style Editor", nullptr,
                                    &show_imgui_style_editor, true)) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("ImPlot")) {
                if (ImGui::MenuItem("Demo", nullptr, &show_demo_implot, true)) {
                }
                if (ImGui::MenuItem("Metrics", nullptr, &show_implot_metrics,
                                    true)) {
                }
                if (ImGui::MenuItem("Style Editor", nullptr,
                                    &show_implot_style_editor, true)) {
                }
                if (ImGui::MenuItem("Benchmark", nullptr,
                                    &show_implot_benchmark, true)) {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::End(); // end dockspace
}
} // namespace three_dudes
