# Online Multiplayer Tic-Tac-Toe
Project by 251120 and 251144

This project is an online multiplayer Tic-Tac-Toe game made using **C++ and a web frontend**.

The main idea of the project is to allow two players to connect to the same game room and play Tic-Tac-Toe in real time. The C++ server handles the actual game logic while the frontend is used to display the game and interact with the players.

## Features
| Feature | Description |
|---|---|
| Multiplayer | Two players can play together |
| Online Rooms | Players can create and join rooms |
| Room Code | A room can be joined using a unique code |
| Real-Time Game | Moves are updated for both players |
| Server Validation | The server checks whether moves are valid |
| Turn System | Players take turns automatically |
| Win Detection | Checks rows, columns and diagonals |
| Draw Detection | Detects when the board is full |
| Rematch | Both players can request a rematch |
| Connection Status | Shows when a player connects or leaves |
| Winning Highlight | Shows the winning cells |

## Technologies Used

| Technology | Used For |
|---|---|
| C++ | Server and game logic |
| Crow | C++ web server |
| WebSocket | Real-time communication |
| HTML | Frontend structure |
| CSS | Frontend design |
| JavaScript | Frontend interaction |
| GitHub | Project hosting |

## How the Game Works

The game has two main parts:

```text
Frontend
   |
   | WebSocket
   v
C++ Server
   |
   v
Game Logic
```

The frontend handles what the player sees and sends actions to the server.

The C++ server handles the actual game, such as turns, moves, winning, drawing and rematches.

## Project Structure

```text
TicTacToe/
│
├── server/
│   ├── main.cpp
│   ├── Game.h
│   ├── Game.cpp
│   ├── Player.h
│   ├── Player.cpp
│   ├── Room.h
│   └── Room.cpp
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   ├── script.js
│   └── background.png
│
└── README.md
```

## Main Classes

### Player

The `Player` class stores basic information about a player.

| Data | Purpose |
|---|---|
| Player ID | Identifies the player |
| Name | Stores the player's name |
| Symbol | Stores X or O |
| Connection | Stores whether the player is connected |
| Rematch Status | Stores whether the player wants a rematch |

### Game

The `Game` class handles the actual Tic-Tac-Toe game.

| Function / Data | Purpose |
|---|---|
| Board | Stores the 3 × 3 board |
| Current Turn | Stores whose turn it is |
| `makeMove()` | Places X or O |
| `checkWin()` | Checks for a winner |
| `checkDraw()` | Checks for a draw |
| `reset()` | Starts a new game |

### Room

The `Room` class keeps two players together.

| Data | Purpose |
|---|---|
| Room Code | Identifies the room |
| Player 1 | First player |
| Player 2 | Second player |
| Game | Stores the current game |

## Algorithm

### 1. Start the Server

```text
1. Start the C++ program.
2. Start the Crow server.
3. Set up WebSocket communication.
4. Wait for players to connect.
```

### 2. Create a Room

```text
1. Player chooses Create Room.
2. Generate a unique room code.
3. Create a new room.
4. Add the player to the room.
5. Give the player X.
6. Send the room code to the player.
7. Wait for another player.
```

### 3. Join a Room

```text
1. Player enters a room code.
2. Search for the room.
3. Check if the room exists.
4. Check if the room already has two players.
5. If the room is full, show an error.
6. Otherwise add the player.
7. Give the player O.
8. Start the game.
```

### 4. Make a Move

```text
1. Player selects a square.
2. Send the selected row and column to the server.
3. Server checks whose turn it is.
4. Server checks whether the square is empty.
5. If the move is invalid, reject it.
6. If the move is valid, place X or O.
7. Check for a win.
8. If there is no win, check for a draw.
9. If the game continues, change the turn.
10. Send the updated board to both players.
```

### 5. Check for a Win

```text
1. Check the three rows.
2. Check the three columns.
3. Check the two diagonals.
4. If three matching symbols are found, declare the winner.
5. Otherwise continue the game.
```

### 6. Check for a Draw

```text
1. Check every square.
2. If an empty square exists, continue the game.
3. If all squares are filled and nobody has won, declare a draw.
```

## Rematch System

After the game ends, a **REMATCH** button appears at the bottom of the game.

A new game does not start when only one player clicks it.

**Both players must click REMATCH.**

| Player 1 | Player 2 | Result |
|---|---|---|
| Waiting | Waiting | Game stays finished |
| Ready | Waiting | Wait for Player 2 |
| Waiting | Ready | Wait for Player 1 |
| Ready | Ready | New game starts |

### Rematch Algorithm

```text
1. Game ends.
2. Show the REMATCH button.
3. Both players start as not ready.
4. Player 1 clicks REMATCH.
5. Mark Player 1 as ready.
6. Player 2 clicks REMATCH.
7. Mark Player 2 as ready.
8. Check both players.
9. If both are ready:
      Reset the board.
      Reset the turn.
      Start the new game.
```

## Game Flow

```text
Start
  |
  v
Create / Join Room
  |
  v
Two Players Connected
  |
  v
Start Game
  |
  v
Player Makes Move
  |
  v
Check Move
  |
  +---- Invalid ----> Wait for another move
  |
 Valid
  |
  v
Update Board
  |
  v
Check Win
  |
  +---- Win -------> Game Over
  |
 No Win
  |
  v
Check Draw
  |
  +---- Draw ------> Game Over
  |
 No Draw
  |
  v
Change Turn
  |
  v
Continue Game
  |
  v
Both Players Request Rematch
  |
  v
Reset Board
  |
  v
Start Next Game
```

## Server Validation

The server checks every move instead of trusting the frontend.

It checks:

- Is the player in the room?
- Is the game still running?
- Is it the player's turn?
- Is the selected square empty?
- Is the move valid?

This prevents a player from simply changing the board using the browser.

## Running the Project

Clone the repository:

```bash
git clone <repository-url>
cd TicTacToe
```

Build the C++ server using the project's build setup.

Start the server:

```bash
./server
```

Then open the frontend and connect it to the server.

## Testing With Two Devices

For testing on the same network:

1. Start the C++ server.
2. Find the computer's local IP address.
3. Connect both devices to the same network.
4. Open the frontend on both devices.
5. Player 1 creates a room.
6. Player 2 enters the room code.
7. Start playing.

The main purpose of this project is to learn how a **C++ server and a web frontend can work together to create a real-time multiplayer game**.
