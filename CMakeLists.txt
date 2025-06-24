cmake_minimum_required(VERSION 3.22)

# Set the project name
project(log4daily
    LANGUAGES CXX
)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------

# Add the executable
add_executable(log4daily 
    ${PROJECT_SOURCE_DIR}/app/Source/inputHandlers.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/applicationManager.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/uiRenderer.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/Interface/todosComponent.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/Interface/milestonesComponent.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/Interface/diaryComponent.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/Interface/exitComponent.cpp
    ${PROJECT_SOURCE_DIR}/app/Source/main.cpp
)

target_link_libraries(log4daily
    PRIVATE ${PROJECT_SOURCE_DIR}/l4dFiles/out/l4dFiles.a
)

target_link_libraries(log4daily
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
