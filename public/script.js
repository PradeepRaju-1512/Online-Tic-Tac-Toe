let currentTheme = 'purple';
let localPlayerName = 'Player_One';
let activeMode = 'bot';
let roomID = null;
let gameState = ['', '', '', '', '', '', '', '', ''];
let currentPlayerSymbol = 'X';
let mySymbol = 'X';
let netMySymbol = 'X';
let gameActive = true;

let p1Score = 0;
let p2Score = 0;

function showToast(message, type = 'info') {
  const container = document.getElementById('toastContainer');
  if (!container) return;
  const toast = document.createElement('div');
  toast.className = `toast ${type} rgb-animate`;
  let icon = type === 'success' ? '✅' : type === 'error' ? '⚠️' : 'ℹ️';
  toast.innerHTML = `<span class="toast-icon">${icon}</span><span>${message}</span>`;
  container.appendChild(toast);
  setTimeout(() => toast.classList.add('show'), 50);
  setTimeout(() => {
    toast.classList.remove('show');
    setTimeout(() => toast.remove(), 300);
  }, 3500);
}

function changeTheme(theme) {
  currentTheme = theme;
  document.body.setAttribute('data-theme', theme);
  showToast(`Theme changed to ${theme.toUpperCase()}`, 'info');
}

function updateUsername(name) {
  localPlayerName = name || 'Player_One';
  const p1NameElem = document.getElementById('p1Name');
  if (p1NameElem) p1NameElem.innerText = localPlayerName;
  showToast(`Username updated to ${localPlayerName}`, 'success');
}

function openModal(id) { document.getElementById(id).classList.add('active'); }
function closeModal(id) { document.getElementById(id).classList.remove('active'); }

function startBotGame() {
  closeModal('botModal');
  activeMode = 'bot';
  netMySymbol = 'X';
  mySymbol = 'X';
  p1Score = 0;
  p2Score = 0;
  updateAvatarsAndRoles();
  resetBoard();
  showToast('Match started vs Bot!', 'success');
}

function createRoom() {
  const input = document.getElementById('customRoomInput');
  roomID = input.value.trim() || 'ROOM_' + Math.floor(100 + Math.random() * 900);
  activeMode = 'lan';
  netMySymbol = 'X';
  mySymbol = 'X';
  p1Score = 0;
  p2Score = 0;
  updateAvatarsAndRoles();
  resetBoard();
  updateRoomListStatus(roomID, true);
  showToast(`Successfully created room: ${roomID} (Hosting as X)`, 'success');
}

function joinRoom(id) {
  roomID = id;
  activeMode = 'lan';
  netMySymbol = 'O';
  mySymbol = 'O';
  p1Score = 0;
  p2Score = 0;
  updateAvatarsAndRoles();
  resetBoard();
  updateRoomListStatus(roomID, true);
  showToast(`Successfully joined room: ${roomID} (Playing as O)`, 'success');
}

function exitRoom() {
  if (roomID) {
    updateRoomListStatus(roomID, false);
  }
  roomID = null;
  activeMode = 'bot';
  p1Score = 0;
  p2Score = 0;
  netMySymbol = 'X';
  mySymbol = 'X';
  updateAvatarsAndRoles();
  resetBoard();
  showToast('Exited room. Returned to main dashboard.', 'info');
}

function updateRoomListStatus(id, isInMatch) {
  const statusElem = document.getElementById(`roomStatus-${id}`);
  const btnElem = document.getElementById(`roomBtn-${id}`);
  if (statusElem && btnElem) {
    if (isInMatch) {
      statusElem.innerText = "Status: Full";
      statusElem.style.color = "var(--status-offline)";
      btnElem.innerText = "Full";
      btnElem.disabled = true;
      btnElem.style.opacity = "0.5";
      btnElem.style.cursor = "not-allowed";
    } else {
      statusElem.innerText = "Status: Available";
      statusElem.style.color = "var(--text-muted)";
      btnElem.innerText = "Join";
      btnElem.disabled = false;
      btnElem.style.opacity = "1";
      btnElem.style.cursor = "pointer";
    }
  }
}

function updateAvatarsAndRoles() {
  const opponentSym = mySymbol === 'X' ? 'O' : 'X';

  const p1Avatar = document.getElementById('p1Avatar');
  const p2Avatar = document.getElementById('p2Avatar');
  if (p1Avatar) {
    p1Avatar.innerText = mySymbol;
    p1Avatar.className = `player-avatar ${mySymbol.toLowerCase()}`;
  }
  if (p2Avatar) {
    p2Avatar.innerText = opponentSym;
    p2Avatar.className = `player-avatar ${opponentSym.toLowerCase()}`;
  }

  const p1Role = document.getElementById('p1Role');
  const p2Role = document.getElementById('p2Role');
  if (p1Role) {
    p1Role.innerHTML = `You (Token ${mySymbol}) | Wins: <strong id="p1Score" style="color: var(--${mySymbol.toLowerCase()}-color);">${p1Score}</strong>`;
  }
  if (p2Role) {
    p2Role.innerHTML = `Opponent (Token ${opponentSym}) | Wins: <strong id="p2Score" style="color: var(--${opponentSym.toLowerCase()}-color);">${p2Score}</strong>`;
  }
}

