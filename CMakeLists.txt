cmake_minimum_required(VERSION 3.10)
project(JOB-SCHEDULER)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(job-scheduler ${SOURCES})

# Link pthread library (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(job-scheduler Threads::Threads)