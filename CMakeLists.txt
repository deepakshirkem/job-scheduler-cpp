cmake_minimum_required(VERSION 3.10)
project(JOB-SCHEDULER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all source files
file(GLOB SCHEDULER_SOURCES "src/*.cpp")

# Remove client.cpp from scheduler build
list(REMOVE_ITEM SCHEDULER_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/client.cpp")

# Main scheduler executable
add_executable(job-scheduler ${SCHEDULER_SOURCES})

# Separate client executable
add_executable(scheduler-client src/client.cpp)

# Link pthread
find_package(Threads REQUIRED)
target_link_libraries(job-scheduler Threads::Threads)