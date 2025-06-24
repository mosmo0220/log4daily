#ifndef DIARYCOMPONENT_H
#define DIARYCOMPONENT_H

#include <vector>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "../../../l4dFiles/out/l4dFiles.hpp"

using namespace ftxui;

class DiaryComponent {
public:
    DiaryComponent() {};
    Component diaryComponent(FileData *data);
    DiaryData addDiaryEntry(FileData *data);
private:
    std::string newEntryName;
    std::string newEntryContent;
    std::vector<DiaryData> diaryEntries;
    std::vector<Date> combinedDiaryDates;
};

#endif // DIARYCOMPONENT_H