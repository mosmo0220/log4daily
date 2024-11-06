#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

std::vector<std::string> about = {"About 1", "About 2", "About 3"};
int selected_about = 0;

Component aboutComponent() {
    return CreateComponent(about, selected_about, "About");
}