#Copyright 2022, Battelle Energy Alliance, LLC
# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("GeneralizedSimulatorIntermediary")
add_compile_definitions(WINDOWS)

# Include sub-projects.
add_subdirectory ("GeneralizedSimulatorIntermediary")

