#include "../../Headers/Interface/milestonesComponent.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "../../Headers/applicationManager.h"
#include "../../l4dFiles/out/l4dFiles.hpp"

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

    for (auto& milestone : data->milestonesData) {
        if (milestone.id == id) {
            auto it = std::find_if(milestone.progressPoints.begin(), milestone.progressPoints.end(), [&today](const MilestoneProgressPoint& point) {
                return point.date.year == today.year && point.date.month == today.month && point.date.day == today.day;
            });

            if (it == milestone.progressPoints.end()) {
                MilestoneProgressPoint newPoint;
                newPoint.date = today;
                newPoint.isCompleted = newProgressPoint.isCompleted;
                milestone.progressPoints.push_back(newPoint);
            }
            else {
                notificationText = "A progress point for today already exists";
            }
        }
    }
}

MilestonesProgressPoints MilestonesComponent::getMilestonesPoints(FileData *data, int id) {
    MilestonesProgressPoints points;

    for (const auto& milestone : data->milestonesData) {
        if (milestone.id == id) {
            for (const auto& point : milestone.progressPoints) {
                points.progressPoints.push_back(point);
            }
        }
    }

    return points;
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
            milestones.milestonesIds.erase(milestones.milestonesIds.begin() + selectedMilestones);
        }
    });

    auto clearNotification = [this] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        notificationText.clear();
    };

    auto addProgressPointButton = ftxui::Button("Add Progress Point", [data, clearNotification, this] {
        if (!milestones.milestones.empty() && selectedMilestones < static_cast<int>(milestones.milestones.size())) {
            addMilestoneProgressPoint(data, milestones.milestonesIds[selectedMilestones]);
        }

        newProgressPoint.isCompleted = false;

        std::thread(clearNotification).detach();
    });

    auto selectedMilestoneLabel = Renderer([this] {
        if (milestones.milestones.empty() || selectedMilestones >= static_cast<int>(milestones.milestones.size())) {
            return ftxui::text("Selected Milestone: No milestone is selected");
        }
        return hbox(
            ftxui::text("Selected Milestone: "),
            paragraph(milestones.milestones[selectedMilestones])
        );
    });

    auto selectedMilestoneDescription = Renderer([data, this] {
        if (milestones.milestones.empty() || selectedMilestones >= static_cast<int>(milestones.milestones.size())) {
            return ftxui::text("Description: No Milestone selected");
        }
        const auto& milestone = data->milestonesData[selectedMilestones];
        std::string description = milestone.milestoneDescription;
        if (description.empty()) {
            description = "Description: No description";
        }
        return hbox(
            ftxui::text("Description: "),
            paragraph(description)
        );
    });

    auto milestonesDisplay = ftxui::Renderer(milestonesList, [milestonesList, this] {
        if (milestones.milestones.empty()) {
            return ftxui::text("Milestones list is empty");
        }
        return milestonesList->Render() | ftxui::vscroll_indicator | ftxui::frame;
    });

    auto milestonesPointsDisplay = ftxui::Container::Vertical({
        ftxui::Renderer([data, this] {
            if (milestones.milestones.empty() || selectedMilestones >= static_cast<int>(milestones.milestones.size())) {
                return ftxui::text("No milestones selected");
            }

            auto milestonePoints = getMilestonesPoints(data, milestones.milestonesIds[selectedMilestones]);

            auto c = ftxui::Canvas(104, 78);


            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            Date min_date;
            min_date.year = tm.tm_year + 1900;
            min_date.month = tm.tm_mon + 1;
            min_date.day = tm.tm_mday;
            min_date.hour = tm.tm_hour;
            min_date.minute = tm.tm_min;

            auto max_date = min_date;
            tm = {};
            tm.tm_year = min_date.year - 1900;
            tm.tm_mon = min_date.month - 1;
            tm.tm_mday = min_date.day;
            std::time_t time = std::mktime(&tm);
            time -= 104 * 24 * 60 * 60; 
            std::tm *new_tm = std::localtime(&time);
            max_date.year = new_tm->tm_year + 1900;
            max_date.month = new_tm->tm_mon + 1;
            max_date.day = new_tm->tm_mday;

            // Draw the date range at the top of the canvas
            std::string date_range = "From: " + std::to_string(max_date.year) + "-" + std::to_string(max_date.month) + "-" + std::to_string(max_date.day) +
                                     "  To: " + std::to_string(min_date.year) + "-" + std::to_string(min_date.month) + "-" + std::to_string(min_date.day);
            c.DrawText(0, 2, date_range, Color::White);

            int x = 0;
            int y = 6;

            for (size_t i = 0; i < 13; i++) {
                for (size_t j = 0; j < 8; j++) {
                    c.DrawText(x, y, std::to_string(i), Color::White);

                    auto point_date = max_date;
                    auto days_to_add = std::chrono::days(-((i * 8) + j));
                    auto new_date = std::chrono::system_clock::from_time_t(std::mktime(&tm)) + days_to_add;
                    std::time_t new_time = std::chrono::system_clock::to_time_t(new_date);
                    std::tm new_tm = *std::localtime(&new_time);
                    point_date.year = new_tm.tm_year + 1900;
                    point_date.month = new_tm.tm_mon + 1;
                    point_date.day = new_tm.tm_mday;

                    auto it = std::find_if(milestonePoints.progressPoints.begin(), milestonePoints.progressPoints.end(), [&point_date](const MilestoneProgressPoint& point) {
                        return point.date.year == point_date.year && point.date.month == point_date.month && point.date.day == point_date.day;
                    });

                    if (it != milestonePoints.progressPoints.end()) {
                        const auto& point = *it;

                        auto color = point.isCompleted ? Color::GreenLight : Color::RedLight;

                        c.DrawBlock(x, y, 1, color); 
                        c.DrawBlock(x + 1, y, 1, color); 
                        c.DrawBlock(x + 2, y, 1, color); 
                        c.DrawBlock(x + 3, y, 1, color); 

                        c.DrawBlock(x, y + 1, 1, color); 
                        c.DrawBlock(x + 1, y + 1, 1, color); 
                        c.DrawBlock(x + 2, y + 1, 1, color); 
                        c.DrawBlock(x + 3, y + 1, 1, color); 
                        
                        c.DrawBlock(x, y + 2, 1, color); 
                        c.DrawBlock(x + 1, y + 2, 1, color); 
                        c.DrawBlock(x + 2, y + 2, 1, color); 
                        c.DrawBlock(x + 3, y + 2, 1, color); 

                        c.DrawBlock(x, y + 3, 1, color); 
                        c.DrawBlock(x + 1, y + 3, 1, color); 
                        c.DrawBlock(x + 2, y + 3, 1, color); 
                        c.DrawBlock(x + 3, y + 3, 1, color); 
                    }
                    else {
                        auto color = Color::Blue;

                        c.DrawBlock(x, y, 1, color); 
                        c.DrawBlock(x + 1, y, 1, color); 
                        c.DrawBlock(x + 2, y, 1, color); 
                        c.DrawBlock(x + 3, y, 1, color); 

                        c.DrawBlock(x, y + 1, 1, color); 
                        c.DrawBlock(x + 1, y + 1, 1, color); 
                        c.DrawBlock(x + 2, y + 1, 1, color); 
                        c.DrawBlock(x + 3, y + 1, 1, color); 
                        
                        c.DrawBlock(x, y + 2, 1, color); 
                        c.DrawBlock(x + 1, y + 2, 1, color); 
                        c.DrawBlock(x + 2, y + 2, 1, color); 
                        c.DrawBlock(x + 3, y + 2, 1, color); 

                        c.DrawBlock(x, y + 3, 1, color); 
                        c.DrawBlock(x + 1, y + 3, 1, color); 
                        c.DrawBlock(x + 2, y + 3, 1, color); 
                        c.DrawBlock(x + 3, y + 3, 1, color); 
                    }
                    

                    x += 8;
                    if (x >= 104) {
                        x = 0;
                        y += 8;
                    }
                }
            }

            return canvas(std::move(c));
        }) 
    });     

    auto notification = ftxui::Renderer([this] {
        if (notificationText.empty()) {
            return ftxui::text("");
        }
        return ftxui::text(notificationText);
    });

    return ftxui::Container::Horizontal({
        ftxui::Renderer([] { return filler(); }),
        ftxui::Container::Vertical({
            Renderer([] { return text("Milestones list:"); }),
            milestonesDisplay | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 20),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Progress Points:"); }),
                milestonesPointsDisplay,
            }),
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
                Renderer([] { return text("Add Milestone:"); }),
                ftxui::Renderer([] { return ftxui::text("Milestone Title:"); }),
                newMilestoneInput | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30),
                ftxui::Renderer([] { return ftxui::text("Description:"); }),
                descriptionInput | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 45),
                addMilestoneButton | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
            }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                selectedMilestoneLabel,
                selectedMilestoneDescription,
                removeMilestoneButton | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
            }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Add Progress Point:"); }),
                newProgressPointInput,
                addProgressPointButton | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20),
                ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
                notification,
            }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Progess Points Legend:"); }),
                ftxui::Renderer([] { return ftxui::text("Green Box: Completed") | ftxui::color(ftxui::Color::GreenLight); }),
                ftxui::Renderer([] { return ftxui::text("Red Box: Not Completed") | ftxui::color(ftxui::Color::RedLight); }),
                ftxui::Renderer([] { return ftxui::text("Blue Box: Not Added") | ftxui::color(ftxui::Color::Blue); }),
            })
        }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 50),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);
}