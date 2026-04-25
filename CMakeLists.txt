cmake_minimum_required(VERSION 3.16)
project(AgentBasedModeling VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL)
find_package(Threads REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/GridWorld.cpp
    src/Agent.cpp
    src/Pathfinder.cpp
    src/Simulation.cpp
)

# Header files
set(HEADERS
    include/MainWindow.h
    include/GridWorld.h
    include/Agent.h
    include/Pathfinder.h
    include/Simulation.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGL
    Threads::Threads
)

# Enable Qt MOC
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

# Testing
# enable_testing()
# find_package(Catch2 REQUIRED)
# add_subdirectory(tests)

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)