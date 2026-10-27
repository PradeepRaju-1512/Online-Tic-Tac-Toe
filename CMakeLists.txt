cmake_minimum_required(VERSION 3.10)
project(lan_tictactoe CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required system packages
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Use pkg-config to locate libpqxx installed via apt
pkg_check_modules(PQXX REQUIRED libpqxx)

# Include directories pointing to your third_party paths
include_directories(
    include
    third_party/Crow/include
    third_party/asio/include
    ${PQXX_INCLUDE_DIRS}
)

link_directories(${PQXX_LIBRARY_DIRS})

# Collect source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(lan_tictactoe ${SOURCES})

# Define a macro so Database.cpp knows libpqxx is available
if(PQXX_FOUND)
    target_compile_definitions(lan_tictactoe PRIVATE USE_PQXX HAVE_LIBPQXX)
endif()

# Link libraries
target_link_libraries(lan_tictactoe 
    PRIVATE 
    ${PQXX_LIBRARIES} 
    Threads::Threads
)