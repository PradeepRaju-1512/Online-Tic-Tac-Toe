document.addEventListener('DOMContentLoaded', () => {
    const token = localStorage.getItem('sessionToken');
    const username = localStorage.getItem('username');

    // Kick unauthenticated users back to the login screen
    if (!token) {
        window.location.href = 'index.html';
        return;
    }

    document.getElementById('welcome-message').textContent = `Welcome, ${username}`;

    const findMatchBtn = document.getElementById('find-match-btn');
    const statusText = document.getElementById('lobby-status');

    findMatchBtn.addEventListener('click', () => {
        findMatchBtn.disabled = true;
        statusText.textContent = "Connecting to matchmaking server...";

        // Establish the WebSocket connection
        // We pass the token in the URL for the C++ server to validate
        const ws = new WebSocket(`ws://localhost:9000/game?token=${token}`);

        ws.onopen = () => {
            statusText.textContent = "Waiting for an opponent...";
            // Tell the server we are looking for a game
            ws.send(JSON.stringify({ action: "find_match" }));
        };

        ws.onmessage = (event) => {
            const msg = JSON.parse(event.data);
            
            // The C++ server will send this when two players are paired
            if (msg.type === "match_found") {
                statusText.textContent = "Match found! Joining game...";
                
                // Store the assigned game ID and transition to the game screen
                localStorage.setItem('gameId', msg.gameId);
                localStorage.setItem('symbol', msg.symbol); // 'X' or 'O'
                
                // Small delay for UI smoothness, then redirect
                setTimeout(() => {
                    window.location.href = 'game.html';
                }, 1000);
            }
        };

        ws.onerror = (error) => {
            console.error('WebSocket Error:', error);
            statusText.textContent = "Connection error. Try again.";
            findMatchBtn.disabled = false;
        };
    });

    document.getElementById('logout-btn').addEventListener('click', () => {
        localStorage.clear();
        window.location.href = 'index.html';
    });
});
