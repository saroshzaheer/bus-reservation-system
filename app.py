from __future__ import annotations
import json
import subprocess
from pathlib import Path
from flask import Flask, jsonify, request, send_file
from flask_cors import CORS
app = Flask(__name__, static_folder=None)
CORS(app)
BASE_DIR = Path(__file__).resolve().parent

@app.errorhandler(400)
def handle_bad_request(error):
    message = "Invalid request payload"
    if request.path.startswith("/api/"):
        return jsonify({"success": False, "message": message}), 400
    return jsonify({"error": message}), 400

@app.errorhandler(404)
def handle_not_found(error):
    if request.path.startswith("/api/"):
        return jsonify({"success": False, "message": "Endpoint not found"}), 404
    return jsonify({"error": "Not found"}), 404

@app.errorhandler(500)
def handle_server_error(error):
    if request.path.startswith("/api/"):
        return jsonify({"success": False, "message": "Internal server error"}), 500
    return jsonify({"error": "Internal server error"}), 500
DATA_DIR = BASE_DIR / "output"
DATA_DIR.mkdir(exist_ok=True)
MAX_RETRIES = 3
CPP_BIN = BASE_DIR / "output/main.exe"
def _run_cpp(action: str, *args: str):
    """Run the compiled C++ backend as a subprocess for the given action.
    The C++ binary reads/writes the same text files located in DATA_DIR.
    We run from BASE_DIR so main.cpp (with DATA_DIR="output/") finds them correctly.
    """
    if not CPP_BIN.exists():
        return {"success": False, "message": "Backend binary not found. Please compile main.cpp to backend.exe."}, 500
    cmd = [str(CPP_BIN), action, *args]
    try:
        result = subprocess.run(cmd, cwd=str(BASE_DIR), capture_output=True, text=True, timeout=10)
    except Exception as exc:
        return {"success": False, "message": f"Backend error: {exc}"}, 500
    stdout = (result.stdout or "").strip()
    stderr = (result.stderr or "").strip()
    if not stdout and not stderr:
        return {"success": False, "message": "Empty response from backend."}, 500
    if stdout:
        try:
            payload = json.loads(stdout)
        except json.JSONDecodeError:
            return {"success": False, "message": f"Invalid backend response: {stdout}"}, 500
        status = 200 if payload.get("success") else 400
        return payload, status
    return {"success": False, "message": stderr or "Backend execution failed"}, 500
@app.route("/")
def index():
    """Serve the frontend HTML file."""
    html_file = BASE_DIR / "1.html"
    if html_file.exists():
        return send_file(str(html_file))
    return jsonify({"error": "Frontend not found"}), 404
@app.get("/api/health")
def health():
    """Health check endpoint."""
    return jsonify({"status": "ok"})
@app.route('/favicon.jpg')
def favicon():
    return send_file(BASE_DIR / 'favicon.jpg')
