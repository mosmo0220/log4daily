#include "diaryComponent.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component DiaryComponent::diaryComponent() {
    auto radiobox = Radiobox(&diary, &selected_diary);
    auto renderer = Renderer([=] { return text("Diary"); });
    return Container::Vertical({
        ftxui::Renderer([] { return filler(); }),
        radiobox,
        renderer,
        ftxui::Renderer([] { return filler(); }),
    }) | center;
}