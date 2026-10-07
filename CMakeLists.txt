cmake_minimum_required(VERSION 3.15)
project(TicTacToeMultiplayer)

# Enforce modern C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate SQLite3 on the host machine
find_package(SQLite3 REQUIRED)

# Define ALL the source files for the backend executable
add_executable(Server
    backend/main.cpp
    backend/Database.cpp
    backend/Board.cpp
    backend/Player.cpp
    backend/GameSession.cpp
    backend/User.cpp
    backend/Auth.cpp
)

# Link the SQLite3 library and standard threading
target_link_libraries(Server SQLite::SQLite3 pthread)
