#ifndef ABOUTCOMPONENT_H
#define ABOUTCOMPONENT_H

#include <vector>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

class AboutComponent {
public:
    AboutComponent() {};
    Component aboutComponent();
private:
    std::vector<std::string> about = {"About 1", "About 2", "About 3"};
    int selected_about = 0;
};

#endif // ABOUTCOMPONENT_H