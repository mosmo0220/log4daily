#include "diaryComponent.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

void DiaryComponent::getTodayDate(std::string &today_date) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    today_date = std::to_string(1 + ltm->tm_mon) + "/" + std::to_string(ltm->tm_mday) + "/" + std::to_string(1900 + ltm->tm_year);
}

Component DiaryComponent::diaryComponent() {
    std::string today_date;
    getTodayDate(today_date);

    auto container = Container::Vertical({
        ftxui::Renderer([] { return filler(); }),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Container::Vertical({
            ftxui::Renderer([] { return filler(); }),
            Renderer([today_date] {
                return text("Today's Date: " + today_date);
            }),
            Renderer([] {
                return text("Today's Journal: Not set yet (in development)");
            }),
        }) | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);

    return container;
}