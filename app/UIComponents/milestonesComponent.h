#ifndef MILESTONES_COMPONENT_H
#define MILESTONES_COMPONENT_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "../Storage/localStorage.h"
#include "../mainThread.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

struct MilestonesListData {
    std::vector<std::string> milestones;
    std::vector<int> milestonesIds;
};

struct NewMilestoneProgressPoint {
    bool isCompleted;
    std::string progressDescription;
};

class MilestonesComponent {
public:
    MilestonesComponent();
    /**
     * @brief Adds a new milestone to the list.
     * 
     * This function adds a new milestone to the list of milestones.
     */
    int addMilestone(FileData *data);
    /**
     * @brief Removes a milestone from the list.
     * 
     * This function removes a milestone from the list of milestones.
     * The milestone with the specified ID is removed from the list.
     */
    void removeMilestone(FileData *data, int id);
    /**
     * @brief Adds a progress point to a milestone.
     * 
     * This function adds a progress point to a milestone in the list of milestones.
     * The progress point is added to the milestone with the specified ID.
     */
    void addMilestoneProgressPoint(FileData *data, int id);
    /**
     * @brief Creates the milestones component.
     * 
     * This function creates the milestones component, which displays the list of milestones and allows the user to interact with them.
     * The component includes options to add new milestones, remove existing milestones, and add progress points to milestones.
     */
    Component renderMilestonesComponent(FileData *data);

private:
    MilestonesListData milestones;
    int selectedMilestones = 0;
    std::string newMilestoneName;
    std::string newMilestoneDescription;

    NewMilestoneProgressPoint newProgressPoint;
};

#endif // MILESTONES_COMPONENT_H