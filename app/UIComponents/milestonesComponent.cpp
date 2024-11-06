#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

std::vector<std::string> milestones = {"Milestone 1", "Milestone 2", "Milestone 3"};
int selected_milestones = 0;

Component milestonesComponent() {
    return CreateComponent(milestones, selected_milestones, "Milestones");
}