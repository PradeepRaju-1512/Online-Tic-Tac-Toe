# Online-Tic-Tac-Toe
251144, 251120  -- Assignment

# Online Multiplayer Tic-Tac-Toe

A real-time, multiplayer Tic-Tac-Toe web application built with a high-performance C++ backend and a lightweight Vanilla JavaScript, HTML, and CSS frontend. This project demonstrates core Object-Oriented Programming (OOP) principles, multithreaded game session management, and WebSocket-based network communication.

## Features

* **Real-Time Multiplayer:** Play against an opponent over the network with instant move syncing via WebSockets.
* **Robust C++ Backend:** Game state, logic, and validation are securely handled server-side to prevent client spoofing.
* **Thread-Safe Concurrency:** Multiple distinct game sessions are supported simultaneously using `std::thread` and `std::mutex` to prevent data races.
* **Strict OOP Design:** Clean separation of concerns using modular `Board`, `Player`, and `GameSession` classes.
* **Lightweight UI:** Zero heavy frontend frameworks—just pure DOM manipulation and CSS Grid for a snappy, responsive user experience.

## Tech Stack

* **Frontend:** HTML5, CSS3 (CSS Grid), Vanilla JavaScript, Native Browser WebSocket API.
* **Backend:** C++ (C++17 or later).
* **Networking:** C++ WebSocket library (e.g., Crow or uWebSockets).
* **Concurrency:** Native C++ `<thread>` and `<mutex>`.

## OOP Architecture

The backend logic is strictly separated into three primary classes:

* **`Board`:** Manages the internal 3x3 grid data structure, validates incoming move coordinates, and evaluates win/draw conditions after every turn.
* **`Player`:** Encapsulates player metadata, storing their assigned symbol ('X' or 'O') and their unique network connection handle to route messages correctly.
* **`GameSession`:** Acts as the master controller for a single match. It pairs two `Player` objects, owns the central `Board` instance, and manages the state machine (e.g., Waiting, In Progress, Game Over). Mutex locks are applied here to guarantee thread safety when both clients communicate simultaneously.

## Prerequisites

* C++ Compiler (GCC, Clang, or MSVC) with C++17 support.
* CMake (for building the backend).
* A C++ WebSocket library installed and linked.
* A modern web browser.

## Installation & Setup

1. **Clone the repository:**
```bash
git clone https://github.com/yourusername/cpp-tictactoe.git
cd cpp-tictactoe

```


2. **Build the C++ Server:**
```bash
mkdir build && cd build
cmake ..
make

```


3. **Run the Server:**
```bash
./TicTacToeServer

```


*The server will boot up and listen for incoming WebSocket connections on `ws://localhost:9000`.*
4. **Launch the Client:**
Open `frontend/index.html` in your web browser. Open a second tab or a different browser to connect as Player 2.

---

Which C++ WebSocket library should we lock in for the networking—Crow (easier syntax) or uWebSockets (maximum performance)?