@app.post("/api/signup")
def signup():
    """User signup - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "password": string,
        "confirmPassword": string,
        "phone": string,
        "name": string,
        "email": string,
        "motherName": string,
        "birthCity": string
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    password = payload.get("password") or ""
    confirm_password = payload.get("confirmPassword") or ""
    phone = (payload.get("phone") or "").strip()
    name = (payload.get("name") or "").strip()
    email = (payload.get("email") or "").strip()
    mother_name = (payload.get("motherName") or "").strip()
    birth_city = (payload.get("birthCity") or "").strip()
    cpp_payload, status = _run_cpp(
        "signup",
        username,
        password,
        confirm_password,
        phone,
        name,
        email,
        mother_name,
        birth_city,
    )
    if not cpp_payload.get("success"):
        return jsonify(cpp_payload), status
    return jsonify({
        "success": True,
        "user": {
            "username": username,
            "name": name,
            "phone": phone,
            "email": email,
            "isAdmin": False
        }
    })
@app.post("/api/check-username")
def check_username():
    """Check if username already exists - delegates to C++ backend.
    Expected payload:
    {
        "username": string
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    if not username:
        return jsonify({"exists": False})
    cpp_payload, status = _run_cpp("check-username", username)
    if cpp_payload.get("success"):
        return jsonify({"exists": cpp_payload.get("exists", False)})
    return jsonify({"exists": False})
@app.post("/api/check-phone")
def check_phone():
    """Check if phone already exists - delegates to C++ backend.
    Expected payload:
    {
        "phone": string
    }
    """
    payload = request.get_json(force=True) or {}
    phone = (payload.get("phone") or "").strip()
    if not phone:
        return jsonify({"exists": False})
    cpp_payload, status = _run_cpp("check-phone", phone)
    if cpp_payload.get("success"):
        return jsonify({"exists": cpp_payload.get("exists", False)})
    return jsonify({"exists": False})
@app.post("/api/check-email")
def check_email():
    """Check if email already exists - delegates to C++ backend.
    Expected payload:
    {
        "email": string
    }
    """
    payload = request.get_json(force=True) or {}
    email = (payload.get("email") or "").strip()
    if not email:
        return jsonify({"exists": False})
    cpp_payload, status = _run_cpp("check-email", email)
    if cpp_payload.get("success"):
        return jsonify({"exists": cpp_payload.get("exists", False)})
    return jsonify({"exists": False})
@app.post("/api/login")
def login():
    """User login - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "password": string,
        "loginType": string (optional, "admin" or "user")
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    password = payload.get("password") or ""
    login_type = (payload.get("loginType") or "user").strip().lower()
    cpp_payload, status = _run_cpp("login", username, password, login_type)
    if not cpp_payload.get("success"):
        return jsonify(cpp_payload), 401 if status == 200 else status
    user_data = cpp_payload.get("user", {})
    return jsonify({
        "success": True,
        "user": {
            "username": user_data.get("username"),
            "name": user_data.get("name", ""),
            "phone": user_data.get("phone", ""),
            "email": user_data.get("email", ""),
            "isAdmin": bool(user_data.get("isAdmin", False))
        }
    })
@app.post("/api/reset-password")
def reset_password():
    """Reset password endpoint - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "password": string
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    new_password = payload.get("password") or ""
    cpp_payload, status = _run_cpp("reset-password", username, new_password)
    return jsonify(cpp_payload), status
@app.post("/api/recover-username")
def recover_username():
    """Recover username using phone and security answers - delegates to C++ backend.
    Expected payload:
    {
        "phone": string,
        "motherName": string,
        "birthCity": string
    }
    """
    payload = request.get_json(force=True) or {}
    phone = (payload.get("phone") or "").strip()
    mother_name = (payload.get("motherName") or "").strip()
    birth_city = (payload.get("birthCity") or "").strip()
    cpp_payload, status = _run_cpp("recover-username", phone, mother_name, birth_city)
    return jsonify(cpp_payload), status
@app.post("/api/recover-password")
def recover_password():
    """Recover password endpoint - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("recover-password")
    return jsonify(cpp_payload), status
@app.post("/api/verify-security-answers")
def verify_security_answers():
    """Verify security answers for password recovery - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "motherName": string,
        "birthCity": string
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    mother_name = (payload.get("motherName") or "").strip()
    birth_city = (payload.get("birthCity") or "").strip()
    cpp_payload, status = _run_cpp("verify-security", username, mother_name, birth_city)
    return jsonify(cpp_payload), status
@app.post("/api/reset-password-with-username")
def reset_password_with_username():
    """Reset password using username - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "newPassword": string
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    new_password = payload.get("newPassword") or ""
    cpp_payload, status = _run_cpp("reset-password-username", username, new_password)
    return jsonify(cpp_payload), status
@app.get("/api/cities")
def cities():
    """Get list of available cities - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("get-cities")
    if cpp_payload.get("success"):
        return jsonify({
            "success": True,
            "cities": cpp_payload.get("cities", [])
        }), status
    return jsonify({"success": False, "cities": []}), status
@app.post("/api/search-buses")
def search_buses():
    """Search buses by source, destination, and date - delegates to C++ backend.
    Expected payload:
    {
        "source": string,
        "destination": string,
        "date": string  (YYYY-MM-DD)
    }
    """
    payload = request.get_json(force=True) or {}
    source = (payload.get("source") or "").strip()
    destination = (payload.get("destination") or "").strip()
    date = (payload.get("date") or "").strip()
    if not source or not destination or not date:
         return jsonify({"success": False, "message": "Missing source, destination, or date"}), 400
    cpp_payload, status = _run_cpp("search-buses", source, destination, date)
    return jsonify(cpp_payload), status
@app.get("/api/seats/<bus_id>")
def get_seats(bus_id: str):
    """Get seat availability for a bus - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("get-seats", bus_id)
    return jsonify(cpp_payload), status
@app.get("/api/loyalty/<username>")
def loyalty(username: str):
    """Get loyalty information for a user - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("get-loyalty", username)
    return jsonify(cpp_payload), status
@app.post("/api/my-bookings")
def my_bookings():
    """Return bookings for the current user."""
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    if not username:
        return jsonify({"success": False, "message": "Username required"}), 400
    cpp_payload, status = _run_cpp("user-bookings", username)
    return jsonify(cpp_payload), status
@app.post("/api/refund")
def refund():
    """Submit a refund request for a booking."""
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    booking_id = str(payload.get("bookingID") or "")
    account_number = (payload.get("accountNumber") or "").strip()
    account_type = (payload.get("accountType") or "").strip()
    if not all([username, booking_id, account_number, account_type]):
        return jsonify({"success": False, "message": "Missing refund details"}), 400
    cpp_payload, status = _run_cpp(
        "request-refund",
        username,
        booking_id,
        account_number,
        account_type,
    )
    return jsonify(cpp_payload), status
@app.post("/api/my-refunds")
def my_refunds():
    """Return refund history for the current user."""
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    if not username:
        return jsonify({"success": False, "message": "Username required"}), 400
    cpp_payload, status = _run_cpp("user-refunds", username)
    return jsonify(cpp_payload), status
@app.post("/api/update-credentials")
def update_credentials():
    """Update user credentials (name, phone, email, username)."""
    payload = request.get_json(force=True) or {}
    new_username = (payload.get("username") or "").strip() 
    current_username = (payload.get("currentUsername") or new_username).strip() 
    name = (payload.get("name") or "").strip()
    phone = (payload.get("phone") or "").strip()
    email = (payload.get("email") or "").strip()
    if not all([new_username, current_username, name, phone, email]):
        return jsonify({"success": False, "message": "Missing credentials"}), 400
    cpp_payload, status = _run_cpp(
        "update-credentials",
        current_username,
        new_username,
        name,
        phone,
        email,
    )
    return jsonify(cpp_payload), status
@app.get("/api/admin/users")
def admin_get_users():
    """Get all users for admin."""
    cpp_payload, status = _run_cpp("admin-get-users")
    return jsonify(cpp_payload), status
@app.get("/api/admin/stats")
def admin_stats():
    """Get admin dashboard statistics."""
    cpp_payload, status = _run_cpp("admin-stats")
    return jsonify(cpp_payload), status
@app.post("/api/admin/users/toggle")
def admin_toggle_user():
    """Toggle user ban status (admin only) - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "ban": boolean
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    ban = payload.get("ban", False)
    if not username:
        return jsonify({"success": False, "message": "Missing username"}), 400
    cpp_payload, status = _run_cpp("admin-toggle-user", username, "1" if ban else "0")
    return jsonify(cpp_payload), status
@app.delete("/api/admin/users/<username>")
def admin_delete_user(username: str):
    """Delete a user (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-delete-user", username)
    return jsonify(cpp_payload), status
@app.post("/api/update-password")
def update_password():
    """Update user password."""
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    currentPassword = (payload.get("currentPassword") or "").strip()
    newPassword = (payload.get("newPassword") or "").strip()
    if not all([username, currentPassword, newPassword]):
        return jsonify({"success": False, "message": "Missing password arguments"}), 400
    cpp_payload, status = _run_cpp(
        "update-password",
        username,
        currentPassword,
        newPassword,
    )
    return jsonify(cpp_payload), status
@app.post("/api/book-seats")
def book_seats():
    """Book seats on a bus - delegates to C++ backend.
    Expected payload:
    {
        "username": string,
        "busID": string,
        "seats": [
            {
                "seatNumber": number,
                "gender": string
            }
        ],
        "paymentMethod": string,
        "tokensRedeemed": number
    }
    """
    payload = request.get_json(force=True) or {}
    username = (payload.get("username") or "").strip()
    bus_id = str(payload.get("busID") or "")
    print(f"DEBUG: Received booking request for Bus/Trip ID: {bus_id}", flush=True)
    if not bus_id:
        return jsonify({"success": False, "message": "Missing Bus/Trip ID"}), 400
    seats = payload.get("seats") or []
    payment_method = payload.get("paymentMethod", "Card")
    tokens_redeemed = int(payload.get("tokensRedeemed", 0))
    seats_str = ",".join(f"{s.get('seatNumber', '')}:{s.get('gender', 'M')}:{s.get('passengerName', '').replace(',', ' ')}" for s in seats)
    cpp_payload, status = _run_cpp(
        "book-seats",
        username,
        bus_id,
        seats_str,
        payment_method,
        str(tokens_redeemed)
    )
    return jsonify(cpp_payload), status
@app.get("/api/admin/bookings")
def admin_bookings():
    """Get all bookings (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-bookings")
    return jsonify(cpp_payload), status
@app.get("/api/admin/buses")
def admin_get_buses():
    """Get all buses (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-buses")
    return jsonify(cpp_payload), status
@app.post("/api/admin/buses")
def admin_add_bus():
    """Add a new bus (admin only) - delegates to C++ backend.
    Expected payload:
    {
        "bus": {
            "busNumber": string,
            "source": string,
            "destination": string,
            "seats": number,
            "time": string,
            "fare": number
        }
    }
    """
    payload = request.get_json(force=True) or {}
    if "bus" in payload and isinstance(payload["bus"], dict):
        bus_data = payload["bus"]
        bus_number = (bus_data.get("busID") or "").strip()
        seats = str(bus_data.get("seats", "20"))
    else:
        bus_number = (payload.get("busNumber") or "").strip()
        seats = str(payload.get("seats", "20"))
    if not bus_number:
         return jsonify({"success": False, "message": "Missing bus number"}), 400
    cpp_payload, status = _run_cpp(
        "admin-add-bus",
        bus_number,
        seats
    )
    return jsonify(cpp_payload), status
@app.post("/api/admin/add-bus")
def admin_add_bus_alias():
    """Compatibility alias for the frontend's admin bus creation endpoint."""
    return admin_add_bus()

@app.delete("/api/admin/buses/<bus_id>")
def admin_delete_bus(bus_id: str):
    """Delete a bus (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-delete-bus", bus_id)
    return jsonify(cpp_payload), status
@app.get("/api/routes")
def get_routes():
    """Get all routes - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("get-routes")
    return jsonify(cpp_payload), status
@app.post("/api/admin/routes")
def add_route():
    """Add a new route (admin only) - delegates to C++ backend.
    Expected payload:
    {
        "route": {
            "source": string,
            "destination": string,
            "distanceKM": number
        }
    }
    """
    payload = request.get_json(force=True) or {}
    route = payload.get("route") or {}
    source = route.get("source", "")
    destination = route.get("destination", "")
    distance = str(route.get("distanceKM", "0"))
    cpp_payload, status = _run_cpp("admin-add-route", source, destination, distance)
    return jsonify(cpp_payload), status
@app.delete("/api/admin/routes/<route_id>")
def delete_route(route_id: str):
    """Delete a route (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-delete-route", route_id)
    return jsonify(cpp_payload), status
@app.post("/api/admin/cities")
def add_city():
    """Add a new city (admin only) - delegates to C++ backend.
    Expected payload:
    {
        "id": string,
        "name": string
    }
    """
    payload = request.get_json(force=True) or {}
    city_id = str(payload.get("id") or "").strip()
    name = (payload.get("name") or "").strip()
    cpp_payload, status = _run_cpp("admin-add-city", city_id, name)
    return jsonify(cpp_payload), status
@app.delete("/api/admin/cities/<city_id>")
def delete_city(city_id: str):
    """Delete a city (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-delete-city", city_id)
    return jsonify(cpp_payload), status
@app.post("/api/admin/toggle-bus/<bus_id>")
def toggle_bus(bus_id: str):
    """Toggle bus active status (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-toggle-bus", bus_id)
    return jsonify(cpp_payload), status
@app.post("/api/loyalty")
def get_loyalty():
    """Fetch loyalty info for the given username (expects JSON: {"username": ...})"""
    payload = request.get_json(force=True)
    username = (payload.get("username") or "").strip()
    if not username:
        return jsonify({"success": False, "message": "Missing username"}), 400
    cpp_payload, status = _run_cpp("get-loyalty", username)
    return jsonify(cpp_payload), status
@app.post("/api/my-bookings")
def get_my_bookings():
    """Get bookings for a user - delegates to C++ backend."""
    payload = request.get_json(force=True)
    username = (payload.get("username") or "").strip()
    if not username:
        return jsonify({"success": False, "message": "Missing username"}), 400
    cpp_payload, status = _run_cpp("get-my-bookings", username)
    return jsonify(cpp_payload), status
@app.post("/api/my-refunds")
def get_my_refunds():
    """Get refunds for a user - delegates to C++ backend."""
    payload = request.get_json(force=True)
    username = (payload.get("username") or "").strip()
    if not username:
        return jsonify({"success": False, "message": "Missing username"}), 400
    cpp_payload, status = _run_cpp("get-my-refunds", username)
    return jsonify(cpp_payload), status
@app.get("/api/admin/trips")
def admin_get_trips():
    """Get all trips (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-trips")
    return jsonify(cpp_payload), status
@app.post("/api/admin/trips")
def admin_add_trip():
    """Add a new trip (admin only) - delegates to C++ backend.
    Expected payload:
    {
        "trip": {
            "tripID": number,
            "busNumber": string,
            "source": string,
            "destination": string,
            "date": string,
            "time": string,
            "seats": number,
            "fare": number
        }
    }
    """
    payload = request.get_json(force=True) or {}
    trip = payload.get("trip") or {}
    trip_id = str(trip.get("tripID", ""))
    bus_number = trip.get("busNumber", "")
    source = trip.get("source", "")
    destination = trip.get("destination", "")
    date = trip.get("date", "")
    time = trip.get("time", "")
    fare = str(trip.get("fare", "500"))
    cpp_payload, status = _run_cpp(
        "admin-add-trip",
        bus_number,
        source,
        destination,
        date,
        time,
        fare
    )
    return jsonify(cpp_payload), status
    return jsonify(cpp_payload), status
@app.delete("/api/admin/trips/<trip_id>")
def admin_delete_trip(trip_id: str):
    """Delete a trip (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-delete-trip", trip_id)
    return jsonify(cpp_payload), status
@app.patch("/api/admin/buses/<bus_id>/status")
def admin_toggle_bus_status(bus_id: str):
    """Toggle bus active status (admin only) - delegates to C++ backend."""
    cpp_payload, status = _run_cpp("admin-toggle-bus", bus_id)
    return jsonify(cpp_payload), status
if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)
