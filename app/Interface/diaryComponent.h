#ifndef DIARYCOMPONENT_H
#define DIARYCOMPONENT_H

#include <vector>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

class DiaryComponent {
public:
    DiaryComponent() {};
    Component diaryComponent();
private:
    std::vector<std::string> diary = {"Info 1", "Info 2", "Info 3"};
    int selected_diary = 0;
};

#endif // DIARYCOMPONENT_H