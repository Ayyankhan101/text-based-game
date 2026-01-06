cmake_minimum_required(VERSION 3.16)
project(WolfGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt prefix path
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt6")

# Find Qt packages - prioritize Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Include directories
include_directories(include)

# Qt MOC
qt6_wrap_cpp(MOC_SOURCES include/game_window.h)

# Common sources for all executables (non-GUI)
set(COMMON_SOURCES src/action_queue.cpp src/decision_tree.cpp src/game_stack.cpp src/inventory.cpp src/pack.cpp src/priority_queue.cpp)

# Executable
add_executable(WolfGame src/main.cpp src/game_window.cpp ${COMMON_SOURCES} ${MOC_SOURCES})

# Validator executable
add_executable(Validator src/validator.cpp ${COMMON_SOURCES})

# Balance tester executable
add_executable(BalanceTester src/balance_tester.cpp ${COMMON_SOURCES})

# Link Qt libraries
target_link_libraries(WolfGame Qt6::Core Qt6::Widgets Qt6::Gui)
target_link_libraries(Validator)
target_link_libraries(BalanceTester)