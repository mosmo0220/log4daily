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
private:
    std::string newEntryName;
    std::string newEntryContent;
};

#endif // DIARYCOMPONENT_H