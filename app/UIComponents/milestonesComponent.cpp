#include "milestonesComponent.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "../Storage/localStorage.h"
#include "../mainThread.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

MilestonesComponent::MilestonesComponent() {
    selectedMilestones = 0;
}

int MilestonesComponent::addMilestone(FileData *data) {
    if (!data) return -1;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Date startDate;
    startDate.year = tm.tm_year + 1900;
    startDate.month = tm.tm_mon + 1;
    startDate.day = tm.tm_mday;
    startDate.hour = tm.tm_hour;
    startDate.minute = tm.tm_min;
    startDate.second = 0;

    MilestonesData newMilestone;
    newMilestone.id = data->milestonesData.size() + 1;
    newMilestone.startDate = startDate;
    newMilestone.milestoneName = newMilestoneName;
    newMilestone.milestoneDescription = newMilestoneDescription;

    data->milestonesData.push_back(newMilestone);
    return newMilestone.id;
}

void MilestonesComponent::removeMilestone(FileData *data, int id) {
    if (!data) return;

    auto it = std::remove_if(data->milestonesData.begin(), data->milestonesData.end(), [id](const MilestonesData& milestone) {
        return milestone.id == id;
    });
    data->milestonesData.erase(it, data->milestonesData.end());
}

void MilestonesComponent::addMilestoneProgressPoint(FileData *data, int id) {
    if (!data) return;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Date today;
    today.year = tm.tm_year + 1900;
    today.month = tm.tm_mon + 1;
    today.day = tm.tm_mday;
    today.hour = tm.tm_hour;
    today.minute = tm.tm_min;
    today.second = 0;

    for (auto& milestone : data->milestonesData) {
        if (milestone.id == id) {
            auto it = std::find_if(milestone.progressPoints.begin(), milestone.progressPoints.end(), [&today](const MilestoneProgressPoint& point) {
                return point.date.year == today.year && point.date.month == today.month && point.date.day == today.day;
            });

            if (it == milestone.progressPoints.end()) {
                MilestoneProgressPoint newPoint;
                newPoint.date = today;
                newPoint.isCompleted = newProgressPoint.isCompleted;
                newPoint.progressDescription = newProgressPoint.progressDescription;
                milestone.progressPoints.push_back(newPoint);
            }
        }
    }
}

Component MilestonesComponent::renderMilestonesComponent(FileData *data) {
    if (!data) return ftxui::Renderer([] { return ftxui::text("Error: Data is null"); });

    milestones.milestones.clear();
    milestones.milestonesIds.clear();

    for (const auto& milestone : data->milestonesData) {
        milestones.milestones.push_back(milestone.milestoneName);
        milestones.milestonesIds.push_back(milestone.id);
    }
    
    auto milestonesList = ftxui::Menu(&milestones.milestones, &selectedMilestones) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30);

    auto newMilestoneInput = ftxui::Input(&newMilestoneName, "New Milestone");
    auto descriptionInput = ftxui::Input(&newMilestoneDescription, "Description");

    auto newProgressPointInput = ftxui::Checkbox("Completed", &newProgressPoint.isCompleted);
    auto newProgressPointDescription = ftxui::Input(&newProgressPoint.progressDescription, "Progress Description");

    auto addMilestoneButton = ftxui::Button("Add Milestone", [data, this] {
        if (!newMilestoneName.empty()) {
            int newId = addMilestone(data);

            if (newId != -1) {
                milestones.milestones.push_back(newMilestoneName);
                milestones.milestonesIds.push_back(newId);

                newMilestoneName.clear();
                newMilestoneDescription.clear();
            }
        }
    });

    auto removeMilestoneButton = ftxui::Button("Remove Milestone", [data, this] {
        if (!milestones.milestones.empty() && selectedMilestones < static_cast<int>(milestones.milestones.size())) {
            int idToRemove = milestones.milestonesIds[selectedMilestones];
            removeMilestone(data, idToRemove);
            milestones.milestones.erase(milestones.milestones.begin() + selectedMilestones);
        }
    });

    auto addProgressPointButton = ftxui::Button("Add Progress Point", [data, this] {
        if (!milestones.milestones.empty() && selectedMilestones < static_cast<int>(milestones.milestones.size())) {
            addMilestoneProgressPoint(data, milestones.milestonesIds[selectedMilestones]);
        }
    });

    auto milestonesDisplay = ftxui::Renderer(milestonesList, [milestonesList, this] {
        if (milestones.milestones.empty()) {
            return ftxui::text("Milestones list is empty");
        }
        return milestonesList->Render();
    });

    auto milestonesListLabel = Renderer([] {
        return text("Milestones list:");
    });
    auto selectedMilestoneLabel = Renderer([] {
        return text("Add Milestone:");
    });

    return ftxui::Container::Horizontal({
        ftxui::Renderer([] { return filler(); }),
        ftxui::Container::Vertical({
            milestonesListLabel,
            milestonesDisplay,
        }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 50),
        Renderer([]() -> Element {
            return hbox({
                filler() | size(WIDTH, EQUAL, 2),
                separator() | size(WIDTH, EQUAL, 1),
                filler() | size(WIDTH, EQUAL, 2),
            });
        }),
        ftxui::Container::Vertical({
            ftxui::Container::Vertical({
                selectedMilestoneLabel,
                ftxui::Renderer([] { return ftxui::text("Milestone Title:"); }),
                newMilestoneInput | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30),
                ftxui::Renderer([] { return ftxui::text("Description:"); }),
                descriptionInput | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 45),
                ftxui::Container::Horizontal({
                    addMilestoneButton | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
                    removeMilestoneButton | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
                }),
            }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Add Progress Point:"); }),
                newProgressPointInput,
                newProgressPointDescription | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 45),
                addProgressPointButton | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
                ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Progress Points:"); }),
                ftxui::Renderer([] { return ftxui::text("No progress points"); }),
            }),
        }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 50),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);
}