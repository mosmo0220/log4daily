#include "../../Headers/Interface/diaryComponent.h"

#include "../../Headers/Storage/localStorage.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

DiaryData DiaryComponent::addDiaryEntry(FileData *data) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Date today;
    today.year = tm.tm_year + 1900;
    today.month = tm.tm_mon + 1;
    today.day = tm.tm_mday;
    today.hour = 0;
    today.minute = 0;

    DiaryData newDiaryEntry;
    newDiaryEntry.id = data->diaryData.size() + 1;
    newDiaryEntry.date = today;
    newDiaryEntry.diaryEntryName = newEntryName;
    newDiaryEntry.diaryEntry = newEntryContent;

    data->diaryData.push_back(newDiaryEntry);
    return newDiaryEntry;
}

Component DiaryComponent::diaryComponent(FileData *data) {
    diary_entries = data->diaryData;

    std::vector<Date> diary_dates;
    for (DiaryData entry : diary_entries) {
        diary_dates.push_back(entry.date);
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    Date today_date = Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, 0, 0);

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

    auto selected_index = std::make_shared<int>(0);
    auto is_valid_diary = std::make_shared<int>(0);

    auto newEntryNameInput = Input(&newEntryName, "New Entry Name: ");
    auto newEntryContentInput = Input(&newEntryContent, "New Entry Content: ");

    auto date_to_string = [](Date date) {
        std::string day_str = (date.day < 10 ? "0" : "") + std::to_string(date.day);
        std::string month_str = (date.month < 10 ? "0" : "") + std::to_string(date.month);
        std::string date_string = day_str + "/" + month_str + "/" + std::to_string(date.year);
        return date_string;
    };

    auto get_diary_from_date = [selected_index, this]() -> DiaryData {
        if (*selected_index >= 0 && *selected_index < diary_dates_combined.size()) {
            for (const auto& entry : diary_entries) {
                if (entry.date == diary_dates_combined[*selected_index]) {
                    return entry;
                }
            }
        }
        return DiaryData({-1, Date({0,0,0,0,0}), "", ""});
    };

    auto addEntryButton = Button("Add Entry", [data, this, is_valid_diary, get_diary_from_date] {
        if (!newEntryName.empty() && !newEntryContent.empty()) {
            DiaryData addedEntry = addDiaryEntry(data);
            diary_entries.push_back(addedEntry);
            diary_dates_combined.push_back(addedEntry.date);
            newEntryName.clear();
            newEntryContent.clear();
            *is_valid_diary = get_diary_from_date() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;
        }
    });

    auto selected_diary_view_state_0 = ftxui::Container::Vertical({
        ftxui::Renderer([] { return text("Add Today Entry:"); }),
        newEntryNameInput,
        newEntryContentInput,
        addEntryButton
    });

    auto selected_diary_view_state_1 = ftxui::Container::Vertical({
        Renderer([get_diary_from_date] {
            auto diary = get_diary_from_date();
            return hbox(
                ftxui::text("Diary: "),
                paragraph(diary.diaryEntryName)
            );
        }),
        Renderer([get_diary_from_date] {
            auto diary = get_diary_from_date();
            return hbox(paragraph(diary.diaryEntry));
        }),
    });

    *is_valid_diary = get_diary_from_date() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;

    auto selected_diary_view = ftxui::Container::Tab({
        selected_diary_view_state_0,
        selected_diary_view_state_1,
    }, is_valid_diary.get());

    auto up_button = Button(">>", [selected_index, this, is_valid_diary, get_diary_from_date] {
        if (*selected_index > 0) {
            (*selected_index)--;
            *is_valid_diary = get_diary_from_date() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;
        }
    });

    auto down_button = Button("<<", [selected_index, this, is_valid_diary, get_diary_from_date] {
        if (*selected_index < diary_dates_combined.size() - 1) {
            (*selected_index)++;
            *is_valid_diary = get_diary_from_date() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;
        }
    });

    auto selected_date_view = ftxui::Container::Horizontal({
        down_button,
        Renderer([date_to_string, this, selected_index] {
            if (*selected_index >= 0 && *selected_index < diary_dates_combined.size()) {
                return text("  " + date_to_string(diary_dates_combined[*selected_index]) + "  ");
            }
            return text("Invalid Date");
        }) | ftxui::center,
        up_button,
    });


    auto container = Container::Vertical({
        ftxui::Renderer([] { return filler(); }),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Container::Vertical({
            ftxui::Renderer([] { return filler(); }),
            selected_date_view,
            selected_diary_view
        }) | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180),
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);

    return container;
}