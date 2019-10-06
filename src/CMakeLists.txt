# CMakeList.txt : CMake project for Concurrency, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

## Introduction and Running Threads
# add_executable (Concurrency "Introduction and Running Threads/testThreads.cpp")
# add_executable (Concurrency "Introduction and Running Threads/runningASingleThread.cpp")
# add_executable (Concurrency "Introduction and Running Threads/lambdaFunction.cpp")
# add_executable (Concurrency "Introduction and Running Threads/startingAThreadWithFunctionObjects.cpp")
# add_executable (Concurrency "Introduction and Running Threads/startingAThreadWithVariadicTemplatesAndMemberFunctions.cpp")
add_executable (Concurrency "Introduction and Running Threads/runningMultipleThreads.cpp")

# TODO: Add tests and install targets if needed.
