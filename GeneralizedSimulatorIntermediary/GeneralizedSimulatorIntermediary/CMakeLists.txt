# CMakeList.txt : CMake project for GeneralizedSimulatorIntermediary, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (GeneralizedSimulatorIntermediary "GeneralizedSimulatorIntermediary.cpp" "GeneralizedSimulatorIntermediary.h" "WinSockHandler.h" "Threading/Thread.cpp" "Threading/Thread.h" "Threading/ThreadPool.h" "Threading/ThreadPool.cpp" "NetworkHandling/NetworkHandler.h" "NetworkHandling/NetworkHandler.cpp" "DataHandling/InputScanning.h" "DataHandling/InputScanning.cpp" "DataHandling/SharedResources/ThreadsafeDataValue.cpp" "DataHandling/SharedResources/ThreadsafeDataValue.h" "DataHandling/SharedResources/SharedResource.h" "DataHandling/SharedResources/SharedResource.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GeneralizedSimulatorIntermediary PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
