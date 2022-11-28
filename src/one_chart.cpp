#include "one_chart.h"

namespace three_dudes {
OneChart::OneChart(const std::string& window_title, unsigned short width,
                   unsigned short height)
    : glfw_context{window_title, width, height} {}
void OneChart::loop() {
    glfw_context.loop();
}
bool OneChart::isActive() const {
    return glfw_context.isActive();
}
} // namespace three_dudes
