// Base URL matching the C++ Crow server port
const API_URL = 'http://localhost:9000';

// Attach event listener for the Login form if it exists on the current page
const loginForm = document.getElementById('login-form');
if (loginForm) {
    loginForm.addEventListener('submit', async (e) => {
        e.preventDefault(); // Prevent standard page reload
        
        const user = document.getElementById('username').value;
        const pass = document.getElementById('password').value;
        const errorDisplay = document.getElementById('error-message');

        try {
            const response = await fetch(`${API_URL}/login`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ username: user, password: pass })
            });

            if (response.ok) {
                const data = await response.json();
                // Store the auth token locally so the WebSocket can use it later
                localStorage.setItem('sessionToken', data.token);
                localStorage.setItem('username', data.username);
                
                // Transition to the lobby area
                window.location.href = 'lobby.html';
            } else {
                errorDisplay.textContent = 'Invalid username or password.';
                errorDisplay.style.display = 'block';
            }
        } catch (error) {
            console.error('Network Error:', error);
            errorDisplay.textContent = 'Failed to connect to the server.';
            errorDisplay.style.display = 'block';
        }
    });
}

// Attach event listener for the Register form if it exists on the current page
const registerForm = document.getElementById('register-form');
if (registerForm) {
    registerForm.addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const user = document.getElementById('username').value;
        const pass = document.getElementById('password').value;
        const errorDisplay = document.getElementById('error-message');

        try {
            const response = await fetch(`${API_URL}/register`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ username: user, password: pass })
            });

            if (response.ok) {
                // Immediately log the user in after successful registration
                alert('Account created successfully! Please login.');
                window.location.href = 'index.html';
            } else if (response.status === 409) {
                errorDisplay.textContent = 'Username already exists.';
                errorDisplay.style.display = 'block';
            } else {
                errorDisplay.textContent = 'Registration failed. Try again.';
                errorDisplay.style.display = 'block';
            }
        } catch (error) {
            console.error('Network Error:', error);
            errorDisplay.textContent = 'Failed to connect to the server.';
            errorDisplay.style.display = 'block';
        }
    });
}
