#ifndef DIARYCOMPONENT_H
#define DIARYCOMPONENT_H

#include <vector>
#include <string>

#include "../Storage/localStorage.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

class DiaryComponent {
public:
    DiaryComponent() {};
    Component diaryComponent(FileData *data);
    DiaryData addDiaryEntry(FileData *data);
private:
    std::string newEntryName;
    std::string newEntryContent;
    std::vector<DiaryData> diary_entries;
    std::vector<Date> diary_dates_combined;
};

#endif // DIARYCOMPONENT_H