#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component CreateComponent(const std::vector<std::string>& entries, int& selected, const std::string& content) {
    auto radiobox = Radiobox(&entries, &selected);
    auto renderer = Renderer([=] { return text(content); });
    return Container::Vertical({
        ftxui::Renderer([] { return filler(); }),
        radiobox,
        renderer,
        ftxui::Renderer([] { return filler(); }),
    }) | center;
}