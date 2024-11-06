#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

std::vector<std::string> diary = {"Info 1", "Info 2", "Info 3"};
int selected_diary = 0;

Component diaryComponent() {
    return CreateComponent(diary, selected_diary, "About");
}