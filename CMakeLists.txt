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
    ${PROJECT_SOURCE_DIR}/app/handlers.cpp
    ${PROJECT_SOURCE_DIR}/app/Storage/localStorage.cpp
    ${PROJECT_SOURCE_DIR}/app/mainThread.cpp
    ${PROJECT_SOURCE_DIR}/app/main.cpp
)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

target_link_libraries(log4daily
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
