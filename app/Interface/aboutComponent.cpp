#include "aboutComponent.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component AboutComponent::aboutComponent() {
    auto radiobox = Radiobox(&about, &selected_about);
    auto renderer = Renderer([=] { return text("About"); });
    return Container::Vertical({
        ftxui::Renderer([] { return filler(); }),
        radiobox,
        renderer,
        ftxui::Renderer([] { return filler(); }),
    }) | center;
}