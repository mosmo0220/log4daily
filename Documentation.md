# Log4Daily

## Project Overview

Log4Daily is a feature-rich console application meticulously crafted in C++ to empower users in managing their daily tasks, tracking milestones, and journaling their thoughts and experiences. Leveraging the FTXUI library for its user interface, Log4Daily provides an intuitive and efficient environment for organizing daily life within the terminal.  Data persistence is achieved through JSON serialization, ensuring the secure storage and retrieval of user information.

## Key Features

* **Task Management:**
  * Create, modify, and delete tasks with detailed descriptions and deadlines.
  * Mark tasks as complete with an intuitive checkbox interface.
  * Filter tasks by completion status, due date, or keywords.
  * Prioritize tasks to focus on what matters most.
* **Milestone Tracking:**
  * Define milestones with clear objectives and target dates.
  * Track progress towards milestones with granular checkpoints.
  * Visualize milestone timelines to monitor progress over time.
* **Diary Entries:**
  * Record daily reflections, thoughts, and experiences in a private journal.
  * Organize journal entries by date for easy retrieval and review.
  * Search journal entries by keywords to quickly find relevant content.
* **User-Friendly Interface:**
  * FTXUI provides a responsive and visually appealing terminal-based interface.
  * Intuitive navigation and interaction with clear menus and prompts.
  * Customizable themes and color schemes to personalize the user experience.
* **Data Persistence:**
  * Robust data management using JSON serialization for reliable storage.
  * Secure storage of sensitive information in user-specific configuration files.
  * Automatic data backup and recovery mechanisms to prevent data loss.

## File Structure and Functionality

* **`handlers.cpp/.h`:** This module serves as the core input processing unit, responsible for parsing user commands, validating input, and dispatching actions to the appropriate modules.
* **`main.cpp`:** The entry point of the application. It initializes the main thread, sets up the FTXUI environment, and launches the main event loop.
* **`mainThread.cpp/.h`:**  This component manages the main thread's execution, coordinating interactions between the UI, data storage, and command processing modules. It handles the execution of user commands and updates the UI accordingly.
* **`renderUI.cpp`:**  This module is dedicated to rendering the user interface using FTXUI components. It dynamically generates UI elements based on user data and application state, providing a visually informative and interactive experience.
* **`localStorage.cpp/.h`:**  The local storage module provides an abstraction layer for data persistence. It handles the serialization and deserialization of user data to JSON format, ensuring data integrity and efficient retrieval.
* **`manageConfig.cpp`:**  This component is responsible for managing the application's configuration files, including user preferences, themes, and data storage locations. It ensures secure storage and retrieval of configuration data.
* **`todosComponent.cpp`:** This module implements the UI component specifically for managing to-dos. It provides functionalities for creating, editing, deleting, and filtering tasks, and presents them in an organized and visually appealing manner.

## Usage

Run the executable (`./log4daily`) with these optional command-line arguments:

* `--new [filename]` : Creates a new log file with the specified name.
* `--open [filename]` : Opens an existing log file.
* `--delete [filename]` : Deletes a log file.
* `--help` : Displays help information and usage instructions.
