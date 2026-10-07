document.addEventListener('DOMContentLoaded', () => {
    const token = localStorage.getItem('sessionToken');
    const symbol = localStorage.getItem('symbol');
    
    if (!token) {
        window.location.href = 'index.html';
        return;
    }

    const statusDisplay = document.getElementById('game-status');
    const cells = document.querySelectorAll('.cell');
    let isMyTurn = false;
    let gameActive = true;

    // Connect to the WebSocket, specifically asking to rejoin the active game session
    const ws = new WebSocket(`ws://localhost:9000/game?token=${token}`);

    ws.onopen = () => {
        statusDisplay.textContent = `You are playing as ${symbol}. Waiting for sync...`;
    };

    ws.onmessage = (event) => {
        const msg = JSON.parse(event.data);

        // State update sent by the C++ GameSession after every valid move
        if (msg.type === "state_update") {
            updateBoardUI(msg.board); // msg.board should be a 9-char string like "X O X    "
            
            isMyTurn = (msg.currentTurn === symbol);
            
            if (msg.gameState === "IN_PROGRESS") {
                statusDisplay.textContent = isMyTurn ? "Your turn!" : "Opponent's turn...";
            } else if (msg.gameState === "GAME_OVER") {
                gameActive = false;
                if (msg.winner === 'D') {
                    statusDisplay.textContent = "It's a draw!";
                } else if (msg.winner === symbol) {
                    statusDisplay.textContent = "You win!";
                } else {
                    statusDisplay.textContent = "You lose!";
                }
            }
        }
    };

    // Attach click listeners to the HTML grid
    cells.forEach(cell => {
        cell.addEventListener('click', () => {
            const index = cell.getAttribute('data-index');

            // Prevent clicking if it's not our turn, game is over, or cell is taken
            if (!isMyTurn || !gameActive || cell.classList.contains('taken')) {
                return;
            }

            // Optimistically update the UI locally for instant feedback
            cell.textContent = symbol;
            cell.classList.add('taken', symbol.toLowerCase());
            isMyTurn = false;
            statusDisplay.textContent = "Sending move...";

            // Fire the move to the C++ server to validate and broadcast
            ws.send(JSON.stringify({
                action: "move",
                cell: parseInt(index)
            }));
        });
    });

    // Helper to sync the DOM with the C++ backend string representation
    function updateBoardUI(boardString) {
        for (let i = 0; i < 9; i++) {
            const char = boardString[i];
            const cell = cells[i];
            
            if (char === 'X' || char === 'O') {
                cell.textContent = char;
                cell.classList.add('taken', char.toLowerCase());
            } else {
                cell.textContent = '';
                cell.className = 'cell'; // Reset classes
            }
        }
    }

    document.getElementById('leave-btn').addEventListener('click', () => {
        ws.close();
        window.location.href = 'lobby.html';
    });
});
