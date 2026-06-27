
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
const int MAX_BUSES = 50;
const int MAX_USERS = 100;
const int MAX_ROUTES = 100;
const int MAX_CITIES = 50; 
const int SEATS_PER_BUS = 20;
const int MAX_BOOKINGS = 500;
const string DATA_DIR = "output/";
const int MAX_TRIPS = 100; 
struct Trip {
    int tripID;
    char busNumber[20];
    char source[50];
    char destination[50];
    char date[20];
    char time[10];
    int fare;
    int maxSeats;
    bool active;
};
Trip trips[MAX_TRIPS];
int tripCount = 0;
void loadTrips();
void saveTrips();
struct Bus {
    int busID;
    char busNumber[20];
    int totalSeats;
    bool active;
};
struct City {
    int cityID;
    char name[50];
    bool active;
};
struct User {
    int userID;
    char username[50];
    char password[50];
    char phone[20];
    char name[100];
    char email[120];
    bool isAdmin;
    int failedLoginAttempts;
    bool isLocked;
    char secQuestion1[100];
    char secAnswer1[50];
    char secQuestion2[100];
    char secAnswer2[50];
    int completedTrips; 
};
struct Route {
    int routeID;
    char source[50];
    char destination[50];
    int distanceKM;
};
struct BookingSession {
    int bookingID;
    int userID;
    int tripID; 
    int seatNumber;
    char passengerName[50];
    char gender;
    int fare;
    char bookingDate[20];
    bool active;
    long long bookingTimestamp;
    bool usedLoyaltyDiscount; 
};
struct SeatInfo {
    int seatNumber;
    bool isBooked;
    char gender;
    int userID;
};
struct Refund {
    int refundID;
    int bookingID;
    int userID;
    int amount;
    char accountNumber[50];
    char accountType[20];
    char status[20];
    char processedDate[20];
};
Bus buses[MAX_BUSES];
int busCount = 0;
User users[MAX_USERS];
int userCount = 0;
Route routes[MAX_ROUTES];
int routeCount = 0;
BookingSession bookings[MAX_BOOKINGS];
int bookingCount = 0;
Refund refunds[MAX_BOOKINGS];
int refundCount = 0;
City cities[MAX_CITIES];
int cityCount = 0;
void initializeSystem();
void saveData();
void loadBuses();
void saveBuses();
void loadUsers();
void saveUsers();
void loadTrips();
void saveTrips();
void loadRefunds();
void saveRefunds();
void loadCities();
void saveCities();
void resetPassword();
void resetUsername();
int countActiveBuses();
int countDailyBookings();
long long calculateTotalRevenue();
long long calculateMonthlyRevenue();
int countTotalBookings();
void splitString(const string& str, char delimiter, string tokens[], int& count);
void mainMenu();
void userMenu(int userIndex);
void adminMenu(int userIndex);
bool login(bool asAdmin, int& userIndex);
void signup();
void searchAndBookBus(int userIndex);
void viewMyBookings(int userIndex);
void cancelBooking(int userIndex);
void changePassword(int userIndex);
void viewMyRefunds(int userIndex);
void adminAddBus();
void adminViewBuses();
void adminUpdateBus();
void adminDeleteBus();
void adminViewAllBookings();
void loadSeatsForBus(int busID, SeatInfo seats[]);
void saveSeatsForBus(int busID, SeatInfo seats[]);
bool processPayment(int fare);
string getCurrentDate();
int generateID(const char* type);
void loadRoutes();
void saveRoutes();
void loadBookings();
void saveBookings();
void clearInputBuffer();
bool isPasswordValid(const char* password);
int strcasecmp_custom(const char* s1, const char* s2);
int cliMain(int argc, char* argv[]);
bool isPhoneValid(const string& phone);
int findUserIndexByUsername(const string& username);
int findUserIndexByPhone(const string& phone);
int findUserIndexByEmail(const string& email);
int main(int argc, char* argv[]) {
    if (argc > 1) {
        initializeSystem();
        int rc = cliMain(argc, argv);
        return rc;
    }
    initializeSystem();
    mainMenu();
    return 0;
}
void initializeSystem() {
    loadRoutes();
    loadBuses();
    loadUsers();
    loadBookings();
    loadRefunds();
    loadCities();
    loadTrips();
}
void saveData() {
    saveBuses();
    saveUsers();
    saveRoutes();
    saveBookings();
    saveRefunds();
    saveCities();
    saveTrips();
}
void loadUsers() {
    ifstream file(DATA_DIR + "users.txt");
    if (!file.is_open()) return;
    string line;
    userCount = 0;
    while (getline(file, line) && userCount < MAX_USERS) {
        string tokens[15];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 14) {
            users[userCount].userID = stoi(tokens[0]);
            strcpy(users[userCount].username, tokens[1].c_str());
            strcpy(users[userCount].password, tokens[2].c_str());
            strcpy(users[userCount].phone, tokens[3].c_str());
            users[userCount].isAdmin = (tokens[4] == "1");
            users[userCount].failedLoginAttempts = stoi(tokens[5]);
            users[userCount].isLocked = (tokens[6] == "1");
            strcpy(users[userCount].secQuestion1, tokens[7].c_str());
            strcpy(users[userCount].secAnswer1, tokens[8].c_str());
            strcpy(users[userCount].secQuestion2, tokens[9].c_str());
            strcpy(users[userCount].secAnswer2, tokens[10].c_str());
            strcpy(users[userCount].name, tokens[11].c_str());
            strcpy(users[userCount].email, tokens[12].c_str());
            users[userCount].completedTrips = stoi(tokens[13]);
            userCount++;
        } else if (count >= 12) {
            users[userCount].userID = stoi(tokens[0]);
            strcpy(users[userCount].username, tokens[1].c_str());
            strcpy(users[userCount].password, tokens[2].c_str());
            strcpy(users[userCount].phone, tokens[3].c_str());
            users[userCount].isAdmin = (tokens[4] == "1");
            users[userCount].failedLoginAttempts = stoi(tokens[5]);
            users[userCount].isLocked = (tokens[6] == "1");
            strcpy(users[userCount].secAnswer1, tokens[7].c_str());
            strcpy(users[userCount].secAnswer2, tokens[8].c_str());
            strcpy(users[userCount].name, tokens[9].c_str());
            strcpy(users[userCount].email, tokens[10].c_str());
            users[userCount].completedTrips = stoi(tokens[11]);
            userCount++;
        } else if (count >= 11) {
            users[userCount].userID = stoi(tokens[0]);
            strcpy(users[userCount].username, tokens[1].c_str());
            strcpy(users[userCount].password, tokens[2].c_str());
            strcpy(users[userCount].phone, tokens[3].c_str());
            users[userCount].isAdmin = (tokens[4] == "1");
            users[userCount].failedLoginAttempts = stoi(tokens[5]);
            users[userCount].isLocked = (tokens[6] == "1");
            strcpy(users[userCount].secAnswer1, tokens[7].c_str());
            strcpy(users[userCount].secAnswer2, tokens[8].c_str());
            strcpy(users[userCount].name, tokens[9].c_str());
            strcpy(users[userCount].email, tokens[10].c_str());
            users[userCount].completedTrips = 0; 
            userCount++;
        }
    }
    file.close();
}
void saveUsers() {
    ofstream file(DATA_DIR + "users.txt", ios::trunc);
    if (!file.is_open()) return;
    for (int i = 0; i < userCount; i++) {
           file << users[i].userID << "|"
               << users[i].username << "|"
               << users[i].password << "|"
               << users[i].phone << "|"
               << (users[i].isAdmin ? 1 : 0) << "|"
               << users[i].failedLoginAttempts << "|"
               << (users[i].isLocked ? 1 : 0) << "|"
               << users[i].secAnswer1 << "|"
               << users[i].secAnswer2 << "|"
               << users[i].name << "|"
               << users[i].email << "|"
               << users[i].completedTrips << "\n";
    }
    file.close();
}
void loadBuses() {
    ifstream file(DATA_DIR + "buses.txt");
    if (!file.is_open()) return;
    string line;
    busCount = 0;
    while (getline(file, line) && busCount < MAX_BUSES) {
        string tokens[5];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 4) {
            buses[busCount].busID = stoi(tokens[0]);
            strcpy(buses[busCount].busNumber, tokens[1].c_str());
            buses[busCount].totalSeats = stoi(tokens[2]);
            buses[busCount].active = (tokens[3] == "1");
            busCount++;
        }
    }
    file.close();
}
void saveBuses() {
    ofstream file(DATA_DIR + "buses.txt");
    for (int i = 0; i < busCount; i++) {
        file << buses[i].busID << "|"
             << buses[i].busNumber << "|"
             << buses[i].totalSeats << "|"
             << (buses[i].active ? 1 : 0) << "\n";
    }
    file.close();
}
void loadRoutes() {
}
void saveRoutes() {
}
void loadBookings() {
    ifstream file(DATA_DIR + "bookings.txt");
    if (!file.is_open()) return;
    string line;
    bookingCount = 0;
    while (getline(file, line) && bookingCount < MAX_BOOKINGS) {
        string tokens[15];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 9) {
            bookings[bookingCount].bookingID = stoi(tokens[0]);
            bookings[bookingCount].userID = stoi(tokens[1]);
            bookings[bookingCount].tripID = stoi(tokens[2]); 
            bookings[bookingCount].seatNumber = stoi(tokens[3]);
            strcpy(bookings[bookingCount].passengerName, tokens[4].c_str());
            bookings[bookingCount].gender = tokens[5][0];
            bookings[bookingCount].fare = stoi(tokens[6]);
            strcpy(bookings[bookingCount].bookingDate, tokens[7].c_str());
            bookings[bookingCount].active = (tokens[8] == "1");
            if (count >= 10) {
                 try {
                    bookings[bookingCount].bookingTimestamp = stoll(tokens[9]);
                 } catch (...) {
                    bookings[bookingCount].bookingTimestamp = 0;
                 }
            } else {
                 bookings[bookingCount].bookingTimestamp = (long long)time(0);
            }
            if (count >= 11) {
                bookings[bookingCount].usedLoyaltyDiscount = (tokens[10] == "1");
            } else {
                bookings[bookingCount].usedLoyaltyDiscount = false;
            }
            bookingCount++;
        }
    }
    file.close();
}
void saveBookings() {
    ofstream file(DATA_DIR + "bookings.txt");
    for (int i = 0; i < bookingCount; i++) {
        file << bookings[i].bookingID << "|"
             << bookings[i].userID << "|"
             << bookings[i].tripID << "|"
             << bookings[i].seatNumber << "|"
             << bookings[i].passengerName << "|"
             << bookings[i].gender << "|"
             << bookings[i].fare << "|"
             << bookings[i].bookingDate << "|"
             << (bookings[i].active ? 1 : 0) << "|"
             << bookings[i].bookingTimestamp << "|"
             << (bookings[i].usedLoyaltyDiscount ? 1 : 0) << "\n";
    }
    file.close();
}
void loadRefunds() {
    ifstream file(DATA_DIR + "refunds.txt");
    if (!file.is_open()) return;
    string line;
    refundCount = 0;
    while (getline(file, line) && refundCount < MAX_BOOKINGS) {
        string tokens[10];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 7) {
            refunds[refundCount].refundID = stoi(tokens[0]);
            refunds[refundCount].bookingID = stoi(tokens[1]);
            refunds[refundCount].userID = stoi(tokens[2]);
            refunds[refundCount].amount = stoi(tokens[3]);
            strcpy(refunds[refundCount].accountNumber, tokens[4].c_str());
            strcpy(refunds[refundCount].accountType, tokens[5].c_str());
            strcpy(refunds[refundCount].status, tokens[6].c_str());
            strcpy(refunds[refundCount].processedDate, tokens[7].c_str());
            refundCount++;
        }
    }
    file.close();
}
void saveRefunds() {
    ofstream file(DATA_DIR + "refunds.txt");
    for (int i = 0; i < refundCount; i++) {
        file << refunds[i].refundID << "|"
             << refunds[i].bookingID << "|"
             << refunds[i].userID << "|"
             << refunds[i].amount << "|"
             << refunds[i].accountNumber << "|"
             << refunds[i].accountType << "|"
             << refunds[i].status << "|"
             << refunds[i].processedDate << "\n";
    }
    file.close();
}
void loadCities() {
    ifstream file(DATA_DIR + "cities.txt");
    if (!file.is_open()) return;
    string line;
    cityCount = 0;
    while (getline(file, line) && cityCount < MAX_CITIES) {
        string tokens[3];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 3) {
            cities[cityCount].cityID = stoi(tokens[0]);
            strcpy(cities[cityCount].name, tokens[1].c_str());
            cities[cityCount].active = (tokens[2] == "1");
            cityCount++;
        }
    }
    file.close();
}
void saveCities() {
    ofstream file(DATA_DIR + "cities.txt");
    for (int i = 0; i < cityCount; i++) {
        file << cities[i].cityID << "|"
             << cities[i].name << "|"
             << (cities[i].active ? 1 : 0) << "\n";
    }
    file.close();
}
void loadTrips() {
    ifstream file(DATA_DIR + "trip.txt");
    if (!file.is_open()) return;
    string line;
    tripCount = 0;
    while (getline(file, line) && tripCount < MAX_TRIPS) {
        string tokens[10];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 9) {
            trips[tripCount].tripID = stoi(tokens[0]);
            strcpy(trips[tripCount].busNumber, tokens[1].c_str());
            strcpy(trips[tripCount].source, tokens[2].c_str());
            strcpy(trips[tripCount].destination, tokens[3].c_str());
            strcpy(trips[tripCount].date, tokens[4].c_str());
            strcpy(trips[tripCount].time, tokens[5].c_str());
            trips[tripCount].fare = stoi(tokens[6]);
            trips[tripCount].maxSeats = stoi(tokens[7]);
            trips[tripCount].active = (tokens[8] == "1");
            tripCount++;
        }
    }
    file.close();
}
void saveTrips() {
    ofstream file(DATA_DIR + "trip.txt");
    for (int i = 0; i < tripCount; i++) {
        file << trips[i].tripID << "|"
             << trips[i].busNumber << "|"
             << trips[i].source << "|"
             << trips[i].destination << "|"
             << trips[i].date << "|"
             << trips[i].time << "|"
             << trips[i].fare << "|"
             << trips[i].maxSeats << "|"
             << (trips[i].active ? 1 : 0) << "\n";
    }
    file.close();
}
void splitString(const string& str, char delimiter, string tokens[], int& count) {
    stringstream ss(str);
    string token;
    count = 0;
    while (getline(ss, token, delimiter)) {
        tokens[count++] = token;
    }
}
int generateID(const char* type) {
    if (strcmp(type, "bus") == 0) {
        int maxID = 0;
        for (int i = 0; i < busCount; i++) {
            if (buses[i].busID > maxID) maxID = buses[i].busID;
        }
        return maxID + 1;
    } else if (strcmp(type, "user") == 0) {
        int maxID = 0;
        for (int i = 0; i < userCount; i++) {
            if (users[i].userID > maxID) maxID = users[i].userID;
        }
        return maxID + 1;
    } else if (strcmp(type, "route") == 0) {
        int maxID = 0;
        for (int i = 0; i < routeCount; i++) {
            if (routes[i].routeID > maxID) maxID = routes[i].routeID;
        }
        return maxID + 1;
    } else if (strcmp(type, "booking") == 0) {
        int maxID = 0;
        for (int i = 0; i < bookingCount; i++) {
            if (bookings[i].bookingID > maxID) maxID = bookings[i].bookingID;
        }
        return maxID + 1;
    } else if (strcmp(type, "refund") == 0) {
        int maxID = 0;
        for (int i = 0; i < refundCount; i++) {
            if (refunds[i].refundID > maxID) maxID = refunds[i].refundID;
        }
        return maxID + 1;
    } else if (strcmp(type, "trip") == 0) {
        int maxID = 0;
        for (int i = 0; i < tripCount; i++) {
            if (trips[i].tripID > maxID) maxID = trips[i].tripID;
        }
        return maxID + 1;
    }
    return 1;
}
string getCurrentDate() {
    time_t now = time(0);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&now));
    return string(buffer);
}
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}
bool isPasswordValid(const char* password) {
    bool hasLetter = false;
    bool hasDigit = false;
    int length = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        char c = password[i];
        length++;
        if (isalpha(static_cast<unsigned char>(c))) hasLetter = true;
        if (isdigit(static_cast<unsigned char>(c))) hasDigit = true;
    }
    return length >= 6 && hasLetter && hasDigit;
}
int strcasecmp_custom(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        char c1 = tolower(*s1);
        char c2 = tolower(*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return tolower(*s1) - tolower(*s2);
}
int countActiveBuses() {
    int count = 0;
    for (int i = 0; i < busCount; i++) {
        if (buses[i].active) {
            count++;
        }
    }
    return count;
}
int countDailyBookings() {
    string today = getCurrentDate();
    int count = 0;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].active && string(bookings[i].bookingDate) == today) {
            count++;
        }
    }
    return count;
}
long long calculateTotalRevenue() {
    long long total = 0;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].fare < 0) {
            continue; 
        }
        if (bookings[i].active) {
            if (total > 9223372036854775807LL - bookings[i].fare) {
                return 9223372036854775807LL;
            }
            total += bookings[i].fare;
        }
    }
    return total;
}
long long calculateMonthlyRevenue() {
    string today = getCurrentDate();
    string currentMonth = today.substr(0, 7); 
    long long total = 0;
    for (int i = 0; i < bookingCount; i++) {
        if (!bookings[i].active) continue; 
        string bDate = bookings[i].bookingDate;
        if (bDate.length() >= 7 && bDate.substr(0, 7) == currentMonth) {
            if (bookings[i].fare < 0) continue;
            if (total > 9223372036854775807LL - bookings[i].fare) {
                return 9223372036854775807LL;
            }
            total += bookings[i].fare;
        }
    }
    return total;
}
int countTotalBookings() {
    return bookingCount;
}
bool isEmailValid(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos) && (dotPos != string::npos);
}
bool isPhoneValid(const string& phone) {
    if (phone.length() != 11) return false;
    for (char c : phone) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}
