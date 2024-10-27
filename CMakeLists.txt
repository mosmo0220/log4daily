cmake_minimum_required(VERSION 3.20)

# Set the project name
project(log4daily)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(log4daily ${PROJECT_SOURCE_DIR}/app/main.cpp)
add_executable(tests ${PROJECT_SOURCE_DIR}/app/tests.cpp)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link libraries (if any)
# target_link_libraries(log4daily <library_name>)
# Specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)