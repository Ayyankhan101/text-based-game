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

# Source files
file(GLOB SOURCES "src/*.cpp")

# Filter out validator and balance tester from main executable
list(FILTER SOURCES EXCLUDE REGEX "validator\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX "balance_tester\\.cpp$")

# Executable
add_executable(WolfGame ${SOURCES} ${MOC_SOURCES})

# Create a list of common sources (excluding main.cpp, validator.cpp, balance_tester.cpp)
set(COMMON_SOURCES ${SOURCES})
list(FILTER COMMON_SOURCES EXCLUDE REGEX "main\\.cpp$")

# Validator executable
add_executable(Validator src/validator.cpp ${COMMON_SOURCES} ${MOC_SOURCES})

# Balance tester executable  
add_executable(BalanceTester src/balance_tester.cpp ${COMMON_SOURCES} ${MOC_SOURCES})

# Link Qt libraries
target_link_libraries(WolfGame Qt6::Core Qt6::Widgets Qt6::Gui)
target_link_libraries(Validator Qt6::Core Qt6::Widgets Qt6::Gui)
target_link_libraries(BalanceTester Qt6::Core Qt6::Widgets Qt6::Gui)