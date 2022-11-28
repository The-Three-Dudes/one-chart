#pragma once

#ifndef ONE_CHART_INCLUDE_ONE_CHART_H
#define ONE_CHART_INCLUDE_ONE_CHART_H
#include "gui_context.h"
#include <string>

namespace three_dudes {
class OneChart {
public:
    OneChart(const std::string& window_title, unsigned short width,
             unsigned short height);
    void loop();
    bool isActive() const;

private:
    // std::string window_title;
    three_dudes::GUIContext glfw_context;
};
} // namespace three_dudes

#endif // ONE_CHART_INCLUDE_ONE_CHART_H