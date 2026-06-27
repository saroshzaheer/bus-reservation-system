# Bus Reservation System

A full-stack bus reservation application that combines a Python Flask API layer with a C++ backend engine. The Flask app serves the frontend and exposes REST endpoints, while the C++ executable handles persistent data operations using text files.

## Project structure

- `app.py` — Flask application and API bridge.
- `main.cpp` — C++ backend engine for users, buses, trips, bookings, refunds, and admin operations.
- `1.html` — single-page frontend served by Flask.
- `output/` — data storage files for users, buses, routes, trips, bookings, cities, and refunds.
- `tests/test_backend.py` — backend smoke test validating compile and signup flow.

## Features

- User signup, login, password reset, and recovery
- Admin dashboard for buses, routes, cities, trips, user management, and statistics
- Search buses, view seats, book seats, and request refunds
- Loyalty tracking and booking/refund history
- Backend logic in C++ with Flask acting as the web layer

## Setup

1. Open a terminal in the project directory.
2. Create and activate a Python virtual environment:
   ```bash
   python3 -m venv .venv
   source .venv/bin/activate
   ```
3. Install Python dependencies:
   ```bash
   pip install flask flask-cors
   ```
4. Compile the C++ backend:
   ```bash
   g++ -std=c++17 -O2 -o output/main.exe main.cpp
   ```
5. Start the Flask server:
   ```bash
   python app.py
   ```
6. Open the web app:
   ```text
   http://127.0.0.1:5000
   ```

## Testing

Run the backend smoke test:

```bash
python -m unittest -v tests.test_backend
```

## Notes

- The backend binary must be present at `output/main.exe` for the Flask API to function correctly.
- Data is stored in text files inside the `output/` folder.
- Existing UI behavior is preserved while improving backend reliability.

## GitHub

Remote repository:

- `https://github.com/saroshzaheer/bus-reservation-system`