int findUserIndexByUsername(const string& username) {
    for (int i = 0; i < userCount; i++) {
        if (strcasecmp_custom(users[i].username, username.c_str()) == 0) {
            return i;
        }
    }
    return -1;
}
int findUserIndexByPhone(const string& phone) {
    for (int i = 0; i < userCount; i++) {
        if (phone == string(users[i].phone)) {
            return i;
        }
    }
    return -1;
}
int findUserIndexByEmail(const string& email) {
    for (int i = 0; i < userCount; i++) {
        if (strcasecmp_custom(users[i].email, email.c_str()) == 0) {
            return i;
        }
    }
    return -1;
}
void updateCompletedTripsForUser(int userIdx) {
    int userId = users[userIdx].userID;
    users[userIdx].completedTrips = 0; 
    time_t now = time(0);
    struct tm nowTm;
    nowTm = *localtime(&now);
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].userID == userId && bookings[i].active) {
            int tripIdx = -1;
            for (int t = 0; t < tripCount; t++) {
                 if (trips[t].tripID == bookings[i].tripID) {
                      tripIdx = t;
                      break;
                 }
            }
            if (tripIdx == -1) continue;
            string dateStr = trips[tripIdx].date;
            string today = getCurrentDate();
            if (dateStr < today) {
                   users[userIdx].completedTrips++;
            }
        }
    }
}
int cliMain(int argc, char* argv[]) {
    string action = argv[1];
    auto printSuccess = [](const string& json) {
        cout << json << endl;
        return 0;
    };
    auto printError = [](const string& json, int code = 1) {
        cout << json << endl;
        return code;
    };
    if (action == "signup") {
        if (argc < 10) {
            return printError("{\"success\":false,\"message\":\"Missing signup arguments\"}");
        }
        string username = argv[2];
        string password = argv[3];
        string confirmPassword = argv[4];
        string phone = argv[5];
        string name = argv[6];
        string email = argv[7];
        string motherName = argv[8];
        string birthCity = argv[9];
        if (password != confirmPassword) {
            return printError("{\"success\":false,\"message\":\"Passwords do not match.\"}");
        }
        if (!isPasswordValid(password.c_str())) {
            return printError("{\"success\":false,\"message\":\"Password does not meet requirements.\"}");
        }
        if (!isPhoneValid(phone)) {
            return printError("{\"success\":false,\"message\":\"Phone must be 11 digits.\"}");
        }
        if (!isEmailValid(email)) {
            return printError("{\"success\":false,\"message\":\"Invalid email format.\"}");
        }
        if (findUserIndexByUsername(username) >= 0) {
            return printError("{\"success\":false,\"message\":\"Username already exists.\"}");
        }
        if (findUserIndexByPhone(phone) >= 0) {
            return printError("{\"success\":false,\"message\":\"Phone number already registered.\"}");
        }
        if (findUserIndexByEmail(email) >= 0) {
            return printError("{\"success\":false,\"message\":\"Email address already registered.\"}");
        }
        if (userCount >= MAX_USERS) {
            return printError("{\"success\":false,\"message\":\"User limit reached.\"}");
        }
        int newId = (userCount == 0 ? 1 : users[userCount-1].userID + 1);
        users[userCount].userID = newId;
        strcpy(users[userCount].username, username.c_str());
        strcpy(users[userCount].password, password.c_str());
        strcpy(users[userCount].phone, phone.c_str());
        strcpy(users[userCount].name, name.c_str());
        strcpy(users[userCount].email, email.c_str());
        users[userCount].isAdmin = false;
        users[userCount].failedLoginAttempts = 0;
        users[userCount].isLocked = false;
        strcpy(users[userCount].secQuestion1, "What is your mother's name?");
        strcpy(users[userCount].secAnswer1, motherName.c_str());
        strcpy(users[userCount].secQuestion2, "What is your birth city?");
        strcpy(users[userCount].secAnswer2, birthCity.c_str());
        userCount++;
        saveUsers();
        return printSuccess("{\"success\":true,\"username\":\"" + username + "\"}");
    }
    if (action == "recover-username") {
        if (argc < 5) {
            return printError("{\"success\":false,\"message\":\"Missing recovery arguments\"}");
        }
        string phone = argv[2];
        string motherName = argv[3];
        string birthCity = argv[4];
        for (int i = 0; i < userCount; i++) {
            if (phone == string(users[i].phone) &&
                strcasecmp_custom(users[i].secAnswer1, motherName.c_str()) == 0 &&
                strcasecmp_custom(users[i].secAnswer2, birthCity.c_str()) == 0) {
                string json = "{\"success\":true,\"username\":\"" + string(users[i].username) + "\"}";
                return printSuccess(json);
            }
        }
        return printError("{\"success\":false,\"message\":\"No account found with these details.\"}", 1);
    }
    if (action == "verify-security") {
        if (argc < 5) {
            return printError("{\"success\":false,\"message\":\"Missing verification arguments\"}");
        }
        string username = argv[2];
        string motherName = argv[3];
        string birthCity = argv[4];
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"Username not found.\"}", 1);
        }
        if (strcasecmp_custom(users[idx].secAnswer1, motherName.c_str()) == 0 &&
            strcasecmp_custom(users[idx].secAnswer2, birthCity.c_str()) == 0) {
            return printSuccess("{\"success\":true,\"message\":\"Identity verified.\"}");
        }
        return printError("{\"success\":false,\"message\":\"Invalid security answers.\"}", 1);
    }
    if (action == "reset-password-username") {
        if (argc < 4) {
            return printError("{\"success\":false,\"message\":\"Missing reset arguments\"}");
        }
        string username = argv[2];
        string newPassword = argv[3];
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found.\"}", 1);
        }
        if (!isPasswordValid(newPassword.c_str())) {
            return printError("{\"success\":false,\"message\":\"Password does not meet requirements.\"}", 1);
        }
        if (strcmp(users[idx].password, newPassword.c_str()) == 0) {
            return printError("{\"success\":false,\"message\":\"New password cannot be the same as your old password.\"}", 1);
        }
        strcpy(users[idx].password, newPassword.c_str());
        users[idx].failedLoginAttempts = 0;
        users[idx].isLocked = false;
        saveUsers();
        return printSuccess("{\"success\":true,\"message\":\"Password reset successfully.\"}");
    }
    if (action == "migrate-users") {
        saveUsers();
        return printSuccess("{\"success\":true,\"message\":\"Users migrated to optimized schema.\"}");
    }
    if (action == "check-username") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username argument\"}");
        }
        string username = argv[2];
        int idx = findUserIndexByUsername(username);
        string json = "{\"success\":true,\"exists\":" + string(idx >= 0 ? "true" : "false") + "}";
        return printSuccess(json);
    }
    if (action == "check-phone") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing phone argument\"}");
        }
        string phone = argv[2];
        int idx = findUserIndexByPhone(phone);
        string json = "{\"success\":true,\"exists\":" + string(idx >= 0 ? "true" : "false") + "}";
        return printSuccess(json);
    }
    if (action == "check-email") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing email argument\"}");
        }
        string email = argv[2];
        int idx = findUserIndexByEmail(email);
        string json = "{\"success\":true,\"exists\":" + string(idx >= 0 ? "true" : "false") + "}";
        return printSuccess(json);
    }
    if (action == "login") {
        if (argc < 4) {
            return printError("{\"success\":false,\"message\":\"Missing login arguments\"}");
        }
        string username = argv[2];
        string password = argv[3];
        string loginType = (argc >= 5) ? string(argv[4]) : "user"; 
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"Invalid username or password\"}");
        }
        if (users[idx].isLocked) {
             return printError("{\"success\":false,\"message\":\"Account is locked\"}");
        }
        if (strcmp(users[idx].password, password.c_str()) == 0) {
            bool isAdminUser = users[idx].isAdmin;
            bool isAdminLogin = (loginType == "admin");
            if (isAdminLogin && !isAdminUser) {
                return printError("{\"success\":false,\"message\":\"Access denied. This is not an admin account.\"}");
            }
            if (!isAdminLogin && isAdminUser) {
                return printError("{\"success\":false,\"message\":\"Access denied. Admin accounts cannot login to user section.\"}");
            }
            users[idx].failedLoginAttempts = 0;
            saveUsers();
            string json = "{\"success\":true,\"user\":{\"userID\":" + to_string(users[idx].userID) +
                          ",\"username\":\"" + string(users[idx].username) + "\"" +
                          ",\"name\":\"" + string(users[idx].name) + "\"" +
                          ",\"email\":\"" + string(users[idx].email) + "\"" +
                          ",\"phone\":\"" + string(users[idx].phone) + "\"" +
                          ",\"isAdmin\":" + string(users[idx].isAdmin ? "true" : "false") +
                          ",\"completedTrips\":" + to_string(users[idx].completedTrips) + "}}";
            return printSuccess(json);
        } else {
            users[idx].failedLoginAttempts++;
            if (users[idx].failedLoginAttempts >= 3) {
                users[idx].isLocked = true;
            }
            saveUsers();
            return printError("{\"success\":false,\"message\":\"Invalid username or password\"}");
        }
    }
    if (action == "update-password") {
        if (argc < 5) {
            return printError("{\"success\":false,\"message\":\"Missing arguments\"}");
        }
        string username = argv[2];
        string currentPass = argv[3];
        string newPass = argv[4];
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        if (strcmp(users[idx].password, currentPass.c_str()) != 0) {
            return printError("{\"success\":false,\"message\":\"Incorrect current password\"}");
        }
        if (!isPasswordValid(newPass.c_str())) {
            return printError("{\"success\":false,\"message\":\"New password does not meet requirements\"}");
        }
        if (strcmp(currentPass.c_str(), newPass.c_str()) == 0) {
            return printError("{\"success\":false,\"message\":\"New password cannot be same as current password\"}");
        }
        strcpy(users[idx].password, newPass.c_str());
        saveUsers();
        return printSuccess("{\"success\":true}");
    }
    if (action == "search-buses") {
        if (argc < 5) {
            return printError("{\"success\":false,\"message\":\"Missing search arguments\"}");
        }
        string source = argv[2];
        string destination = argv[3];
        string date = argv[4];
        string json = "{\"success\":true,\"buses\":[";
        bool first = true;
        for (int i = 0; i < tripCount; i++) {
            if (trips[i].active &&
                strcasecmp_custom(trips[i].source, source.c_str()) == 0 &&
                strcasecmp_custom(trips[i].destination, destination.c_str()) == 0 &&
                strcmp(trips[i].date, date.c_str()) == 0) {
                if (!first) json += ",";
                json += "{\"busID\":" + to_string(trips[i].busNumber[0] == 'B' ? stoi(string(trips[i].busNumber).substr(4)) : 0) + 
                        ",\"tripID\":" + to_string(trips[i].tripID) + 
                        ",\"busNumber\":\"" + string(trips[i].busNumber) + "\"" +
                        ",\"time\":\"" + string(trips[i].time) + "\"" +
                        ",\"seatsAvailable\":" + to_string(trips[i].maxSeats) + 
                        ",\"fare\":" + to_string(trips[i].fare) + "}";
                first = false;
            }
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "get-seats") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing bus ID\"}");
        }
        int busID = stoi(argv[2]);
        SeatInfo seats[SEATS_PER_BUS];
        loadSeatsForBus(busID, seats);
        string json = "{\"success\":true,\"seats\":{";
        bool first = true;
        for (int i = 0; i < SEATS_PER_BUS; i++) {
            if (!first) json += ",";
            json += "\"" + to_string(seats[i].seatNumber) + "\":\"" + 
                    (seats[i].isBooked ? string(1, seats[i].gender) : "*") + "\"";
            first = false;
        }
        json += "}}";
        return printSuccess(json);
    }
    if (action == "get-loyalty") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username\"}");
        }
        string username = argv[2];
        int userIdx = findUserIndexByUsername(username);
        int completed = 0;
        int eligible = 0;
        if (userIdx >= 0) {
            updateCompletedTripsForUser(userIdx);
            saveUsers();
            completed = users[userIdx].completedTrips;
            eligible = (completed >= 5) ? 1 : 0;
        }
        string json = "{\"success\":true,\"loyalty\":{\"completedTrips\":" + to_string(completed) + ",\"eligibleDiscount\":" + to_string(eligible) + "}}";
        return printSuccess(json);
    }
    if (action == "get-my-bookings") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username\"}");
        }
        string username = argv[2];
        int userIdx = findUserIndexByUsername(username);
        string json = "{\"success\":true,\"bookings\":[";
        bool first = true;
        if (userIdx >= 0) {
            int uid = users[userIdx].userID;
            for (int i = 0; i < bookingCount; i++) {
                if (bookings[i].userID == uid) {
                    if (!first) json += ",";
                    string busNum = "Unknown";
                    for (int t = 0; t < tripCount; t++) {
                         if (trips[t].tripID == bookings[i].tripID) { 
                             busNum = trips[t].busNumber;
                             break;
                         }
                    }
                    json += "{\"bookingID\":" + to_string(bookings[i].bookingID) + 
                            ",\"busID\":" + to_string(bookings[i].tripID) + 
                            ",\"busNumber\":\"" + busNum + "\"" +
                            ",\"seatNumber\":" + to_string(bookings[i].seatNumber) +
                            ",\"fare\":" + to_string(bookings[i].fare) +
                            ",\"date\":\"" + string(bookings[i].bookingDate) + "\"" +
                            ",\"refundable\":" + (bookings[i].active ? "true" : "false") + "}";
                    first = false;
                }
            }
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "get-my-refunds") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username\"}");
        }
        string username = argv[2];
        int userIdx = findUserIndexByUsername(username);
        string json = "{\"success\":true,\"refunds\":[";
        bool first = true;
        if (userIdx >= 0) {
            int uid = users[userIdx].userID;
            for (int i = 0; i < refundCount; i++) {
                if (refunds[i].userID == uid) {
                    if (!first) json += ",";
                    json += "{\"refundID\":" + to_string(refunds[i].refundID) + 
                            ",\"bookingID\":" + to_string(refunds[i].bookingID) +
                            ",\"amount\":" + to_string(refunds[i].amount) +
                            ",\"status\":\"" + string(refunds[i].status) + "\"" +
                            ",\"accountType\":\"" + string(refunds[i].accountType) + "\"" +
                            ",\"processedDate\":\"" + string(refunds[i].processedDate) + "\"}";
                    first = false;
                }
            }
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "get-seats") {
        if (argc < 3) return printError("{\"success\":false,\"message\":\"Missing trip ID\"}");
        int tripID = 0;
        try {
            tripID = stoi(argv[2]);
        } catch (...) {
            return printError("{\"success\":false,\"message\":\"Invalid trip ID\"}");
        }
        SeatInfo seats[SEATS_PER_BUS];
        loadSeatsForBus(tripID, seats);
        string json = "{\"success\":true,\"seats\":{";
        bool first = true;
        for (int i = 0; i < SEATS_PER_BUS; i++) {
            if (seats[i].isBooked) {
                if (!first) json += ",";
                json += "\"" + to_string(i + 1) + "\":\"" + string(1, seats[i].gender) + "\"";
                first = false;
            }
        }
        json += "}}";
        return printSuccess(json);
    }
    if (action == "book-seats") {
        if (argc < 7) {
            return printError("{\"success\":false,\"message\":\"Missing booking arguments\"}");
        }
        string username = argv[2];
        string seatsStr = argv[4];
        string paymentMethod = argv[5];
        int tripID;
        int tokensRedeemed;
        vector<int> seatNumbers;
        vector<char> seatGenders;
        vector<string> seatNames;
        try {
            tripID = stoi(argv[3]); 
            tokensRedeemed = stoi(argv[6]);
            stringstream ss(seatsStr);
            string segment;
            while (getline(ss, segment, ',')) {
                 if (!segment.empty()) {
                    size_t firstColon = segment.find(':');
                    if (firstColon != string::npos) {
                        try {
                            int sNum = stoi(segment.substr(0, firstColon));
                            size_t secondColon = segment.find(':', firstColon + 1);
                            if (secondColon != string::npos) {
                                char g = segment[firstColon + 1];
                                string pName = segment.substr(secondColon + 1);
                                seatNumbers.push_back(sNum);
                                seatGenders.push_back(g);
                                seatNames.push_back(pName);
                            } else {
                                char g = segment[firstColon + 1];
                                seatNumbers.push_back(sNum);
                                seatGenders.push_back(g);
                                seatNames.push_back(""); 
                            }
                        } catch (...) {}
                    } else {
                        try {
                            seatNumbers.push_back(stoi(segment));
                            seatGenders.push_back('M');
                            seatNames.push_back("");
                        } catch (...) {}
                    }
                 }
            }
        } catch (...) {
            return printError("{\"success\":false,\"message\":\"Invalid input format (ID/Seats/Tokens)\"}");
        }
        int userIdx = findUserIndexByUsername(username);
        if (userIdx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        int tripIdx = -1;
        for (int i = 0; i < tripCount; i++) {
            if (trips[i].tripID == tripID && trips[i].active) {
                tripIdx = i;
                break;
            }
        }
        if (tripIdx < 0) return printError("{\"success\":false,\"message\":\"Trip not found\"}");
        int busIdx = -1;
        for (int i = 0; i < busCount; i++) {
            if (strcasecmp_custom(buses[i].busNumber, trips[tripIdx].busNumber) == 0 && buses[i].active) {
                busIdx = i;
                break;
            }
        }
        if (busIdx < 0) return printError("{\"success\":false,\"message\":\"Assigned bus not found\"}");
        SeatInfo seats[SEATS_PER_BUS];
        loadSeatsForBus(tripID, seats); 
        for (int seatNum : seatNumbers) {
            if (seatNum < 1 || seatNum > SEATS_PER_BUS) {
                return printError("{\"success\":false,\"message\":\"Invalid seat number\"}");
            }
            if (seats[seatNum - 1].isBooked) {
                return printError("{\"success\":false,\"message\":\"Seat already booked\"}");
            }
        }
        for (size_t k = 0; k < seatNumbers.size(); k++) {
            int seatNum = seatNumbers[k];
            char gender = seatGenders[k];
            seats[seatNum - 1].isBooked = true;
            seats[seatNum - 1].gender = gender;
            seats[seatNum - 1].userID = users[userIdx].userID;
            if (bookingCount < MAX_BOOKINGS) {
                bookings[bookingCount].bookingID = generateID("booking");
                bookings[bookingCount].userID = users[userIdx].userID;
                bookings[bookingCount].tripID = tripID;
                bookings[bookingCount].seatNumber = seatNum;
                string pName = seatNames[k];
                if (pName.empty()) pName = users[userIdx].name;
                strncpy(bookings[bookingCount].passengerName, pName.c_str(), 49);
                bookings[bookingCount].passengerName[49] = '\0';
                bookings[bookingCount].gender = gender;
                bookings[bookingCount].fare = trips[tripIdx].fare;
            bookings[bookingCount].active = true;
            bookings[bookingCount].bookingTimestamp = (long long)time(0);
            if (users[userIdx].completedTrips >= 5) {
                 bookings[bookingCount].usedLoyaltyDiscount = true;
            } else {
                 bookings[bookingCount].usedLoyaltyDiscount = false;
            }
            bookingCount++;
        }
    }
        saveSeatsForBus(tripID, seats);
        saveBookings();
        updateCompletedTripsForUser(userIdx);
        int baseAmount = seatNumbers.size() * trips[tripIdx].fare;
        int discount = 0;
        if (users[userIdx].completedTrips >= 5) {
            discount = baseAmount / 10;
        }
        int amount = baseAmount - discount;
        saveUsers();
        string reference = "BK" + to_string(time(0) % 1000000);
        string json = "{\"success\":true,\"reference\":\"" + reference + 
                     "\",\"amount\":" + to_string(amount) + ",\"discount\":" + to_string(discount) + 
                     ",\"completedTrips\":" + to_string(users[userIdx].completedTrips) + "}";
        return printSuccess(json);
    }
    if (action == "user-bookings") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username\"}");
        }
        string username = argv[2];
        int userIdx = findUserIndexByUsername(username);
        if (userIdx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        int userID = users[userIdx].userID;
        string json = "{\"success\":true,\"bookings\":[";
        bool first = true;
        for (int i = 0; i < bookingCount; i++) {
            if (bookings[i].userID != userID) continue;
            string busNum = "Unknown";
            string depTime = "Unknown";
            string date = "Unknown";
            string src = "Unknown";
            string dest = "Unknown";
            for (int t = 0; t < tripCount; t++) {
                if (trips[t].tripID == bookings[i].tripID) {
                    busNum = trips[t].busNumber;
                    depTime = trips[t].time;
                    date = trips[t].date;
                    src = trips[t].source;
                    dest = trips[t].destination;
                    break;
                }
            }
            if (!first) json += ",";
            string bkRef = "BK" + to_string(100000 + bookings[i].bookingID);
            long long now = (long long)time(0);
            bool isRefundableWindow = (bookings[i].bookingTimestamp > 0 && (now - bookings[i].bookingTimestamp) <= 86400);
            json += "{\"bookingID\":" + to_string(bookings[i].bookingID) +
                    ",\"busID\":" + to_string(bookings[i].tripID) + 
                    ",\"busNumber\":\"" + busNum + "\"" +
                    ",\"time\":\"" + depTime + "\"" +
                    ",\"seatNumber\":" + to_string(bookings[i].seatNumber) +
                    ",\"fare\":" + to_string(bookings[i].fare) +
                    ",\"date\":\"" + date + "\"" +
                    ",\"source\":\"" + src + "\"" +
                    ",\"destination\":\"" + dest + "\"" +
                    ",\"passengerName\":\"" + string(bookings[i].passengerName) + "\"" +
                    ",\"reference\":\"" + bkRef + "\"" +
                    ",\"status\":\"" + string(bookings[i].active ? "Active" : "Cancelled") + "\"" +
                    ",\"refundable\":" + string(bookings[i].active ? "true" : "false") + 
                    ",\"isRefundableWindow\":" + string(isRefundableWindow ? "true" : "false") + "}";
            first = false;
                    ",\"destination\":\"" + dest + "\"" +
                    ",\"passengerName\":\"" + string(bookings[i].passengerName) + "\"" +
                    ",\"reference\":\"" + bkRef + "\"" +
                    ",\"status\":\"" + string(bookings[i].active ? "Active" : "Cancelled") + "\"" +
                    ",\"refundable\":" + string(bookings[i].active ? "true" : "false") + "}";
            first = false;
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "request-refund") {
        if (argc < 6) {
            return printError("{\"success\":false,\"message\":\"Missing refund arguments\"}");
        }
        string username = argv[2];
        int bookingID = stoi(argv[3]);
        string accountNumber = argv[4];
        string accountType = argv[5];
        int userIdx = findUserIndexByUsername(username);
        if (userIdx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        int bookingIdx = -1;
        for (int i = 0; i < bookingCount; i++) {
            if (bookings[i].bookingID == bookingID && bookings[i].userID == users[userIdx].userID) {
                bookingIdx = i;
                break;
            }
        }
        if (bookingIdx < 0) {
            return printError("{\"success\":false,\"message\":\"Booking not found for user\"}");
        }
        if (!bookings[bookingIdx].active) {
            return printError("{\"success\":false,\"message\":\"Booking already cancelled\"}");
        }
        for (int r = 0; r < refundCount; r++) {
            if (refunds[r].bookingID == bookingID) {
                return printError("{\"success\":false,\"message\":\"Refund already requested for this booking\"}");
            }
        }
        if (refundCount >= MAX_BOOKINGS) {
            return printError("{\"success\":false,\"message\":\"Refund limit reached\"}");
        }
        bool hasDiscountedBooking = false;
        for (int i = 0; i < bookingCount; i++) {
            if (bookings[i].userID == users[userIdx].userID && 
                bookings[i].active && 
                bookings[i].usedLoyaltyDiscount) {
                hasDiscountedBooking = true;
                break;
            }
        }
        if (hasDiscountedBooking) {
            int potentialCompletedTrips = 0;
            int userId = users[userIdx].userID;
            string today = getCurrentDate();
            for (int i = 0; i < bookingCount; i++) {
                if (bookings[i].bookingID == bookingID) continue;
                if (bookings[i].userID == userId && bookings[i].active) {
                     int tripIdx = -1;
                     for (int t = 0; t < tripCount; t++) {
                          if (trips[t].tripID == bookings[i].tripID) {
                               tripIdx = t;
                               break;
                          }
                     }
                     if (tripIdx != -1) {
                         if (string(trips[tripIdx].date) < today) {
                             potentialCompletedTrips++;
                         }
                     }
                }
            }
            if (potentialCompletedTrips < 5) {
                if (!bookings[bookingIdx].usedLoyaltyDiscount) {
                     return printError("{\"success\":false,\"message\":\"Cannot refund: This trip is required for your active loyalty discount.\"}");
                }
            }
        }
        SeatInfo seats[SEATS_PER_BUS];
        loadSeatsForBus(bookings[bookingIdx].tripID, seats); 
        int seatIdx = bookings[bookingIdx].seatNumber - 1;
        if (seatIdx >= 0 && seatIdx < SEATS_PER_BUS) {
            seats[seatIdx].isBooked = false;
            seats[seatIdx].gender = ' ';
            seats[seatIdx].userID = 0;
            saveSeatsForBus(bookings[bookingIdx].tripID, seats); 
        }
        bookings[bookingIdx].active = false;
        saveBookings();
        long long now = (long long)time(0);
        long long diff = now - bookings[bookingIdx].bookingTimestamp;
        if (diff > 86400) {
             return printSuccess("{\"success\":true,\"message\":\"Booking cancelled. No refund provided (Policy: >24 hours).\"}");
        }
        int refundID = generateID("refund");
        refunds[refundCount].refundID = refundID;
        refunds[refundCount].bookingID = bookingID;
        refunds[refundCount].userID = users[userIdx].userID;
        refunds[refundCount].amount = bookings[bookingIdx].fare;
        strcpy(refunds[refundCount].accountNumber, accountNumber.c_str());
        strcpy(refunds[refundCount].accountType, accountType.c_str());
        strcpy(refunds[refundCount].status, "Pending");
        string date = getCurrentDate();
        strcpy(refunds[refundCount].processedDate, date.c_str());
        refundCount++;
        saveRefunds();
        string json = "{\"success\":true,\"refundID\":" + to_string(refundID) + "}";
        return printSuccess(json);
    }
    if (action == "user-refunds") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username\"}");
        }
        string username = argv[2];
        int userIdx = findUserIndexByUsername(username);
        if (userIdx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        int userID = users[userIdx].userID;
        string json = "{\"success\":true,\"refunds\":[";
        bool first = true;
        for (int i = 0; i < refundCount; i++) {
            if (refunds[i].userID != userID) continue;
            if (!first) json += ",";
            json += "{\"refundID\":" + to_string(refunds[i].refundID) +
                    ",\"bookingID\":" + to_string(refunds[i].bookingID) +
                    ",\"amount\":" + to_string(refunds[i].amount) +
                    ",\"accountType\":\"" + string(refunds[i].accountType) + "\"" +
                    ",\"status\":\"" + string(refunds[i].status) + "\"" +
                    ",\"processedDate\":\"" + string(refunds[i].processedDate) + "\"" + "}";
            first = false;
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "update-credentials") {
        if (argc < 7) {
            return printError("{\"success\":false,\"message\":\"Missing credentials arguments\"}");
        }
        string currentUsername = argv[2];
        string newUsername = argv[3];
        string name = argv[4];
        string phone = argv[5];
        string email = argv[6];
        int idx = findUserIndexByUsername(currentUsername);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        if (currentUsername != newUsername) {
             int existingIdx = findUserIndexByUsername(newUsername);
             if (existingIdx != -1) {
                 return printError("{\"success\":false,\"message\":\"Username already taken\"}");
             }
        }
        if (!users[idx].isAdmin) {
            if (!isPhoneValid(phone)) {
                return printError("{\"success\":false,\"message\":\"Phone must be 11 digits\"}");
            }
            if (!isEmailValid(email)) {
                 return printError("{\"success\":false,\"message\":\"Invalid email format\"}");
            }
            for (int i = 0; i < userCount; i++) {
                if (i != idx && phone == string(users[i].phone)) {
                    return printError("{\"success\":false,\"message\":\"Phone number already registered\"}");
                }
            }
            for (int i = 0; i < userCount; i++) {
                if (i != idx && strcasecmp_custom(users[i].email, email.c_str()) == 0) {
                    return printError("{\"success\":false,\"message\":\"Email already registered\"}");
                }
            }
        }
        strcpy(users[idx].username, newUsername.c_str());
        strcpy(users[idx].name, name.c_str());
        strcpy(users[idx].phone, phone.c_str());
        strcpy(users[idx].email, email.c_str());
        saveUsers();
        string json = "{\"success\":true,\"message\":\"Credentials updated successfully\"}";
        return printSuccess(json);
    }
    if (action == "update-password") {
        if (argc < 5) {
            return printError("{\"success\":false,\"message\":\"Missing password arguments\"}");
        }
        string username = argv[2];
        string currentPassword = argv[3];
        string newPassword = argv[4];
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        if (string(users[idx].password) != currentPassword) {
            return printError("{\"success\":false,\"message\":\"Current password is incorrect\"}");
        }
        if (newPassword.length() < 6) {
            return printError("{\"success\":false,\"message\":\"Password must be at least 6 characters\"}");
        }
        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : newPassword) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }
        if (!hasUpper || !hasLower || !hasDigit) {
            return printError("{\"success\":false,\"message\":\"Password must have uppercase, lowercase, and digit\"}");
        }
        strcpy(users[idx].password, newPassword.c_str());
        saveUsers();
        string json = "{\"success\":true,\"message\":\"Password updated successfully\"}";
        return printSuccess(json);
    }
    if (action == "get-cities") {
        string json = "{\"success\":true,\"cities\":[";
        bool first = true;
        for (int i = 0; i < cityCount; i++) {
            if (cities[i].active) {
                if (!first) json += ",";
                json += "{\"id\":" + to_string(cities[i].cityID) + ",\"name\":\"" + string(cities[i].name) + "\",\"active\":true}";
                first = false;
            }
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "admin-add-city") {
        if (argc < 4) {
            return printError("{\"success\":false,\"message\":\"Missing city arguments\"}");
        }
        int cityID = stoi(argv[2]);
        string cityName = argv[3];
        if (cityCount >= MAX_CITIES) {
            return printError("{\"success\":false,\"message\":\"City limit reached\"}");
        }
        for (int i = 0; i < cityCount; i++) {
            if (cities[i].active) {
                if (cities[i].cityID == cityID) {
                    return printError("{\"success\":false,\"message\":\"City ID already exists\"}");
                }
                if (strcasecmp_custom(cities[i].name, cityName.c_str()) == 0) {
                    return printError("{\"success\":false,\"message\":\"City Name already exists\"}");
                }
            }
        }
        cities[cityCount].cityID = cityID;
        strcpy(cities[cityCount].name, cityName.c_str());
        cities[cityCount].active = true;
        cityCount++;
        saveCities();
        return printSuccess("{\"success\":true}");
    }
    if (action == "admin-delete-city") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing city ID\"}");
        }
        int cityID = stoi(argv[2]);
        int idx = -1;
        for (int i = 0; i < cityCount; i++) {
            if (cities[i].cityID == cityID) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            for (int i = idx; i < cityCount - 1; i++) {
                cities[i] = cities[i + 1];
            }
            cityCount--;
            saveCities();
            return printSuccess("{\"success\":true}");
        } else {
            return printError("{\"success\":false,\"message\":\"City not found\"}");
        }
    }
    if (action == "admin-bookings") {
        string json = "{\"success\":true,\"bookings\":[";
        bool first = true;
        for (int i = 0; i < bookingCount; i++) {
            if (!first) json += ",";
            json += "{\"bookingID\":" + to_string(bookings[i].bookingID) +
                    ",\"reference\":\"" + to_string(bookings[i].bookingID) + "\"" +
                    ",\"userID\":" + to_string(bookings[i].userID) +
                    ",\"username\":\"" + string(bookings[i].passengerName) + "\"" +
                    ",\"busID\":" + to_string(bookings[i].tripID) +
                    ",\"seats\":\"1\"" + 
                    ",\"seatNumber\":" + to_string(bookings[i].seatNumber) +
                    ",\"passengerName\":\"" + string(bookings[i].passengerName) + 
                    "\",\"amount\":" + to_string(bookings[i].fare) +
                    ",\"fare\":" + to_string(bookings[i].fare) +
                    ",\"datetime\":\"" + string(bookings[i].bookingDate) + "\"" +
                    ",\"status\":\"" + string(bookings[i].active ? "Active" : "Cancelled") + "\"}";
            first = false;
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "admin-add-bus") {
        if (argc < 4) {
            return printError("{\"success\":false,\"message\":\"Missing bus arguments\"}");
        }
        string busNumber = argv[2];
        int seats = stoi(argv[3]);
        if (busCount >= MAX_BUSES) {
            return printError("{\"success\":false,\"message\":\"Bus limit reached\"}");
        }
        for (int i = 0; i < busCount; i++) {
             if (strcasecmp_custom(buses[i].busNumber, busNumber.c_str()) == 0) {
                 return printError("{\"success\":false,\"message\":\"Bus number already exists\"}");
             }
        }
        buses[busCount].busID = generateID("bus");
        strcpy(buses[busCount].busNumber, busNumber.c_str());
        buses[busCount].totalSeats = seats;
        buses[busCount].active = true;
        busCount++;
        saveBuses();
        return printSuccess("{\"success\":true}");
    }
    if (action == "admin-buses") {
        string json = "{\"success\":true,\"buses\":[";
        bool first = true;
        for (int i = 0; i < busCount; i++) {
            if (!first) json += ",";
            json += "{\"busID\":" + to_string(buses[i].busID) +
                    ",\"busNumber\":\"" + string(buses[i].busNumber) + "\"" +
                    ",\"seats\":" + to_string(buses[i].totalSeats) +
                    ",\"active\":" + string(buses[i].active ? "true" : "false") + "}";
            first = false;
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "admin-delete-bus") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing bus ID\"}");
        }
        int busID = stoi(argv[2]);
        bool found = false;
        for (int i = 0; i < busCount; i++) {
            if (buses[i].busID == busID) {
                found = true;
                for (int j = i; j < busCount - 1; j++) {
                    buses[j] = buses[j + 1];
                }
                busCount--;
                break;
            }
        }
        if (!found) {
            return printError("{\"success\":false,\"message\":\"Bus not found\"}");
        }
        for (int j = 0; j < bookingCount; j++) {
            if (bookings[j].tripID == busID) {
                bookings[j].active = false;
            }
        }
        saveBuses();
        saveBookings();
        return printSuccess("{\"success\":true,\"message\":\"Bus deleted\"}");
    }
    if (action == "admin-trips") {
        string json = "{\"success\":true,\"trips\":[";
        bool first = true;
        for (int i = 0; i < tripCount; i++) {
            if (trips[i].active) {
                 if (!first) json += ",";
                 json += "{\"tripID\":" + to_string(trips[i].tripID) +
                         ",\"busNumber\":\"" + string(trips[i].busNumber) + "\"" +
                         ",\"source\":\"" + string(trips[i].source) + "\"" +
                         ",\"destination\":\"" + string(trips[i].destination) + "\"" +
                         ",\"date\":\"" + string(trips[i].date) + "\"" +
                         ",\"time\":\"" + string(trips[i].time) + "\"" +
                         ",\"fare\":" + to_string(trips[i].fare) +
                         ",\"active\":" + string(trips[i].active ? "true" : "false") + "}";
                 first = false;
            }
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "admin-add-trip") {
        if (argc < 7) {
            return printError("{\"success\":false,\"message\":\"Missing trip arguments\"}");
        }
        string busNumber = argv[2];
        string source = argv[3];
        string dest = argv[4];
        string date = argv[5];
        string timeStr = argv[6];
        int fare = stoi(argv[7]);
        if (tripCount >= MAX_TRIPS) {
            return printError("{\"success\":false,\"message\":\"Trip limit reached\"}");
        }
        int tripID = generateID("trip");
        trips[tripCount].tripID = tripID;
        strcpy(trips[tripCount].busNumber, busNumber.c_str());
        strcpy(trips[tripCount].source, source.c_str());
        strcpy(trips[tripCount].destination, dest.c_str());
        strcpy(trips[tripCount].date, date.c_str());
        strcpy(trips[tripCount].time, timeStr.c_str());
        trips[tripCount].fare = fare;
        int seats = 20; 
        for(int i=0; i<busCount; i++) {
            if (strcasecmp_custom(buses[i].busNumber, busNumber.c_str()) == 0) {
                seats = buses[i].totalSeats;
                break;
            }
        }
        trips[tripCount].maxSeats = seats; 
        trips[tripCount].active = true;
        tripCount++;
        saveTrips();
        string json = "{\"success\":true,\"tripID\":" + to_string(tripID) + "}";
        return printSuccess(json);
    }
    if (action == "admin-delete-trip") {
        if (argc < 3) return printError("{\"success\":false,\"message\":\"Missing trip ID\"}");
        int tripID = stoi(argv[2]);
        for(int i=0; i<tripCount; i++) {
            if(trips[i].tripID == tripID) {
                 for (int j = i; j < tripCount - 1; j++) {
                    trips[j] = trips[j + 1];
                 }
                 tripCount--;
                 saveTrips();
                 return printSuccess("{\"success\":true}");
            }
        }
        return printError("{\"success\":false,\"message\":\"Trip not found\"}");
    }
    if (action == "admin-delete-bus") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing bus ID\"}");
        }
        int busID = stoi(argv[2]);
        for (int i = 0; i < busCount; i++) {
            if (buses[i].busID == busID) {
                buses[i].active = false;
                saveBuses();
                return printSuccess("{\"success\":true}");
            }
        }
        return printError("{\"success\":false,\"message\":\"Bus not found\"}");
    }
    if (action == "admin-toggle-bus") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing bus ID\"}");
        }
        int busID = stoi(argv[2]);
        for (int i = 0; i < busCount; i++) {
            if (buses[i].busID == busID) {
                buses[i].active = !buses[i].active; 
                for (int j = 0; j < bookingCount; j++) {
                    if (bookings[j].tripID == busID) {
                        bookings[j].active = buses[i].active;
                    }
                }
                saveBuses();
                saveBookings();
                string statusStr = buses[i].active ? "true" : "false";
                return printSuccess("{\"success\":true,\"active\":" + statusStr + "}");
            }
        }
        return printError("{\"success\":false,\"message\":\"Bus not found\"}");
    }
    if (action == "get-routes") {
        string json = "{\"success\":true,\"routes\":[";
        bool first = true;
        for (int i = 0; i < routeCount; i++) {
            int fare_est = max(200, routes[i].distanceKM * 2);
            if (!first) json += ",";
            json += "{\"routeID\":" + to_string(routes[i].routeID) +
                    ",\"source\":\"" + string(routes[i].source) +
                    "\",\"destination\":\"" + string(routes[i].destination) +
                    "\",\"fare\":" + to_string(fare_est) + "}";
            first = false;
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "admin-add-route") {
        return printSuccess("{\"success\":true,\"routeID\":0}"); 
    }
    if (action == "admin-delete-route") {
        return printSuccess("{\"success\":true}"); 
    }
    if (action == "admin-add-city") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing city name\"}");
        }
        return printSuccess("{\"success\":true}");
    }
    if (action == "admin-delete-city") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing city name\"}");
        }
        return printSuccess("{\"success\":true}");
    }
    if (action == "reset-password") {
        if (argc < 4) {
            return printError("{\"success\":false,\"message\":\"Missing reset arguments\"}");
        }
        string username = argv[2];
        string newPassword = argv[3];
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found.\"}", 1);
        }
        if (!isPasswordValid(newPassword.c_str())) {
            return printError("{\"success\":false,\"message\":\"Password does not meet requirements.\"}", 1);
        }
        if (strcmp(users[idx].password, newPassword.c_str()) == 0) {
            return printError("{\"success\":false,\"message\":\"New password cannot be the same as your old password.\"}", 1);
        }
        strcpy(users[idx].password, newPassword.c_str());
        users[idx].failedLoginAttempts = 0;
        users[idx].isLocked = false;
        saveUsers();
        return printSuccess("{\"success\":true,\"message\":\"Password reset successfully.\"}");
    }
    if (action == "admin-stats") {
        long long totalRevenue = calculateTotalRevenue();
        long long monthlyRevenue = calculateMonthlyRevenue();
        int activeBuses = countActiveBuses();
        int newBookingsToday = countDailyBookings();
        int totalBookings = countTotalBookings();
        int activeBookings = 0;
        for (int i = 0; i < bookingCount; i++) {
             if (bookings[i].active) activeBookings++;
        }
        string json = "{\"success\":true,\"revenue\":" + to_string(totalRevenue) + 
                      ",\"monthlyRevenue\":" + to_string(monthlyRevenue) +
                      ",\"activeBuses\":" + to_string(activeBuses) +
                      ",\"totalBookings\":" + to_string(totalBookings) +
                      ",\"activeBookings\":" + to_string(activeBookings) +
                      ",\"newToday\":" + to_string(newBookingsToday) +
                      ",\"routes\":" + to_string(routeCount) + "}";
        return printSuccess(json);
    }
    if (action == "admin-count-active-buses") {
        int count = countActiveBuses();
        return printSuccess("{\"success\":true,\"count\":" + to_string(count) + "}");
    }
    if (action == "admin-get-users") {
        string json = "{\"success\":true,\"users\":[";
        bool first = true;
        for (int i = 0; i < userCount; i++) {
            if (!first) json += ",";
            json += "{\"userID\":" + to_string(users[i].userID) +
                    ",\"username\":\"" + string(users[i].username) + "\"" +
                    ",\"name\":\"" + string(users[i].name) + "\"" +
                    ",\"email\":\"" + string(users[i].email) + "\"" +
                    ",\"phone\":\"" + string(users[i].phone) + "\"" +
                    ",\"isAdmin\":" + string(users[i].isAdmin ? "true" : "false") +
                    ",\"isLocked\":" + string(users[i].isLocked ? "true" : "false") + "}";
            first = false;
        }
        json += "]}";
        return printSuccess(json);
    }
    if (action == "admin-toggle-user") {
        if (argc < 4) {
            return printError("{\"success\":false,\"message\":\"Missing arguments\"}");
        }
        string username = argv[2];
        string banStr = argv[3];
        bool shouldBan = (banStr == "1");
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        if (users[idx].isAdmin) {
            return printError("{\"success\":false,\"message\":\"Cannot ban administrator account\"}");
        }
        users[idx].isLocked = shouldBan;
        if (shouldBan) {
            users[idx].failedLoginAttempts = 0;  
        }
        saveUsers();
        string msg = shouldBan ? "User banned successfully" : "User activated successfully";
        return printSuccess("{\"success\":true,\"message\":\"" + msg + "\"}");
    }
    if (action == "admin-delete-user") {
        if (argc < 3) {
            return printError("{\"success\":false,\"message\":\"Missing username\"}");
        }
        string username = argv[2];
        int idx = findUserIndexByUsername(username);
        if (idx < 0) {
            return printError("{\"success\":false,\"message\":\"User not found\"}");
        }
        if (users[idx].isAdmin) {
            return printError("{\"success\":false,\"message\":\"Cannot delete administrator account\"}");
        }
        int userID = users[idx].userID;
        for (int i = idx; i < userCount - 1; i++) {
            users[i] = users[i + 1];
        }
        userCount--;
        saveUsers();
        int newBookingCount = 0;
        for (int i = 0; i < bookingCount; i++) {
            if (bookings[i].userID != userID) {
                bookings[newBookingCount++] = bookings[i];
            }
        }
        if (newBookingCount != bookingCount) {
            bookingCount = newBookingCount;
            saveBookings();
        }
        int newRefundCount = 0;
        for (int i = 0; i < refundCount; i++) {
            if (refunds[i].userID != userID) {
                refunds[newRefundCount++] = refunds[i];
            }
        }
        if (newRefundCount != refundCount) {
            refundCount = newRefundCount;
            saveRefunds();
        }
        return printSuccess("{\"success\":true,\"message\":\"User and associated data deleted successfully\"}");
    }
    if (action == "unknown") {
        return printError("{\"success\":false,\"message\":\"Unknown action\"}");
    }
    return printError("{\"success\":false,\"message\":\"Unknown action\"}");
}
void mainMenu() {
    while (true) {
        cout << "\n========== BUS RESERVATION SYSTEM ==========\n";
        cout << "1. User Login\n";
        cout << "2. User Signup\n";
        cout << "3. Admin Login\n";
        cout << "4. Forgot Username?\n";
        cout << "5. Forgot Password?\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input. Try again.\n";
            continue;
        }
        int userIndex = -1;
        switch (choice) {
            case 1:
                if (login(false, userIndex)) {
                    userMenu(userIndex);
                }
                break;
            case 2:
                signup();
                break;
            case 3:
                if (login(true, userIndex)) {
                    adminMenu(userIndex);
                }
                break;
            case 4:
                resetUsername();
                break;
            case 5:
                resetPassword();
                break;
            case 6:
                cout << "Thank you for using Bus Reservation System!\n";
                return;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
bool login(bool asAdmin, int& userIndex) {
    char username[50], password[50];
    cout << "\n--- " << (asAdmin ? "Admin" : "User") << " Login ---\n";
    cout << "Username: ";
    cin >> username;
    int userIdx = -1;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            userIdx = i;
            break;
        }
    }
    if (userIdx != -1 && users[userIdx].isLocked) {
        cout << "Account locked due to 3 failed login attempts.\n";
        cout << "Would you like to reset password? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Go to main menu and select 'Forgot Password?'\n";
        }
        return false;
    }
    cout << "Password: ";
    cin >> password;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            if (asAdmin && !users[i].isAdmin) {
                cout << "Access denied. Not an admin account.\n";
                return false;
            }
            if (!asAdmin && users[i].isAdmin) {
                cout << "Please use Admin Login.\n";
                return false;
            }
            users[i].failedLoginAttempts = 0;
            saveUsers();
            userIndex = i;
            cout << "Login successful! Welcome, " << users[i].username << "!\n";
            return true;
        }
    }
    if (userIdx != -1) {
        users[userIdx].failedLoginAttempts++;
        if (users[userIdx].failedLoginAttempts >= 3) {
            users[userIdx].isLocked = true;
            cout << "Account locked! 3 failed login attempts.\n";
            cout << "Use 'Forgot Password?' option to unlock your account.\n";
        } else {
            cout << "Invalid password. Attempts remaining: " 
                 << (3 - users[userIdx].failedLoginAttempts) << "\n";
        }
        saveUsers();
    } else {
        cout << "Invalid username or password.\n";
    }
    return false;
}
void signup() {
    if (userCount >= MAX_USERS) {
        cout << "User limit reached.\n";
        return;
    }
    cout << "\n--- User Signup ---\n";
    User newUser;
    newUser.userID = generateID("user");
    cout << "Username: ";
    cin >> newUser.username;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            cout << "Username already exists.\n";
            return;
        }
    }
    cout << "Password (min 6 chars, 1 uppercase, 1 lowercase, 1 number): ";
    while (true) {
        cin >> newUser.password;
        if (strlen(newUser.password) < 6) {
            cout << "Password too short (minimum 6 characters).\n";
            cout << "Password (min 6 chars, 1 uppercase, 1 lowercase, 1 number): ";
            continue;
        }
        if (!isPasswordValid(newUser.password)) {
            cout << "Password must contain at least 1 uppercase, 1 lowercase, and 1 number.\n";
            cout << "Password (min 6 chars, 1 uppercase, 1 lowercase, 1 number): ";
            continue;
        }
        break;
    }
    cout << "Phone (11 digits): ";
    while (true) {
        cin >> newUser.phone;
        if (strlen(newUser.phone) != 11) {
            cout << "Invalid phone number. Enter 11 digits: ";
            continue;
        }
        break;
    }
    cout << "\n--- Security Questions (for account recovery) ---\n";
    cout << "Security Question 1: What is your mother's name? ";
    cin.ignore();
    cin.getline(newUser.secAnswer1, 50);
    strcpy(newUser.secQuestion1, "What is your mother's name?");
    cout << "Security Question 2: What is your birth city? ";
    cin.getline(newUser.secAnswer2, 50);
    strcpy(newUser.secQuestion2, "What is your birth city?");
    newUser.isAdmin = false;
    newUser.failedLoginAttempts = 0;
    newUser.isLocked = false;
    users[userCount++] = newUser;
    saveUsers();
    cout << "\nSignup successful! Your User ID: " << newUser.userID << "\n";
}
void resetPassword() {
    cout << "\n--- Reset Password ---\n";
    char username[50];
    while (true) {
        cout << "Enter your username (0 to go back): ";
        cin >> username;
        if (strcmp(username, "0") == 0) {
            return;
        }
        int userIdx = -1;
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, username) == 0) {
                userIdx = i;
                break;
            }
        }
        if (userIdx == -1) {
            cout << "Username not found. Please try again.\n";
            continue;
        }
        cout << "\n--- Security Question Verification ---\n";
        while (true) {
            cout << users[userIdx].secQuestion1 << " (0 to go back): ";
            char answer1[50];
            cin.ignore();
            cin.getline(answer1, 50);
            if (strcmp(answer1, "0") == 0) {
                return;
            }
            cout << users[userIdx].secQuestion2 << " (0 to go back): ";
            char answer2[50];
            cin.getline(answer2, 50);
            if (strcmp(answer2, "0") == 0) {
                return;
            }
            if (strcasecmp_custom(answer1, users[userIdx].secAnswer1) != 0 || 
                strcasecmp_custom(answer2, users[userIdx].secAnswer2) != 0) {
                cout << "Incorrect answers. Please try again.\n";
                continue;
            }
            break;
        }
        cout << "\n--- Set New Password ---\n";
        char newPassword[50];
        while (true) {
            cout << "Enter new password (min 6 chars, 1 uppercase, 1 lowercase, 1 number): ";
            cin >> newPassword;
            if (strlen(newPassword) < 6) {
                cout << "Password too short (minimum 6 characters).\n";
                continue;
            }
            if (!isPasswordValid(newPassword)) {
                cout << "Password must contain at least 1 uppercase, 1 lowercase, and 1 number.\n";
                continue;
            }
            break;
        }
        strcpy(users[userIdx].password, newPassword);
        users[userIdx].isLocked = false;
        users[userIdx].failedLoginAttempts = 0;
        saveUsers();
        cout << "Password reset successful! You can now login.\n";
        return;
    }
}
void resetUsername() {
    cout << "\n--- Reset Username ---\n";
    char phone[20];
    while (true) {
        cout << "Enter your phone number (0 to go back): ";
        cin >> phone;
        if (strcmp(phone, "0") == 0) {
            return;
        }
        int userIdx = -1;
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].phone, phone) == 0) {
                userIdx = i;
                break;
            }
        }
        if (userIdx == -1) {
            cout << "Phone number not found in system. Please try again.\n";
            continue;
        }
        cout << "\n--- Security Question Verification ---\n";
        while (true) {
            cout << users[userIdx].secQuestion1 << " (0 to go back): ";
            char answer1[50];
            cin.ignore();
            cin.getline(answer1, 50);
            if (strcmp(answer1, "0") == 0) {
                return;
            }
            cout << users[userIdx].secQuestion2 << " (0 to go back): ";
            char answer2[50];
            cin.getline(answer2, 50);
            if (strcmp(answer2, "0") == 0) {
                return;
            }
            if (strcasecmp_custom(answer1, users[userIdx].secAnswer1) != 0 || 
                strcasecmp_custom(answer2, users[userIdx].secAnswer2) != 0) {
                cout << "Incorrect answers. Please try again.\n";
                continue;
            }
            break;
        }
        cout << "\nYour username is: " << users[userIdx].username << "\n";
        cout << "You can now use this username to login.\n";
        return;
    }
}
void userMenu(int userIndex) {
    while (true) {
        cout << "\n--- User Menu (" << users[userIndex].username << ") ---\n";
        cout << "1. Search & Book Bus\n";
        cout << "2. View My Bookings\n";
        cout << "3. Cancel Booking\n";
        cout << "4. View My Refunds\n";
        cout << "5. Change Password\n";
        cout << "6. Logout\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input.\n";
            continue;
        }
        switch (choice) {
            case 1:
                searchAndBookBus(userIndex);
                break;
            case 2:
                viewMyBookings(userIndex);
                break;
            case 3:
                cancelBooking(userIndex);
                break;
            case 4:
                viewMyRefunds(userIndex);
                break;
            case 5:
                changePassword(userIndex);
                break;
            case 6:
                return;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
void searchAndBookBus(int userIndex) {
    cout << "Interactive mode deprecated. Use Web UI.\n";
}
void viewMyBookings(int userIndex) {
    cout << "Interactive mode deprecated. Use Web UI.\n";
}
void cancelBooking(int userIndex) {
    cout << "Interactive mode deprecated. Use Web UI.\n";
}
void viewMyRefunds(int userIndex) {
    cout << "\n--- My Refunds ---\n";
    bool found = false;
    for (int i = 0; i < refundCount; i++) {
        if (refunds[i].userID == users[userIndex].userID) {
            found = true;
            cout << "\nRefund ID: " << refunds[i].refundID << "\n";
            cout << "Booking ID: " << refunds[i].bookingID << "\n";
            cout << "Amount: Rs. " << refunds[i].amount << "\n";
            cout << "Refund Method: " << refunds[i].accountType << "\n";
            cout << "Account: " << refunds[i].accountNumber << "\n";
            cout << "Status: " << refunds[i].status << "\n";
            cout << "Processed Date: " << refunds[i].processedDate << "\n";
            cout << string(40, '-') << "\n";
        }
    }
    if (!found) {
        cout << "No refunds found.\n";
    }
}
void changePassword(int userIndex) {
    cout << "\n--- Change Password ---\n";
    char currentPassword[50];
    while (true) {
        cout << "Enter current password (0 to return to main menu): ";
        cin >> currentPassword;
        if (strcmp(currentPassword, "0") == 0) {
            cout << "Returning to main menu.\n";
            return;
        }
        if (strcmp(users[userIndex].password, currentPassword) == 0) {
            break;
        }
        cout << "Incorrect password. Try again or enter 0 to go back to main menu.\n";
    }
    char newPassword[50];
    while (true) {
        cout << "Enter new password (min 6 chars, 1 uppercase, 1 lowercase, 1 number): ";
        cin >> newPassword;
        if (strlen(newPassword) < 6) {
            cout << "Password too short (minimum 6 characters).\n";
            continue;
        }
        if (!isPasswordValid(newPassword)) {
            cout << "Password must contain at least 1 uppercase, 1 lowercase, and 1 number.\n";
            continue;
        }
        break;
    }
    char confirmPassword[50];
    cout << "Confirm new password: ";
    cin >> confirmPassword;
    if (strcmp(newPassword, confirmPassword) != 0) {
        cout << "Passwords do not match.\n";
        return;
    }
    strcpy(users[userIndex].password, newPassword);
    saveUsers();
    cout << "Password changed successfully!\n";
}
void adminMenu(int userIndex) {
    cout << "Admin menu deprecated. Use Web UI.\n";
    return;
}
void adminAddBus() {
    cout << "Use Web UI.\n";
}
void adminViewBuses() {
    cout << "Use Web UI.\n";
}
void adminUpdateBus() {
    cout << "Use Web UI.\n";
}
void adminDeleteBus() {
    cout << "Use Web UI.\n";
}
void loadSeatsForBus(int tripID, SeatInfo seats[]) {
    for (int i = 0; i < SEATS_PER_BUS; i++) {
        seats[i].seatNumber = i + 1;
        seats[i].isBooked = false;
        seats[i].gender = ' ';
        seats[i].userID = 0;
    }
    stringstream filename;
    filename << "trip_" << tripID << "_seats.txt";
    ifstream file(filename.str().c_str());
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        string tokens[10];
        int count = 0;
        splitString(line, '|', tokens, count);
        if (count >= 4) {
            int seatNum = stoi(tokens[0]);
            if (seatNum > 0 && seatNum <= SEATS_PER_BUS) {
                seats[seatNum - 1].isBooked = (tokens[1] == "1");
                seats[seatNum - 1].gender = tokens[2][0];
                seats[seatNum - 1].userID = stoi(tokens[3]);
            }
        }
    }
    file.close();
}
void saveSeatsForBus(int tripID, SeatInfo seats[]) {
    stringstream filename;
    filename << "trip_" << tripID << "_seats.txt";
    ofstream file(filename.str().c_str());
    for (int i = 0; i < SEATS_PER_BUS; i++) {
        file << seats[i].seatNumber << "|"
             << (seats[i].isBooked ? 1 : 0) << "|"
             << seats[i].gender << "|"
             << seats[i].userID << "\n";
    }
    file.close();
}
bool processPayment(int fare) {
    cout << "\n--- Payment ---\n";
    cout << "Total Fare: Rs. " << fare << "\n";
    int method;
    while (true) {
        cout << "\nPayment Methods:\n";
        cout << "1. JazzCash\n";
        cout << "2. EasyPaisa\n";
        cout << "3. Credit/Debit Card\n";
        cout << "0. Cancel\n";
        cout << "Select method: ";
        cin >> method;
        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        if (method == 0) {
            cout << "Payment cancelled.\n";
            return false;
        }
        if (method >= 1 && method <= 3) {
            break;
        }
        cout << "Invalid choice. Please select 1-3 or 0 to cancel.\n";
    }
    if (method == 1 || method == 2) {
        char phone[20];
        while (true) {
            cout << "Enter phone number (11 digits): ";
            cin >> phone;
            if (strlen(phone) == 11) {
                break;
            }
            cout << "Invalid phone number. Must be exactly 11 digits.\n";
        }
        cout << "Payment of Rs. " << fare << " processed via " 
             << (method == 1 ? "JazzCash" : "EasyPaisa") << ".\n";
        return true;
    } else if (method == 3) {
        char cardNum[20];
        while (true) {
            cout << "Enter card number (16 digits): ";
            cin >> cardNum;
            if (strlen(cardNum) == 16) {
                break;
            }
            cout << "Invalid card number. Must be exactly 16 digits.\n";
        }
        cout << "Payment of Rs. " << fare << " processed via Card.\n";
        return true;
    }
    return false;
}
