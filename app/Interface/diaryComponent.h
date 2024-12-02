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
    void getTodayDate(std::string &today_date);    
};

#endif // DIARYCOMPONENT_H