function handleCellClick(index) {
  if (!gameActive || gameState[index] !== '') return;
  if (currentPlayerSymbol !== mySymbol) return;
  makeMove(index, currentPlayerSymbol);
}

function makeMove(index, symbol) {
  if (!gameActive || gameState[index] !== '') return false;

  gameState[index] = symbol;
  const cell = document.getElementById(`cell-${index}`);
  cell.innerText = symbol;
  cell.className = `board-cell ${symbol.toLowerCase()}`;

  if (checkWin(symbol)) {
    gameActive = false;
    if (symbol === mySymbol) {
      p1Score++;
    } else {
      p2Score++;
    }
    updateAvatarsAndRoles();

    const winMsg = symbol === mySymbol ? 'YOU WIN THE MATCH!' : `${symbol} WINS THE MATCH!`;
    document.getElementById('turnIndicator').innerText = winMsg;
    showToast(winMsg, 'success');
    logToDbTerminal(`Match ended: ${symbol} won.`);
    return true;
  }

  if (gameState.every(cell => cell !== '')) {
    gameActive = false;
    document.getElementById('turnIndicator').innerText = "IT'S A DRAW!";
    showToast("Match ended in a draw.", 'info');
    logToDbTerminal("Match ended in a draw.");
    return true;
  }

  currentPlayerSymbol = currentPlayerSymbol === 'X' ? 'O' : 'X';
  document.getElementById('turnIndicator').innerText = currentPlayerSymbol === mySymbol ? 'YOUR TURN' : `${currentPlayerSymbol}'S TURN`;
  
  if (activeMode === 'bot' && gameActive && currentPlayerSymbol !== mySymbol) {
    setTimeout(botMove, 400);
  }
  
  return true;
}

function botMove() {
  if (!gameActive || currentPlayerSymbol === mySymbol) return;

  const difficultyElem = document.getElementById('aiDifficulty');
  const difficulty = difficultyElem ? difficultyElem.value : 'unbeatable';
  let moveIdx = -1;

  if (difficulty === 'unbeatable') {
    moveIdx = getBestMove(currentPlayerSymbol);
  } else if (difficulty === 'medium') {
    moveIdx = findWinningOrBlockingMove(currentPlayerSymbol);
    if (moveIdx === -1) moveIdx = findWinningOrBlockingMove(currentPlayerSymbol === 'X' ? 'O' : 'X');
    if (moveIdx === -1) {
      const empty = gameState.map((v, i) => v === '' ? i : null).filter(v => v !== null);
      moveIdx = empty[Math.floor(Math.random() * empty.length)];
    }
  } else {
    const empty = gameState.map((v, i) => v === '' ? i : null).filter(v => v !== null);
    moveIdx = empty[Math.floor(Math.random() * empty.length)];
  }

  if (moveIdx !== -1) {
    makeMove(moveIdx, currentPlayerSymbol);
  }
}

function findWinningOrBlockingMove(symbol) {
  const wins = [
    [0,1,2], [3,4,5], [6,7,8],
    [0,3,6], [1,4,7], [2,5,8],
    [0,4,8], [2,4,6]
  ];
  for (let combo of wins) {
    const [a, b, c] = combo;
    let vals = [gameState[a], gameState[b], gameState[c]];
    if (vals.filter(v => v === symbol).length === 2 && vals.includes('')) {
      if (gameState[a] === '') return a;
      if (gameState[b] === '') return b;
      if (gameState[c] === '') return c;
    }
  }
  return -1;
}

function getBestMove(botSym) {
  const humanSym = botSym === 'X' ? 'O' : 'X';
  let bestScore = -Infinity;
  let bestMove = -1;
  for (let i = 0; i < 9; i++) {
    if (gameState[i] === '') {
      gameState[i] = botSym;
      let score = minimax(gameState, 0, false, botSym, humanSym);
      gameState[i] = '';
      if (score > bestScore) {
        bestScore = score;
        bestMove = i;
      }
    }
  }
  return bestMove;
}

function minimax(board, depth, isMaximizing, botSym, humanSym) {
  if (checkWinState(board, botSym)) return 10 - depth;
  if (checkWinState(board, humanSym)) return depth - 10;
  if (board.every(cell => cell !== '')) return 0;

  if (isMaximizing) {
    let maxScore = -Infinity;
    for (let i = 0; i < 9; i++) {
      if (board[i] === '') {
        board[i] = botSym;
        let score = minimax(board, depth + 1, false, botSym, humanSym);
        board[i] = '';
        maxScore = Math.max(score, maxScore);
      }
    }
    return maxScore;
  } else {
    let minScore = Infinity;
    for (let i = 0; i < 9; i++) {
      if (board[i] === '') {
        board[i] = humanSym;
        let score = minimax(board, depth + 1, true, botSym, humanSym);
        board[i] = '';
        minScore = Math.min(score, minScore);
      }
    }
    return minScore;
  }
}

