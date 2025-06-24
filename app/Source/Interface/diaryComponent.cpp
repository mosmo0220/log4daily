#include "../../Headers/Interface/diaryComponent.h"

#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "../../l4dFiles/out/l4dFiles.hpp"

using namespace ftxui;

DiaryData DiaryComponent::addDiaryEntry(FileData *fileData) {
    auto currentTime = std::time(nullptr);
    auto localTime = *std::localtime(&currentTime);

    Date today;
    today.year = localTime.tm_year + 1900;
    today.month = localTime.tm_mon + 1;
    today.day = localTime.tm_mday;
    today.hour = 0;
    today.minute = 0;

    DiaryData newDiaryEntry;
    newDiaryEntry.id = fileData->diaryData.size() + 1;
    newDiaryEntry.date = today;
    newDiaryEntry.diaryEntryName = newEntryName;
    newDiaryEntry.diaryEntry = newEntryContent;

    fileData->diaryData.push_back(newDiaryEntry);
    return newDiaryEntry;
}

Component DiaryComponent::diaryComponent(FileData *fileData) {
    diaryEntries = fileData->diaryData;

    std::vector<Date> diaryDates;
    for (DiaryData entry : diaryEntries) {
        diaryDates.push_back(entry.date);
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);

    Date todayDate = Date(localTime->tm_mday, 1 + localTime->tm_mon, 1900 + localTime->tm_year, 0, 0);

    if (std::find(diaryDates.begin(), diaryDates.end(), todayDate) == diaryDates.end()) {
        combinedDiaryDates.push_back(todayDate);
    }
    combinedDiaryDates.insert(combinedDiaryDates.end(), diaryDates.begin(), diaryDates.end());

    std::sort(combinedDiaryDates.begin(), combinedDiaryDates.end(), [](Date a, Date b) {
        if (a.year != b.year) {
            return a.year > b.year;
        }
        if (a.month != b.month) {
            return a.month > b.month;
        }
        return a.day > b.day;
    });

    auto selectedIndex = std::make_shared<int>(0);
    auto isValidDiary = std::make_shared<int>(0);

    auto newEntryNameInput = Input(&newEntryName, "New Entry Name: ");
    auto newEntryContentInput = Input(&newEntryContent, "New Entry Content: ");

    auto dateToString = [](Date date) {
        std::string dayStr = (date.day < 10 ? "0" : "") + std::to_string(date.day);
        std::string monthStr = (date.month < 10 ? "0" : "") + std::to_string(date.month);
        std::string dateString = dayStr + "/" + monthStr + "/" + std::to_string(date.year);
        return dateString;
    };

    auto getDiaryFromDate = [selectedIndex, this]() -> DiaryData {
        if (*selectedIndex >= 0 && *selectedIndex < combinedDiaryDates.size()) {
            for (const auto& entry : diaryEntries) {
                if (entry.date == combinedDiaryDates[*selectedIndex]) {
                    return entry;
                }
            }
        }
        return DiaryData({-1, Date({0,0,0,0,0}), "", ""});
    };

    auto addEntryButton = Button("Add Entry", [fileData, this, isValidDiary, getDiaryFromDate] {
        if (!newEntryName.empty() && !newEntryContent.empty()) {
            DiaryData addedEntry = addDiaryEntry(fileData);
            diaryEntries.push_back(addedEntry);
            combinedDiaryDates.push_back(addedEntry.date);
            newEntryName.clear();
            newEntryContent.clear();
            *isValidDiary = getDiaryFromDate() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;
        }
    });

    auto addEntryView = ftxui::Container::Vertical({
        ftxui::Renderer([] { return text("Add Today Entry:"); }),
        newEntryNameInput,
        newEntryContentInput,
        Renderer([]{
            return ftxui::separatorEmpty();
        }),
        addEntryButton
    });

    auto viewDiaryEntry = ftxui::Container::Vertical({
        Renderer([getDiaryFromDate] {
            auto diary = getDiaryFromDate();
            return hbox(
                ftxui::text("Diary: "),
                paragraph(diary.diaryEntryName)
            ) | ftxui::bold;
        }),
        Renderer([]{
            return ftxui::separatorEmpty();
        }),
        Renderer([getDiaryFromDate] {
            auto diary = getDiaryFromDate();
            return hbox(paragraph(diary.diaryEntry));
        }),
    });

    *isValidDiary = getDiaryFromDate() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;

    auto selectedDiaryView = ftxui::Container::Tab({
        addEntryView,
        viewDiaryEntry,
    }, isValidDiary.get());

    auto upButton = Button(">>", [selectedIndex, this, isValidDiary, getDiaryFromDate] {
        if (*selectedIndex > 0) {
            (*selectedIndex)--;
            *isValidDiary = getDiaryFromDate() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;
        }
    });

    auto downButton = Button("<<", [selectedIndex, this, isValidDiary, getDiaryFromDate] {
        if (*selectedIndex < combinedDiaryDates.size() - 1) {
            (*selectedIndex)++;
            *isValidDiary = getDiaryFromDate() == DiaryData({-1, Date({0,0,0,0,0}), "", ""}) ? 0 : 1;
        }
    });

    auto selectedDateView = ftxui::Container::Horizontal({
        downButton,
        Renderer([dateToString, this, selectedIndex] {
            if (*selectedIndex >= 0 && *selectedIndex < combinedDiaryDates.size()) {
                return text("  " + dateToString(combinedDiaryDates[*selectedIndex]) + "  ");
            }
            return text("Invalid Date");
        }) | ftxui::center,
        upButton,
    });

    auto container = Container::Vertical({
        ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
        selectedDateView | ftxui::center,
        ftxui::Renderer([] { return filler(); }),
        ftxui::Container::Vertical({
            selectedDiaryView | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 120)
        }) | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);

    return container;
}
