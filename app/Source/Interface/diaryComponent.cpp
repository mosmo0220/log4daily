#include "../../Headers/Interface/diaryComponent.h"

#include "../../Headers/Storage/localStorage.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component DiaryComponent::diaryComponent(FileData *data) {
    std::vector<DiaryData> diary_entries = data->diaryData;

    std::vector<Date> diary_dates;
    for (DiaryData entry : diary_entries) {
        diary_dates.push_back(entry.date);
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    Date today_date = Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, 0, 0);

    std::vector<Date> diary_dates_combined;
    if (std::find(diary_dates.begin(), diary_dates.end(), today_date) == diary_dates.end()) {
        diary_dates_combined.push_back(today_date);
    }
    diary_dates_combined.insert(diary_dates_combined.end(), diary_dates.begin(), diary_dates.end());

    std::sort(diary_dates_combined.begin(), diary_dates_combined.end(), [](Date a, Date b) {
        if (a.year != b.year) {
            return a.year > b.year;
        }
        if (a.month != b.month) {
            return a.month > b.month;
        }
        return a.day > b.day;
    });

    auto newEntryNameInput = Input(&newEntryName, "New Entry Name: ");
    auto newEntryContentInput = Input(&newEntryContent, "New Entry Content: ");

    auto date_to_string = [](Date date) {
        std::string day_str = (date.day < 10 ? "0" : "") + std::to_string(date.day);
        std::string month_str = (date.month < 10 ? "0" : "") + std::to_string(date.month);
        std::string date_string = day_str + "/" + month_str + "/" + std::to_string(date.year);
        return date_string;
    };

    auto selected_index = std::make_shared<int>(0);  // Use a shared pointer for reactivity

    auto up_button = Button(">>", [selected_index] {
        if (*selected_index > 0) {
            (*selected_index)--;
        }
    });

    auto down_button = Button("<<", [selected_index, diary_dates_combined] {
        if (*selected_index < diary_dates_combined.size() - 1) {
            (*selected_index)++;
        }
    });

    auto selected_date_view = ftxui::Container::Horizontal({
        down_button,
        Renderer([date_to_string, diary_dates_combined, selected_index] { return text("  " + date_to_string(diary_dates_combined[*selected_index]) + "  "); }) | ftxui::center,
        up_button,
    }) | ftxui::center;

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