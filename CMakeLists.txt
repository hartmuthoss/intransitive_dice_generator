####################################################
# CMakeLists.txt for project intransive_dice
####################################################
cmake_minimum_required(VERSION 3.28)
project(intransive_dice)

# Intransitive dice library
include_directories(src)
add_library(${PROJECT_NAME}_lib STATIC
    src/dice.cpp
    src/dice_generator.cpp
    src/dice_util.cpp
)

# Intransitive dice generator
add_executable(intransitive_dice_generator src/intransitive_dice_generator.cpp)
target_link_libraries(intransitive_dice_generator ${PROJECT_NAME}_lib)