function checkWin(symbol) {
  return checkWinState(gameState, symbol);
}

function checkWinState(board, symbol) {
  const wins = [
    [0,1,2], [3,4,5], [6,7,8],
    [0,3,6], [1,4,7], [2,5,8],
    [0,4,8], [2,4,6]
  ];
  return wins.some(combo => combo.every(idx => board[idx] === symbol));
}

function requestRematch() {
  if (activeMode === 'bot') {
    resetBoard();
    showToast('Rematch started!', 'info');
  } else {
    const modalText = document.getElementById('rematchModalText');
    const btnContainer = document.getElementById('rematchButtons');
    
    modalText.innerText = "Rematch requested! Waiting for opponent agreement...";
    btnContainer.innerHTML = `<button class="btn btn-secondary" onclick="closeModal('rematchModal')">Cancel Request</button>`;
    openModal('rematchModal');

    setTimeout(() => {
      modalText.innerText = "Opponent has agreed to a rematch!";
      btnContainer.innerHTML = `<button class="btn" onclick="acceptRematch()" style="background: var(--status-online);">Start Rematch</button>`;
    }, 2000);
  }
}

function acceptRematch() {
  closeModal('rematchModal');
  resetBoard();
  showToast('Rematch accepted! New round started.', 'success');
}

function declineRematch() {
  closeModal('rematchModal');
  exitRoom();
}

function swapSymbols() {
  netMySymbol = netMySymbol === 'X' ? 'O' : 'X';
  mySymbol = netMySymbol;
  updateAvatarsAndRoles();
  resetBoard();
  showToast(`Symbols swapped! You are now playing as ${mySymbol}`, 'success');

  if (activeMode === 'bot' && currentPlayerSymbol !== mySymbol) {
    setTimeout(botMove, 400);
  }
}

function resetBoard() {
  gameState = ['', '', '', '', '', '', '', '', ''];
  currentPlayerSymbol = 'X';
  gameActive = true;
  document.getElementById('turnIndicator').innerText = currentPlayerSymbol === mySymbol ? 'YOUR TURN' : `${currentPlayerSymbol}'S TURN`;
  
  for (let i = 0; i < 9; i++) {
    const cell = document.getElementById(`cell-${i}`);
    if (cell) {
      cell.innerText = '';
      cell.className = 'board-cell';
    }
  }

  if (activeMode === 'bot' && currentPlayerSymbol !== mySymbol) {
    setTimeout(botMove, 400);
  }
}

function logToDbTerminal(msg) {
  const term = document.getElementById('dbTerminal');
  if (term) {
    const time = new Date().toLocaleTimeString();
    term.innerHTML += `<div>[${time}] ${msg}</div>`;
    term.scrollTop = term.scrollHeight;
  }
}

function refreshDBLogs() {
  fetch('/api/db/records')
    .then(res => res.json())
    .then(data => {
      const term = document.getElementById('dbTerminal');
      if (term) {
        term.innerHTML += `<div>[Fetched DB Records]: ${JSON.stringify(data)}</div>`;
        term.scrollTop = term.scrollHeight;
      }
      showToast('PostgreSQL records fetched successfully', 'success');
    })
    .catch(() => showToast('Failed to fetch DB records', 'error'));
}

// Live Health Checks (Server & Database)
setInterval(() => {
  fetch('/api/status')
    .then(res => res.json())
    .then(() => {
      const dot = document.getElementById('serverDot');
      const text = document.getElementById('serverStatusText');
      if (dot) dot.className = 'status-dot online';
      if (text) text.innerText = 'Server Online';
    })
    .catch(() => {
      const dot = document.getElementById('serverDot');
      const text = document.getElementById('serverStatusText');
      if (dot) dot.className = 'status-dot';
      if (text) text.innerText = 'Server Offline';
    });

  fetch('/api/db/records')
    .then(res => {
      if (!res.ok) throw new Error();
      return res.json();
    })
    .then(() => {
      const dbDot = document.getElementById('dbDot');
      const dbText = document.getElementById('dbStatusText');
      if (dbDot) dbDot.className = 'status-dot online';
      if (dbText) dbText.innerText = 'Database Online';
    })
    .catch(() => {
      const dbDot = document.getElementById('dbDot');
      const dbText = document.getElementById('dbStatusText');
      if (dbDot) dbDot.className = 'status-dot';
      if (dbText) dbText.innerText = 'Database Offline';
    });
}, 5000);