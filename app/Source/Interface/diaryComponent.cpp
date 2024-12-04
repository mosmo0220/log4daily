#include "../../Headers/Interface/diaryComponent.h"

#include "../../Headers/Storage/localStorage.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component DiaryComponent::diaryComponent(FileData *data) {
    std::vector<Date> open_dates = data->calendarData;
    std::vector<DiaryData> diary_entries = data->diaryData;

    std::vector<Date> diary_dates;
    for (DiaryData entry : diary_entries) {
        diary_dates.push_back(entry.date);
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    Date selected_date = Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, 0, 0);

    auto newEntryNameInput = Input(&newEntryName, "New Entry Name: ");
    auto newEntryContentInput = Input(&newEntryContent, "New Entry Content: ");

    auto date_to_string = [](Date date) {
        std::string date_string = std::to_string(date.day) + "/" + std::to_string(date.month) + "/" + std::to_string(date.year);
        return date_string;
    };

    auto selected_date_view = Renderer([date_to_string, selected_date] {
        return text("Selected Date: " + date_to_string(selected_date));
    });

    auto container = Container::Vertical({
        ftxui::Renderer([] { return filler(); }),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Container::Vertical({
            ftxui::Renderer([] { return filler(); }),
            selected_date_view,
            Renderer([] {
                return text("Today's Journal: Not set yet (in development)");
            }),
        }) | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Container::Vertical({
            Renderer([] {
                return text("Add Today Entry:");
            }),
            newEntryNameInput,
            newEntryContentInput,
        }) | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);

    return container;
}