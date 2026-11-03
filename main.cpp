// ============================================================
//  SMART COLLEGE BUS MANAGEMENT SYSTEM
//  Language  : C++
//  Concept   : Object-Oriented Programming (OOP)
//  Storage   : File Handling (data/*.txt)
//
//  OOP Concepts demonstrated:
//    - Classes & Objects
//    - Encapsulation  (private members + getters/setters)
//    - Inheritance    (Student, Driver, Admin  <-  User)
//    - Polymorphism   (virtual login / displayDashboard)
//    - Abstraction    (pure virtual displayDashboard)
//    - Constructors
//    - File Handling
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cstdlib>

using namespace std;

// ============================================================
//  FILE PATH CONSTANTS
// ============================================================
const string STUDENTS_FILE  = "data/students.txt";
const string BUSES_FILE     = "data/buses.txt";
const string ROUTES_FILE    = "data/routes.txt";
const string DRIVERS_FILE   = "data/drivers.txt";
const string SEATS_FILE     = "data/seats.txt";
const string SCHEDULES_FILE = "data/schedules.txt";
const string WAITING_FILE   = "data/waiting.txt";

// ============================================================
//  HELPER UTILITIES
// ============================================================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    cin.get();
}

void printLine(char c = '-', int w = 68) {
    cout << string(w, c) << "\n";
}

void printTitle(const string& title, int w = 68) {
    printLine('=', w);
    int pad = (w - (int)title.length()) / 2;
    if (pad > 0) cout << string(pad, ' ');
    cout << title << "\n";
    printLine('=', w);
}

void printHeader(const string& title) {
    cout << "\n";
    printTitle(title);
    cout << "\n";
}

// Split a string by delimiter
vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim))
        tokens.push_back(token);
    return tokens;
}

// Join vector elements with a delimiter string
string joinVec(const vector<string>& v, const string& delim) {
    string res;
    for (size_t i = 0; i < v.size(); i++) {
        res += v[i];
        if (i + 1 < v.size()) res += delim;
    }
    return res;
}

// Trim leading/trailing spaces
string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Safe integer input with validation
int getIntInput(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(); return val; }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Please enter a valid number.\n";
    }
}

// ============================================================
//  CLASS: Bus
// ============================================================
class Bus {
private:
    string busNumber;
    int    capacity;
    string routeId;
    string driverId;
    string status;       // "Not Started" | "Running" | "Completed"
    string scheduleType; // "Full Day"    | "Half Day"

public:
    // ---- Constructors ----
    Bus() : busNumber(""), capacity(0), routeId("-"), driverId("-"),
            status("Not Started"), scheduleType("Full Day") {}

    Bus(string bn, int cap, string rid, string did, string st, string scht)
        : busNumber(bn), capacity(cap), routeId(rid), driverId(did),
          status(st), scheduleType(scht) {}

    // ---- Getters (Encapsulation) ----
    string getBusNumber()    const { return busNumber; }
    int    getCapacity()     const { return capacity; }
    string getRouteId()      const { return routeId; }
    string getDriverId()     const { return driverId; }
    string getStatus()       const { return status; }
    string getScheduleType() const { return scheduleType; }

    // ---- Setters (Encapsulation) ----
    void setBusNumber(const string& v)    { busNumber = v; }
    void setCapacity(int v)               { capacity = v; }
    void setRouteId(const string& v)      { routeId = v; }
    void setDriverId(const string& v)     { driverId = v; }
    void setStatus(const string& v)       { status = v; }
    void setScheduleType(const string& v) { scheduleType = v; }

    // ---- Display ----
    void display() const {
        cout << left
             << setw(12) << busNumber
             << setw(10) << capacity
             << setw(10) << routeId
             << setw(12) << driverId
             << setw(15) << status
             << scheduleType << "\n";
    }

    // ---- File Serialisation ----
    string toFileString() const {
        return busNumber + "|" + to_string(capacity) + "|" +
               routeId  + "|" + driverId + "|" + status + "|" + scheduleType;
    }

    static Bus fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() >= 6)
            return Bus(p[0], stoi(p[1]), p[2], p[3], p[4], p[5]);
        return Bus();
    }

    // ---- Static File Operations ----
    static vector<Bus> loadAll() {
        vector<Bus> v;
        ifstream f(BUSES_FILE);
        string line;
        while (getline(f, line))
            if (!trim(line).empty()) v.push_back(fromFileString(line));
        return v;
    }

    static void saveAll(const vector<Bus>& v) {
        ofstream f(BUSES_FILE);
        for (auto& b : v) f << b.toFileString() << "\n";
    }

    static Bus* findByNumber(vector<Bus>& v, const string& num) {
        for (auto& b : v) if (b.getBusNumber() == num) return &b;
        return nullptr;
    }
};

// ============================================================
//  CLASS: Route
// ============================================================
class Route {
private:
    string         routeId;
    string         routeName;
    vector<string> stops;
    string         timing;

public:
    Route() {}
    Route(string rid, string rn, vector<string> s, string t)
        : routeId(rid), routeName(rn), stops(s), timing(t) {}

    string         getRouteId()   const { return routeId; }
    string         getRouteName() const { return routeName; }
    vector<string> getStops()     const { return stops; }
    string         getTiming()    const { return timing; }

    void setRouteId(const string& v)         { routeId = v; }
    void setRouteName(const string& v)       { routeName = v; }
    void setStops(const vector<string>& v)   { stops = v; }
    void setTiming(const string& v)          { timing = v; }

    void display() const {
        cout << "  Route ID   : " << routeId   << "\n";
        cout << "  Route Name : " << routeName  << "\n";
        cout << "  Timing     : " << timing     << "\n";
        cout << "  Stops      : ";
        for (size_t i = 0; i < stops.size(); i++) {
            cout << stops[i];
            if (i + 1 < stops.size()) cout << " -> ";
        }
        cout << "\n";
    }

    string toFileString() const {
        return routeId + "|" + routeName + "|" + joinVec(stops, ",") + "|" + timing;
    }

    static Route fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() >= 4)
            return Route(p[0], p[1], split(p[2], ','), p[3]);
        return Route();
    }

    static vector<Route> loadAll() {
        vector<Route> v;
        ifstream f(ROUTES_FILE);
        string line;
        while (getline(f, line))
            if (!trim(line).empty()) v.push_back(fromFileString(line));
        return v;
    }

    static void saveAll(const vector<Route>& v) {
        ofstream f(ROUTES_FILE);
        for (auto& r : v) f << r.toFileString() << "\n";
    }

    static Route* findById(vector<Route>& v, const string& id) {
        for (auto& r : v) if (r.getRouteId() == id) return &r;
        return nullptr;
    }
};

// ============================================================
//  CLASS: Schedule
// ============================================================
class Schedule {
private:
    string scheduleId;
    string busId;
    string scheduleType;  // "Full Day" | "Half Day"
    string departureTime;
    string arrivalTime;

public:
    Schedule() {}
    Schedule(string sid, string bid, string stype, string dep, string arr)
        : scheduleId(sid), busId(bid), scheduleType(stype),
          departureTime(dep), arrivalTime(arr) {}

    string getScheduleId()    const { return scheduleId; }
    string getBusId()         const { return busId; }
    string getScheduleType()  const { return scheduleType; }
    string getDepartureTime() const { return departureTime; }
    string getArrivalTime()   const { return arrivalTime; }

    void display() const {
        cout << left
             << setw(12) << scheduleId
             << setw(10) << busId
             << setw(12) << scheduleType
             << setw(14) << departureTime
             << arrivalTime << "\n";
    }

    string toFileString() const {
        return scheduleId + "|" + busId + "|" + scheduleType + "|" +
               departureTime + "|" + arrivalTime;
    }

    static Schedule fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() >= 5)
            return Schedule(p[0], p[1], p[2], p[3], p[4]);
        return Schedule();
    }

    static vector<Schedule> loadAll() {
        vector<Schedule> v;
        ifstream f(SCHEDULES_FILE);
        string line;
        while (getline(f, line))
            if (!trim(line).empty()) v.push_back(fromFileString(line));
        return v;
    }

    static void saveAll(const vector<Schedule>& v) {
        ofstream f(SCHEDULES_FILE);
        for (auto& s : v) f << s.toFileString() << "\n";
    }
};

// ============================================================
//  CLASS: Seat
// ============================================================
class Seat {
private:
    string seatId;
    string busId;
    int    seatNumber;
    string seatType;   // "Window" | "Front" | "Other"
    string status;     // "Available" | "Booked"
    string studentId;  // "-" when free

public:
    Seat() : seatId(""), busId(""), seatNumber(0),
             seatType("Other"), status("Available"), studentId("-") {}

    Seat(string sid, string bid, int snum, string stype, string stat, string stid)
        : seatId(sid), busId(bid), seatNumber(snum),
          seatType(stype), status(stat), studentId(stid) {}

    string getSeatId()    const { return seatId; }
    string getBusId()     const { return busId; }
    int    getSeatNumber()const { return seatNumber; }
    string getSeatType()  const { return seatType; }
    string getStatus()    const { return status; }
    string getStudentId() const { return studentId; }

    void setSeatId(const string& v)    { seatId = v; }
    void setBusId(const string& v)     { busId = v; }
    void setSeatNumber(int v)          { seatNumber = v; }
    void setSeatType(const string& v)  { seatType = v; }
    void setStatus(const string& v)    { status = v; }
    void setStudentId(const string& v) { studentId = v; }

    bool isAvailable() const { return status == "Available"; }

    void display() const {
        cout << left
             << setw(12) << seatId
             << setw(8)  << seatNumber
             << setw(10) << seatType
             << setw(12) << status;
        if (studentId != "-") cout << studentId;
        cout << "\n";
    }

    string toFileString() const {
        return seatId + "|" + busId + "|" + to_string(seatNumber) + "|" +
               seatType + "|" + status + "|" + studentId;
    }

    static Seat fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() >= 6)
            return Seat(p[0], p[1], stoi(p[2]), p[3], p[4], p[5]);
        return Seat();
    }

    static vector<Seat> loadAll() {
        vector<Seat> v;
        ifstream f(SEATS_FILE);
        string line;
        while (getline(f, line))
            if (!trim(line).empty()) v.push_back(fromFileString(line));
        return v;
    }

    static void saveAll(const vector<Seat>& v) {
        ofstream f(SEATS_FILE);
        for (auto& s : v) f << s.toFileString() << "\n";
    }
};

// ============================================================
//  CLASS: WaitingList
// ============================================================
class WaitingList {
public:
    struct WaitEntry {
        string studentId;
        string busId;
        string preference;
        string toFileString() const {
            return studentId + "|" + busId + "|" + preference;
        }
    };

private:
    vector<WaitEntry> list;

public:
    WaitingList() { load(); }

    void load() {
        list.clear();
        ifstream f(WAITING_FILE);
        string line;
        while (getline(f, line)) {
            if (trim(line).empty()) continue;
            vector<string> p = split(line, '|');
            if (p.size() >= 3)
                list.push_back({p[0], p[1], p[2]});
        }
    }

    void save() {
        ofstream f(WAITING_FILE);
        for (auto& e : list) f << e.toFileString() << "\n";
    }

    void add(const string& sid, const string& bid, const string& pref) {
        list.push_back({sid, bid, pref});
        save();
        cout << "  Added to waiting list successfully!\n";
    }

    bool isInList(const string& sid) const {
        for (auto& e : list) if (e.studentId == sid) return true;
        return false;
    }

    void remove(const string& sid) {
        list.erase(
            remove_if(list.begin(), list.end(),
                [&](const WaitEntry& e){ return e.studentId == sid; }),
            list.end());
        save();
    }

    WaitEntry* getFirstForBus(const string& bid) {
        for (auto& e : list) if (e.busId == bid) return &e;
        return nullptr;
    }

    int size() const { return (int)list.size(); }

    void display() const {
        if (list.empty()) { cout << "  No students in waiting list.\n"; return; }
        cout << left << setw(16) << "Student ID" << setw(12) << "Bus" << "Preference\n";
        printLine();
        for (auto& e : list)
            cout << left << setw(16) << e.studentId
                 << setw(12) << e.busId
                 << e.preference << "\n";
    }
};

// ============================================================
//  CLASS: User  (Base class — Abstraction + Polymorphism)
// ============================================================
class User {
protected:
    string name;
    string id;
    string password;
    string role;

public:
    User() : name(""), id(""), password(""), role("") {}
    User(string n, string i, string p, string r)
        : name(n), id(i), password(p), role(r) {}

    virtual ~User() {}

    // Getters
    string getName()     const { return name; }
    string getId()       const { return id; }
    string getPassword() const { return password; }
    string getRole()     const { return role; }

    // Setters
    void setName(const string& v)     { name = v; }
    void setId(const string& v)       { id = v; }
    void setPassword(const string& v) { password = v; }

    // Polymorphism: each subclass can override
    virtual bool login(const string& eid, const string& ep) {
        return (id == eid && password == ep);
    }

    // Pure virtual — Abstraction (must be implemented by subclass)
    virtual void displayDashboard() = 0;
};

// ============================================================
//  CLASS: Student  (Inherits User — Inheritance)
// ============================================================
class Student : public User {
private:
    string busId;       // assigned bus ("-" = none)
    string seatId;      // assigned seat ("-" = none)
    string preference;  // "Window" | "Front" | "Any"

public:
    Student()
        : User("", "", "", "Student"), busId("-"), seatId("-"), preference("Any") {}

    Student(string n, string i, string p, string bid, string sid, string pref)
        : User(n, i, p, "Student"), busId(bid), seatId(sid), preference(pref) {}

    string getBusId()      const { return busId; }
    string getSeatId()     const { return seatId; }
    string getPreference() const { return preference; }

    void setBusId(const string& v)      { busId = v; }
    void setSeatId(const string& v)     { seatId = v; }
    void setPreference(const string& v) { preference = v; }

    bool hasAssignedSeat() const { return seatId != "-" && !seatId.empty(); }
    bool hasAssignedBus()  const { return busId  != "-" && !busId.empty(); }

    // Override: Polymorphism
    void displayDashboard() override {
        clearScreen();
        printHeader("STUDENT DASHBOARD");
        cout << "  Name        : " << name << "\n";
        cout << "  Student ID  : " << id   << "\n";
        cout << "  Bus         : " << (hasAssignedBus()  ? busId  : "Not Assigned") << "\n";
        cout << "  Seat        : " << (hasAssignedSeat() ? seatId : "Not Assigned") << "\n";
        cout << "  Preference  : " << preference << "\n";
        printLine();
    }

    string toFileString() const {
        return id + "|" + name + "|" + password + "|" +
               (busId.empty()  ? "-" : busId)  + "|" +
               (seatId.empty() ? "-" : seatId) + "|" + preference;
    }

    static Student fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() >= 6)
            return Student(p[1], p[0], p[2], p[3], p[4], p[5]);
        return Student();
    }

    static vector<Student> loadAll() {
        vector<Student> v;
        ifstream f(STUDENTS_FILE);
        string line;
        while (getline(f, line))
            if (!trim(line).empty()) v.push_back(fromFileString(line));
        return v;
    }

    static void saveAll(const vector<Student>& v) {
        ofstream f(STUDENTS_FILE);
        for (auto& s : v) f << s.toFileString() << "\n";
    }

    static Student* findById(vector<Student>& v, const string& sid) {
        for (auto& s : v) if (s.getId() == sid) return &s;
        return nullptr;
    }
};

// ============================================================
//  CLASS: Driver  (Inherits User — Inheritance)
// ============================================================
class Driver : public User {
private:
    string licenseNo;
    string contact;
    string assignedBusId;  // "-" = none

public:
    Driver()
        : User("", "", "", "Driver"), licenseNo(""), contact(""), assignedBusId("-") {}

    Driver(string n, string i, string p, string lic, string cont, string bid)
        : User(n, i, p, "Driver"), licenseNo(lic), contact(cont), assignedBusId(bid) {}

    string getLicenseNo()     const { return licenseNo; }
    string getContact()       const { return contact; }
    string getAssignedBusId() const { return assignedBusId; }

    void setLicenseNo(const string& v)     { licenseNo = v; }
    void setContact(const string& v)       { contact = v; }
    void setAssignedBusId(const string& v) { assignedBusId = v; }

    bool hasBusAssigned() const { return assignedBusId != "-" && !assignedBusId.empty(); }

    // Override: Polymorphism
    void displayDashboard() override {
        clearScreen();
        printHeader("DRIVER DASHBOARD");
        cout << "  Name         : " << name << "\n";
        cout << "  Driver ID    : " << id   << "\n";
        cout << "  License No.  : " << licenseNo << "\n";
        cout << "  Contact      : " << contact   << "\n";
        cout << "  Assigned Bus : "
             << (hasBusAssigned() ? assignedBusId : "Not Assigned") << "\n";
        printLine();
    }

    string toFileString() const {
        return id + "|" + name + "|" + password + "|" +
               licenseNo + "|" + contact + "|" +
               (assignedBusId.empty() ? "-" : assignedBusId);
    }

    static Driver fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() >= 6)
            return Driver(p[1], p[0], p[2], p[3], p[4], p[5]);
        return Driver();
    }

    static vector<Driver> loadAll() {
        vector<Driver> v;
        ifstream f(DRIVERS_FILE);
        string line;
        while (getline(f, line))
            if (!trim(line).empty()) v.push_back(fromFileString(line));
        return v;
    }

    static void saveAll(const vector<Driver>& v) {
        ofstream f(DRIVERS_FILE);
        for (auto& d : v) f << d.toFileString() << "\n";
    }

    static Driver* findById(vector<Driver>& v, const string& did) {
        for (auto& d : v) if (d.getId() == did) return &d;
        return nullptr;
    }
};

// ============================================================
//  CLASS: Admin  (Inherits User — Inheritance)
// ============================================================
class Admin : public User {
public:
    Admin() : User("Administrator", "admin", "admin123", "Admin") {}
    Admin(string n, string i, string p) : User(n, i, p, "Admin") {}

    // Override: Polymorphism
    void displayDashboard() override {
        clearScreen();
        printHeader("ADMIN DASHBOARD");
        cout << "  Welcome, Administrator!\n";
        cout << "  Admin ID : " << id << "\n";
        printLine();
    }
};

// ============================================================
//  CLASS: SeatAllocation  (Encapsulates booking logic)
// ============================================================
class SeatAllocation {
private:
    WaitingList waitingList;

    // Internal: try to find a seat matching preference on a bus
    Seat* findBestSeat(vector<Seat>& seats, const string& busId, const string& pref) {
        // First pass: exact preference match
        for (auto& s : seats)
            if (s.getBusId() == busId && s.isAvailable())
                if (pref == "Any" ||
                    (pref == "Window" && s.getSeatType() == "Window") ||
                    (pref == "Front"  && s.getSeatType() == "Front"))
                    return &s;

        // Second pass: any available seat (preference not met)
        if (pref != "Any")
            for (auto& s : seats)
                if (s.getBusId() == busId && s.isAvailable())
                    return &s;

        return nullptr;
    }

public:
    SeatAllocation() {}

    // ---- Allocate a seat to a student ----
    bool allocateSeat(const string& studentId, const string& busId,
                      const string& preference) {
        vector<Seat>    seats    = Seat::loadAll();
        vector<Student> students = Student::loadAll();

        Student* stu = Student::findById(students, studentId);
        if (!stu) { cout << "  [!] Student not found!\n"; return false; }

        if (stu->hasAssignedSeat()) {
            cout << "  [!] Already has seat: " << stu->getSeatId() << "\n";
            return false;
        }

        Seat* chosen = findBestSeat(seats, busId, preference);

        if (!chosen) {
            cout << "  [!] No available seats on Bus " << busId << ".\n";
            if (!waitingList.isInList(studentId)) {
                char ch;
                cout << "  Join waiting list? (y/n): ";
                cin >> ch; cin.ignore();
                if (ch == 'y' || ch == 'Y') {
                    stu->setBusId(busId);
                    stu->setPreference(preference);
                    Student::saveAll(students);
                    waitingList.add(studentId, busId, preference);
                }
            } else {
                cout << "  Already in waiting list.\n";
            }
            return false;
        }

        // Book the seat
        chosen->setStatus("Booked");
        chosen->setStudentId(studentId);
        stu->setSeatId(chosen->getSeatId());
        stu->setBusId(busId);
        stu->setPreference(preference);

        Seat::saveAll(seats);
        Student::saveAll(students);

        cout << "  [OK] Seat " << chosen->getSeatId()
             << " (Seat #" << chosen->getSeatNumber()
             << " | " << chosen->getSeatType()
             << ") allocated to " << studentId << "!\n";
        return true;
    }

    // ---- Cancel a seat and auto-allocate to next on waiting list ----
    bool cancelSeat(const string& studentId) {
        vector<Seat>    seats    = Seat::loadAll();
        vector<Student> students = Student::loadAll();

        Student* stu = Student::findById(students, studentId);
        if (!stu) { cout << "  [!] Student not found!\n"; return false; }

        if (!stu->hasAssignedSeat()) {
            cout << "  [!] No assigned seat to cancel.\n";
            return false;
        }

        string oldSeatId = stu->getSeatId();
        string busId     = stu->getBusId();

        // Free the seat
        for (auto& s : seats) {
            if (s.getSeatId() == oldSeatId) {
                s.setStatus("Available");
                s.setStudentId("-");
                break;
            }
        }
        stu->setSeatId("-");

        Seat::saveAll(seats);
        Student::saveAll(students);
        cout << "  [OK] Seat " << oldSeatId << " cancelled.\n";

        // Auto-allocate to first waiting student on same bus
        WaitingList wl;
        WaitingList::WaitEntry* next = wl.getFirstForBus(busId);
        if (next) {
            string wSid  = next->studentId;
            string wPref = next->preference;
            cout << "\n  Auto-allocating to waiting student: " << wSid << " ...\n";
            wl.remove(wSid);
            allocateSeat(wSid, busId, wPref);
        }
        return true;
    }

    // ---- Display available seats for a bus ----
    void displayAvailableSeats(const string& busId) const {
        vector<Seat> seats = Seat::loadAll();
        bool found = false;
        cout << "\n  Available Seats on Bus " << busId << ":\n";
        cout << left << setw(12) << "  Seat ID" << setw(8) << "Seat#" << "Type\n";
        printLine();
        for (auto& s : seats)
            if (s.getBusId() == busId && s.isAvailable()) {
                cout << "  " << left << setw(12) << s.getSeatId()
                     << setw(8) << s.getSeatNumber()
                     << s.getSeatType() << "\n";
                found = true;
            }
        if (!found) cout << "  No available seats.\n";
    }

    // ---- Display all seats (booked + available) for a bus ----
    void displayAllSeats(const string& busId) const {
        vector<Seat> seats = Seat::loadAll();
        bool found = false;
        cout << "\n  All Seats on Bus " << busId << ":\n";
        cout << left << setw(12) << "  Seat ID" << setw(8) << "Seat#"
             << setw(10) << "Type" << setw(12) << "Status" << "Student\n";
        printLine();
        for (auto& s : seats)
            if (s.getBusId() == busId) {
                cout << "  ";
                s.display();
                found = true;
            }
        if (!found) cout << "  No seats defined for this bus.\n";
    }

    // ---- Occupancy statistics ----
    void displayOccupancy(const string& busId) const {
        vector<Seat> seats = Seat::loadAll();
        int total = 0, booked = 0;
        for (auto& s : seats)
            if (s.getBusId() == busId) { total++; if (!s.isAvailable()) booked++; }

        cout << "\n  Occupancy — Bus " << busId << ":\n";
        cout << "    Total Seats    : " << total           << "\n";
        cout << "    Booked Seats   : " << booked          << "\n";
        cout << "    Available      : " << (total - booked) << "\n";
        if (total > 0)
            cout << "    Occupancy %    : "
                 << fixed << setprecision(1)
                 << (double)booked / total * 100.0 << "%\n";
    }

    // ---- Show waiting list ----
    void displayWaitingList() {
        printHeader("WAITING LIST");
        waitingList.display();
    }
};

// ============================================================
//  CLASS: Report  (Abstraction — hides generation logic)
// ============================================================
class Report {
public:
    // ---- 1. Daily Bus Report ----
    void generateDailyBusReport() const {
        clearScreen();
        printHeader("DAILY BUS REPORT");

        vector<Bus>   buses  = Bus::loadAll();
        vector<Route> routes = Route::loadAll();

        if (buses.empty()) { cout << "  No buses found.\n"; return; }

        cout << left << setw(12) << "Bus No."   << setw(10) << "Capacity"
             << setw(10) << "Route"  << setw(12) << "Driver"
             << setw(15) << "Status" << "Schedule\n";
        printLine();
        for (auto& b : buses) b.display();
        cout << "\n  Total Buses: " << buses.size() << "\n";
    }

    // ---- 2. Occupancy Report ----
    void generateOccupancyReport() const {
        clearScreen();
        printHeader("OCCUPANCY REPORT");

        vector<Bus>  buses = Bus::loadAll();
        vector<Seat> seats = Seat::loadAll();

        int gTotal = 0, gBooked = 0;
        cout << left << setw(12) << "Bus No." << setw(10) << "Total"
             << setw(10) << "Booked" << setw(12) << "Available" << "Occupancy%\n";
        printLine();

        for (auto& bus : buses) {
            int total = 0, booked = 0;
            for (auto& s : seats)
                if (s.getBusId() == bus.getBusNumber())
                    { total++; if (!s.isAvailable()) booked++; }

            double pct = total > 0 ? (double)booked / total * 100.0 : 0.0;
            cout << left << setw(12) << bus.getBusNumber()
                 << setw(10) << total
                 << setw(10) << booked
                 << setw(12) << (total - booked)
                 << fixed << setprecision(1) << pct << "%\n";
            gTotal += total; gBooked += booked;
        }
        printLine();
        double gPct = gTotal > 0 ? (double)gBooked / gTotal * 100.0 : 0.0;
        cout << left << setw(12) << "TOTAL"
             << setw(10) << gTotal
             << setw(10) << gBooked
             << setw(12) << (gTotal - gBooked)
             << fixed << setprecision(1) << gPct << "%\n";
    }

    // ---- 3. Route-Wise Report ----
    void generateRouteWiseReport() const {
        clearScreen();
        printHeader("ROUTE-WISE REPORT");

        vector<Route>   routes   = Route::loadAll();
        vector<Bus>     buses    = Bus::loadAll();
        vector<Student> students = Student::loadAll();

        if (routes.empty()) { cout << "  No routes found.\n"; return; }

        for (auto& route : routes) {
            cout << "\n";
            printLine('-', 50);
            route.display();
            cout << "\n  Buses on this route:\n";
            bool busFound = false;
            for (auto& bus : buses) {
                if (bus.getRouteId() == route.getRouteId()) {
                    int sc = 0;
                    for (auto& st : students)
                        if (st.getBusId() == bus.getBusNumber()) sc++;
                    cout << "    Bus " << bus.getBusNumber()
                         << " | Schedule: " << bus.getScheduleType()
                         << " | Status: "   << bus.getStatus()
                         << " | Students: " << sc << "\n";
                    busFound = true;
                }
            }
            if (!busFound) cout << "    No buses on this route.\n";
        }
    }

    // ---- 4. Full Day / Half Day Report ----
    void generateFullHalfDayReport() const {
        clearScreen();
        printHeader("FULL DAY / HALF DAY REPORT");

        vector<Bus> buses = Bus::loadAll();
        int full = 0, half = 0;

        auto printBuses = [&](const string& type) {
            cout << "\n  --- " << type << " BUSES ---\n";
            cout << left << setw(12) << "Bus No." << setw(15) << "Route"
                 << setw(12) << "Driver" << "Status\n";
            printLine('-', 50);
            for (auto& b : buses)
                if (b.getScheduleType() == type) {
                    cout << left << setw(12) << b.getBusNumber()
                         << setw(15) << b.getRouteId()
                         << setw(12) << b.getDriverId()
                         << b.getStatus() << "\n";
                    if (type == "Full Day") full++; else half++;
                }
        };

        printBuses("Full Day");
        printBuses("Half Day");

        cout << "\n  Summary:\n";
        cout << "    Full Day Buses : " << full << "\n";
        cout << "    Half Day Buses : " << half << "\n";
    }

    // ---- 5. Driver Report ----
    void generateDriverReport() const {
        clearScreen();
        printHeader("DRIVER REPORT");

        vector<Driver> drivers = Driver::loadAll();
        vector<Bus>    buses   = Bus::loadAll();
        vector<Route>  routes  = Route::loadAll();

        if (drivers.empty()) { cout << "  No drivers found.\n"; return; }

        cout << left << setw(12) << "Driver ID" << setw(18) << "Name"
             << setw(14) << "License" << setw(10) << "Bus"
             << setw(12) << "Route"   << "Bus Status\n";
        printLine();

        for (auto& d : drivers) {
            string bNum = d.hasBusAssigned() ? d.getAssignedBusId() : "-";
            string rid  = "-", bStat = "-";
            if (d.hasBusAssigned())
                for (auto& b : buses)
                    if (b.getBusNumber() == d.getAssignedBusId())
                        { rid = b.getRouteId(); bStat = b.getStatus(); break; }

            cout << left << setw(12) << d.getId()
                 << setw(18) << d.getName()
                 << setw(14) << d.getLicenseNo()
                 << setw(10) << bNum
                 << setw(12) << rid
                 << bStat << "\n";
        }
        cout << "\n  Total Drivers: " << drivers.size() << "\n";
    }

    // ---- 6. Student Seat Report ----
    void generateStudentSeatReport() const {
        clearScreen();
        printHeader("STUDENT SEAT REPORT");

        vector<Student> students = Student::loadAll();
        vector<Seat>    seats    = Seat::loadAll();

        if (students.empty()) { cout << "  No students found.\n"; return; }

        cout << left << setw(14) << "Student ID" << setw(18) << "Name"
             << setw(10) << "Bus"    << setw(12) << "Seat ID"
             << setw(8)  << "Seat#"  << setw(10) << "Type" << "Preference\n";
        printLine();

        for (auto& stu : students) {
            string sNum = "-", sType = "-";
            if (stu.hasAssignedSeat())
                for (auto& s : seats)
                    if (s.getSeatId() == stu.getSeatId())
                        { sNum = to_string(s.getSeatNumber()); sType = s.getSeatType(); break; }

            cout << left << setw(14) << stu.getId()
                 << setw(18) << stu.getName()
                 << setw(10) << stu.getBusId()
                 << setw(12) << stu.getSeatId()
                 << setw(8)  << sNum
                 << setw(10) << sType
                 << stu.getPreference() << "\n";
        }
        cout << "\n  Total Students: " << students.size() << "\n";
    }
};

// ============================================================
//  MODULE 1 — STUDENT MENU FUNCTIONS
// ============================================================

void studentViewBuses() {
    clearScreen();
    printHeader("AVAILABLE BUSES");
    vector<Bus> buses = Bus::loadAll();
    if (buses.empty()) { cout << "  No buses available.\n"; return; }

    cout << left << setw(12) << "Bus No."  << setw(10) << "Capacity"
         << setw(12) << "Route" << setw(15) << "Status" << "Schedule\n";
    printLine();
    for (auto& b : buses) b.display();
    cout << "\n  Total: " << buses.size() << "\n";
}

void studentViewRoutes() {
    clearScreen();
    printHeader("ROUTES, STOPS & TIMINGS");
    vector<Route>    routes    = Route::loadAll();
    vector<Bus>      buses     = Bus::loadAll();
    vector<Schedule> schedules = Schedule::loadAll();

    if (routes.empty()) { cout << "  No routes available.\n"; return; }

    for (auto& route : routes) {
        route.display();
        cout << "  Buses & Schedules:\n";
        bool any = false;
        for (auto& bus : buses) {
            if (bus.getRouteId() == route.getRouteId()) {
                any = true;
                for (auto& sch : schedules)
                    if (sch.getBusId() == bus.getBusNumber())
                        cout << "    Bus " << bus.getBusNumber()
                             << " [" << sch.getScheduleType() << "]"
                             << " Departs: " << sch.getDepartureTime()
                             << " | Arrives: " << sch.getArrivalTime()
                             << " | Status: " << bus.getStatus() << "\n";
            }
        }
        if (!any) cout << "    No buses assigned.\n";
        printLine('-', 50);
    }
}

void studentSearchBus() {
    clearScreen();
    printHeader("SEARCH BUS / ROUTE");
    cout << "  1. Search by Bus Number\n";
    cout << "  2. Search by Route Name\n";
    int ch = getIntInput("  Enter choice: ");

    vector<Bus>   buses  = Bus::loadAll();
    vector<Route> routes = Route::loadAll();

    if (ch == 1) {
        cout << "  Enter Bus Number: ";
        string num; getline(cin, num);
        bool found = false;
        for (auto& b : buses) {
            if (b.getBusNumber() == num) {
                cout << "\n  Bus Found:\n";
                cout << left << setw(12) << "Bus No." << setw(10) << "Capacity"
                     << setw(12) << "Route" << setw(15) << "Status" << "Schedule\n";
                printLine();
                b.display();
                for (auto& r : routes)
                    if (r.getRouteId() == b.getRouteId())
                        { cout << "\n"; r.display(); }
                found = true; break;
            }
        }
        if (!found) cout << "  Bus not found!\n";
    } else if (ch == 2) {
        cout << "  Enter Route Name (or keyword): ";
        string kw; getline(cin, kw);
        string kwL = kw;
        transform(kwL.begin(), kwL.end(), kwL.begin(), ::tolower);
        bool found = false;
        for (auto& r : routes) {
            string rL = r.getRouteName();
            transform(rL.begin(), rL.end(), rL.begin(), ::tolower);
            if (rL.find(kwL) != string::npos) {
                r.display();
                cout << "  Buses:\n";
                for (auto& b : buses)
                    if (b.getRouteId() == r.getRouteId())
                        cout << "    Bus " << b.getBusNumber()
                             << " | " << b.getScheduleType()
                             << " | " << b.getStatus() << "\n";
                found = true;
            }
        }
        if (!found) cout << "  Route not found!\n";
    }
}

void studentViewFullHalfDay() {
    clearScreen();
    printHeader("FULL DAY / HALF DAY STATUS");
    vector<Bus> buses = Bus::loadAll();
    cout << left << setw(12) << "Bus No." << setw(12) << "Schedule" << "Status\n";
    printLine();
    for (auto& b : buses)
        cout << left << setw(12) << b.getBusNumber()
             << setw(12) << b.getScheduleType()
             << b.getStatus() << "\n";
}

void studentViewMySeat(Student& student) {
    clearScreen();
    printHeader("MY SEAT DETAILS");
    if (!student.hasAssignedSeat()) {
        cout << "  You have no assigned seat.\n";
        cout << "  Use option 6 to request a seat.\n";
        return;
    }
    vector<Seat>  seats  = Seat::loadAll();
    vector<Bus>   buses  = Bus::loadAll();
    vector<Route> routes = Route::loadAll();

    cout << "  Student ID  : " << student.getId()   << "\n";
    cout << "  Name        : " << student.getName()  << "\n";
    cout << "  Bus         : " << student.getBusId() << "\n";
    cout << "  Seat ID     : " << student.getSeatId() << "\n";

    for (auto& s : seats)
        if (s.getSeatId() == student.getSeatId()) {
            cout << "  Seat Number : " << s.getSeatNumber() << "\n";
            cout << "  Seat Type   : " << s.getSeatType()   << "\n";
            cout << "  Status      : " << s.getStatus()     << "\n";
            break;
        }

    for (auto& b : buses)
        if (b.getBusNumber() == student.getBusId()) {
            cout << "\n  Bus Details:\n";
            cout << "    Schedule : " << b.getScheduleType() << "\n";
            cout << "    Status   : " << b.getStatus()       << "\n";
            for (auto& r : routes)
                if (r.getRouteId() == b.getRouteId()) {
                    cout << "    Route    : " << r.getRouteName() << "\n";
                    cout << "    Timing   : " << r.getTiming()    << "\n";
                    cout << "    Stops    : ";
                    auto st = r.getStops();
                    for (size_t i = 0; i < st.size(); i++) {
                        cout << st[i];
                        if (i + 1 < st.size()) cout << " -> ";
                    }
                    cout << "\n";
                }
            break;
        }
}

void studentCancelSeat(Student& student) {
    clearScreen();
    printHeader("CANCEL SEAT");
    if (!student.hasAssignedSeat()) {
        cout << "  You have no seat to cancel.\n";
        return;
    }
    cout << "  Current seat : " << student.getSeatId()
         << " on Bus " << student.getBusId() << "\n";
    cout << "  Confirm cancel? (y/n): ";
    char c; cin >> c; cin.ignore();
    if (c == 'y' || c == 'Y') {
        SeatAllocation sa;
        sa.cancelSeat(student.getId());
        // Reload
        vector<Student> studs = Student::loadAll();
        Student* up = Student::findById(studs, student.getId());
        if (up) student = *up;
    } else {
        cout << "  Cancelled aborted.\n";
    }
}

void studentRequestSeat(Student& student) {
    clearScreen();
    printHeader("REQUEST SEAT ALLOCATION");
    if (student.hasAssignedSeat()) {
        cout << "  You already have seat " << student.getSeatId() << ".\n";
        return;
    }
    vector<Bus> buses = Bus::loadAll();
    if (buses.empty()) { cout << "  No buses available.\n"; return; }

    cout << "  Available Buses:\n";
    cout << left << setw(12) << "  Bus No." << setw(12) << "Route" << "Schedule\n";
    printLine();
    for (auto& b : buses)
        cout << "  " << left << setw(12) << b.getBusNumber()
             << setw(12) << b.getRouteId()
             << b.getScheduleType() << "\n";

    cout << "\n  Enter Bus Number: ";
    string busId; getline(cin, busId);

    if (!Bus::findByNumber(buses, busId)) {
        cout << "  Invalid bus number!\n"; return;
    }

    cout << "  Seat Preference:\n";
    cout << "    1. Window\n    2. Front\n    3. Any\n";
    int pch = getIntInput("  Enter choice: ");
    string pref = (pch == 1) ? "Window" : (pch == 2) ? "Front" : "Any";

    SeatAllocation sa;
    sa.allocateSeat(student.getId(), busId, pref);

    // Reload
    vector<Student> studs = Student::loadAll();
    Student* up = Student::findById(studs, student.getId());
    if (up) student = *up;
}

void studentMenu(Student student) {
    int ch;
    do {
        student.displayDashboard();
        cout << "  STUDENT MENU\n";
        printLine();
        cout << "  1. View Available Buses\n";
        cout << "  2. View Full Day / Half Day Status\n";
        cout << "  3. View Routes, Stops & Timings\n";
        cout << "  4. Search Bus / Route\n";
        cout << "  5. View My Seat\n";
        cout << "  6. Request Seat Allocation\n";
        cout << "  7. Cancel My Seat\n";
        cout << "  0. Logout\n";
        ch = getIntInput("  Enter choice: ");

        switch (ch) {
            case 1: studentViewBuses();         pauseScreen(); break;
            case 2: studentViewFullHalfDay();   pauseScreen(); break;
            case 3: studentViewRoutes();        pauseScreen(); break;
            case 4: studentSearchBus();         pauseScreen(); break;
            case 5: studentViewMySeat(student); pauseScreen(); break;
            case 6: studentRequestSeat(student);pauseScreen(); break;
            case 7: studentCancelSeat(student); pauseScreen(); break;
            case 0: cout << "  Logging out...\n"; break;
            default: cout << "  Invalid choice!\n"; pauseScreen();
        }
    } while (ch != 0);
}

Student doStudentLogin() {
    clearScreen();
    printHeader("STUDENT LOGIN");
    cout << "  College ID : "; string id; getline(cin, id);
    cout << "  Password   : "; string pw; getline(cin, pw);

    vector<Student> studs = Student::loadAll();
    for (auto& s : studs)
        if (s.login(id, pw)) {
            cout << "\n  Welcome, " << s.getName() << "!\n";
            pauseScreen();
            return s;
        }
    cout << "  [!] Invalid credentials!\n";
    pauseScreen();
    return Student();
}

// ============================================================
//  MODULE 2 — DRIVER MENU FUNCTIONS
// ============================================================

void driverViewBus(Driver& driver) {
    clearScreen();
    printHeader("MY ASSIGNED BUS");
    if (!driver.hasBusAssigned()) { cout << "  No bus assigned.\n"; return; }

    vector<Bus>      buses     = Bus::loadAll();
    vector<Route>    routes    = Route::loadAll();
    vector<Schedule> schedules = Schedule::loadAll();

    Bus* bus = Bus::findByNumber(buses, driver.getAssignedBusId());
    if (!bus) { cout << "  Bus data not found!\n"; return; }

    cout << "  Bus Number  : " << bus->getBusNumber()    << "\n";
    cout << "  Capacity    : " << bus->getCapacity()     << "\n";
    cout << "  Route       : " << bus->getRouteId()      << "\n";
    cout << "  Schedule    : " << bus->getScheduleType() << "\n";
    cout << "  Status      : " << bus->getStatus()       << "\n";

    for (auto& r : routes)
        if (r.getRouteId() == bus->getRouteId()) {
            cout << "\n  Route Name  : " << r.getRouteName() << "\n";
            cout << "  Timing      : " << r.getTiming()     << "\n";
            cout << "  Stops       : ";
            auto st = r.getStops();
            for (size_t i = 0; i < st.size(); i++) {
                cout << st[i]; if (i + 1 < st.size()) cout << " -> ";
            }
            cout << "\n"; break;
        }

    cout << "\n  Schedules:\n";
    bool schFound = false;
    for (auto& sch : schedules)
        if (sch.getBusId() == bus->getBusNumber()) {
            cout << "    [" << sch.getScheduleType() << "]"
                 << " Departs: " << sch.getDepartureTime()
                 << " | Arrives: " << sch.getArrivalTime() << "\n";
            schFound = true;
        }
    if (!schFound) cout << "    No schedules defined.\n";
}

void driverUpdateStatus(Driver& driver) {
    clearScreen();
    printHeader("UPDATE BUS STATUS");
    if (!driver.hasBusAssigned()) { cout << "  No bus assigned.\n"; return; }

    vector<Bus> buses = Bus::loadAll();
    Bus* bus = Bus::findByNumber(buses, driver.getAssignedBusId());
    if (!bus) { cout << "  Bus not found!\n"; return; }

    cout << "  Bus: " << bus->getBusNumber() << "\n";
    cout << "  Current Status: " << bus->getStatus() << "\n\n";
    cout << "  New Status:\n";
    cout << "    1. Not Started\n";
    cout << "    2. Running\n";
    cout << "    3. Completed\n";
    int ch = getIntInput("  Enter choice: ");

    string ns;
    if (ch == 1) ns = "Not Started";
    else if (ch == 2) ns = "Running";
    else if (ch == 3) ns = "Completed";
    else { cout << "  Invalid!\n"; return; }

    bus->setStatus(ns);
    Bus::saveAll(buses);
    cout << "  [OK] Status updated to: " << ns << "\n";
}

void driverMenu(Driver driver) {
    int ch;
    do {
        driver.displayDashboard();
        cout << "  DRIVER MENU\n";
        printLine();
        cout << "  1. View Assigned Bus & Route\n";
        cout << "  2. Update Bus Status\n";
        cout << "  3. View Bus Occupancy\n";
        cout << "  0. Logout\n";
        ch = getIntInput("  Enter choice: ");

        switch (ch) {
            case 1: driverViewBus(driver);    pauseScreen(); break;
            case 2:
                driverUpdateStatus(driver);
                pauseScreen();
                break;
            case 3:
                if (driver.hasBusAssigned()) {
                    SeatAllocation sa;
                    sa.displayOccupancy(driver.getAssignedBusId());
                } else {
                    cout << "  No bus assigned.\n";
                }
                pauseScreen(); break;
            case 0: cout << "  Logging out...\n"; break;
            default: cout << "  Invalid choice!\n"; pauseScreen();
        }
    } while (ch != 0);
}

Driver doDriverLogin() {
    clearScreen();
    printHeader("DRIVER LOGIN");
    cout << "  Driver ID  : "; string id; getline(cin, id);
    cout << "  Password   : "; string pw; getline(cin, pw);

    vector<Driver> drvs = Driver::loadAll();
    for (auto& d : drvs)
        if (d.login(id, pw)) {
            cout << "\n  Welcome, " << d.getName() << "!\n";
            pauseScreen();
            return d;
        }
    cout << "  [!] Invalid credentials!\n";
    pauseScreen();
    return Driver();
}

// ============================================================
//  MODULE 4 — ADMIN MENU FUNCTIONS
// ============================================================

// --- Admin: Students ---
void adminStudents() {
    int ch;
    do {
        clearScreen();
        printHeader("STUDENT MANAGEMENT");
        cout << "  1. Add Student\n  2. View All Students\n";
        cout << "  3. Update Student\n  4. Delete Student\n  0. Back\n";
        ch = getIntInput("  Enter choice: ");

        if (ch == 1) {
            clearScreen(); printHeader("ADD STUDENT");
            cout << "  Student ID : "; string id;   getline(cin, id);
            cout << "  Name       : "; string name; getline(cin, name);
            cout << "  Password   : "; string pw;   getline(cin, pw);

            vector<Student> studs = Student::loadAll();
            if (Student::findById(studs, id)) {
                cout << "  [!] ID already exists!\n";
            } else {
                studs.push_back(Student(name, id, pw, "-", "-", "Any"));
                Student::saveAll(studs);
                cout << "  [OK] Student added!\n";
            }
            pauseScreen();
        } else if (ch == 2) {
            clearScreen(); printHeader("ALL STUDENTS");
            vector<Student> studs = Student::loadAll();
            if (studs.empty()) { cout << "  No students.\n"; }
            else {
                cout << left << setw(14) << "ID" << setw(18) << "Name"
                     << setw(10) << "Bus" << setw(12) << "Seat" << "Preference\n";
                printLine();
                for (auto& s : studs)
                    cout << left << setw(14) << s.getId()
                         << setw(18) << s.getName()
                         << setw(10) << s.getBusId()
                         << setw(12) << s.getSeatId()
                         << s.getPreference() << "\n";
                cout << "\n  Total: " << studs.size() << "\n";
            }
            pauseScreen();
        } else if (ch == 3) {
            clearScreen(); printHeader("UPDATE STUDENT");
            cout << "  Student ID : "; string id; getline(cin, id);
            vector<Student> studs = Student::loadAll();
            Student* s = Student::findById(studs, id);
            if (!s) { cout << "  Not found!\n"; }
            else {
                cout << "  New Name (blank = keep '" << s->getName() << "'): ";
                string n; getline(cin, n);
                cout << "  New Password (blank = keep): ";
                string p; getline(cin, p);
                if (!n.empty()) s->setName(n);
                if (!p.empty()) s->setPassword(p);
                Student::saveAll(studs);
                cout << "  [OK] Updated!\n";
            }
            pauseScreen();
        } else if (ch == 4) {
            clearScreen(); printHeader("DELETE STUDENT");
            cout << "  Student ID : "; string id; getline(cin, id);
            vector<Student> studs = Student::loadAll();
            size_t before = studs.size();
            studs.erase(remove_if(studs.begin(), studs.end(),
                [&](Student& s){ return s.getId() == id; }), studs.end());
            if (studs.size() < before) { Student::saveAll(studs); cout << "  [OK] Deleted!\n"; }
            else cout << "  Not found!\n";
            pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin: Buses ---
void adminBuses() {
    int ch;
    do {
        clearScreen();
        printHeader("BUS MANAGEMENT");
        cout << "  1. Add Bus\n  2. View All Buses\n";
        cout << "  3. Update Bus\n  4. Delete Bus\n  5. Add Seats to Bus\n  0. Back\n";
        ch = getIntInput("  Enter choice: ");

        if (ch == 1) {
            clearScreen(); printHeader("ADD BUS");
            cout << "  Bus Number : "; string bn; getline(cin, bn);
            vector<Bus> buses = Bus::loadAll();
            if (Bus::findByNumber(buses, bn)) { cout << "  [!] Already exists!\n"; }
            else {
                int cap = getIntInput("  Capacity   : ");
                cout << "  Route ID   : "; string rid; getline(cin, rid);
                cout << "  Schedule (Full Day/Half Day): "; string scht; getline(cin, scht);
                buses.push_back(Bus(bn, cap, rid, "-", "Not Started", scht));
                Bus::saveAll(buses);
                cout << "  [OK] Bus added!\n";
            }
            pauseScreen();
        } else if (ch == 2) {
            clearScreen(); printHeader("ALL BUSES");
            vector<Bus> buses = Bus::loadAll();
            if (buses.empty()) { cout << "  No buses.\n"; }
            else {
                cout << left << setw(12) << "Bus No." << setw(10) << "Capacity"
                     << setw(10) << "Route" << setw(12) << "Driver"
                     << setw(15) << "Status" << "Schedule\n";
                printLine();
                for (auto& b : buses) b.display();
                cout << "\n  Total: " << buses.size() << "\n";
            }
            pauseScreen();
        } else if (ch == 3) {
            clearScreen(); printHeader("UPDATE BUS");
            cout << "  Bus Number : "; string bn; getline(cin, bn);
            vector<Bus> buses = Bus::loadAll();
            Bus* bus = Bus::findByNumber(buses, bn);
            if (!bus) { cout << "  Not found!\n"; }
            else {
                cout << "  New Route ID (blank=keep '" << bus->getRouteId() << "'): ";
                string r; getline(cin, r);
                cout << "  New Schedule (blank=keep '" << bus->getScheduleType() << "'): ";
                string s; getline(cin, s);
                cout << "  New Status (blank=keep '" << bus->getStatus() << "'): ";
                string st; getline(cin, st);
                if (!r.empty()) bus->setRouteId(r);
                if (!s.empty()) bus->setScheduleType(s);
                if (!st.empty()) bus->setStatus(st);
                Bus::saveAll(buses);
                cout << "  [OK] Bus updated!\n";
            }
            pauseScreen();
        } else if (ch == 4) {
            clearScreen(); printHeader("DELETE BUS");
            cout << "  Bus Number : "; string bn; getline(cin, bn);
            vector<Bus> buses = Bus::loadAll();
            size_t before = buses.size();
            buses.erase(remove_if(buses.begin(), buses.end(),
                [&](Bus& b){ return b.getBusNumber() == bn; }), buses.end());
            if (buses.size() < before) { Bus::saveAll(buses); cout << "  [OK] Deleted!\n"; }
            else cout << "  Not found!\n";
            pauseScreen();
        } else if (ch == 5) {
            // Add/reset seats for a bus with type distribution
            clearScreen(); printHeader("ADD SEATS TO BUS");
            cout << "  Bus Number : "; string bn; getline(cin, bn);
            vector<Bus> buses = Bus::loadAll();
            Bus* bus = Bus::findByNumber(buses, bn);
            if (!bus) { cout << "  Bus not found!\n"; }
            else {
                int cap = bus->getCapacity();
                cout << "  Total capacity: " << cap << "\n";
                int win = getIntInput("  Window seats : ");
                int frt = getIntInput("  Front seats  : ");
                int oth = cap - win - frt;
                if (oth < 0) { cout << "  [!] Exceeds capacity!\n"; }
                else {
                    vector<Seat> seats = Seat::loadAll();
                    // Remove old seats for this bus
                    seats.erase(remove_if(seats.begin(), seats.end(),
                        [&](Seat& s){ return s.getBusId() == bn; }), seats.end());

                    int n = 1;
                    auto addSeats = [&](int cnt, const string& type) {
                        for (int i = 0; i < cnt; i++, n++) {
                            string sid = bn + "-S" + to_string(n);
                            seats.push_back(Seat(sid, bn, n, type, "Available", "-"));
                        }
                    };
                    addSeats(win, "Window");
                    addSeats(frt, "Front");
                    addSeats(oth, "Other");

                    Seat::saveAll(seats);
                    cout << "  [OK] Seats created — Window: " << win
                         << "  Front: " << frt << "  Other: " << oth << "\n";
                }
            }
            pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin: Routes ---
void adminRoutes() {
    int ch;
    do {
        clearScreen();
        printHeader("ROUTE MANAGEMENT");
        cout << "  1. Add Route\n  2. View All Routes\n  3. Delete Route\n  0. Back\n";
        ch = getIntInput("  Enter choice: ");

        if (ch == 1) {
            clearScreen(); printHeader("ADD ROUTE");
            cout << "  Route ID   : "; string rid; getline(cin, rid);
            vector<Route> routes = Route::loadAll();
            if (Route::findById(routes, rid)) { cout << "  [!] Already exists!\n"; }
            else {
                cout << "  Route Name : "; string rn; getline(cin, rn);
                cout << "  Stops (comma-separated e.g. A,B,C): ";
                string st; getline(cin, st);
                vector<string> stops = split(st, ',');
                for (auto& s : stops) s = trim(s);
                cout << "  Timing (e.g. 7:00 AM - 9:00 AM): ";
                string tim; getline(cin, tim);
                routes.push_back(Route(rid, rn, stops, tim));
                Route::saveAll(routes);
                cout << "  [OK] Route added!\n";
            }
            pauseScreen();
        } else if (ch == 2) {
            clearScreen(); printHeader("ALL ROUTES");
            vector<Route> routes = Route::loadAll();
            if (routes.empty()) { cout << "  No routes.\n"; }
            else for (auto& r : routes) { r.display(); printLine('-', 50); }
            pauseScreen();
        } else if (ch == 3) {
            clearScreen(); printHeader("DELETE ROUTE");
            cout << "  Route ID : "; string rid; getline(cin, rid);
            vector<Route> routes = Route::loadAll();
            size_t before = routes.size();
            routes.erase(remove_if(routes.begin(), routes.end(),
                [&](Route& r){ return r.getRouteId() == rid; }), routes.end());
            if (routes.size() < before) { Route::saveAll(routes); cout << "  [OK] Deleted!\n"; }
            else cout << "  Not found!\n";
            pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin: Drivers ---
void adminDrivers() {
    int ch;
    do {
        clearScreen();
        printHeader("DRIVER MANAGEMENT");
        cout << "  1. Add Driver\n  2. View All Drivers\n";
        cout << "  3. Assign Driver to Bus\n  4. Delete Driver\n  0. Back\n";
        ch = getIntInput("  Enter choice: ");

        if (ch == 1) {
            clearScreen(); printHeader("ADD DRIVER");
            cout << "  Driver ID  : "; string id;   getline(cin, id);
            vector<Driver> drvs = Driver::loadAll();
            if (Driver::findById(drvs, id)) { cout << "  [!] Already exists!\n"; }
            else {
                cout << "  Name       : "; string name; getline(cin, name);
                cout << "  Password   : "; string pw;   getline(cin, pw);
                cout << "  License No : "; string lic;  getline(cin, lic);
                cout << "  Contact    : "; string con;  getline(cin, con);
                drvs.push_back(Driver(name, id, pw, lic, con, "-"));
                Driver::saveAll(drvs);
                cout << "  [OK] Driver added!\n";
            }
            pauseScreen();
        } else if (ch == 2) {
            clearScreen(); printHeader("ALL DRIVERS");
            vector<Driver> drvs = Driver::loadAll();
            if (drvs.empty()) { cout << "  No drivers.\n"; }
            else {
                cout << left << setw(12) << "ID" << setw(18) << "Name"
                     << setw(14) << "License" << setw(12) << "Contact" << "Bus\n";
                printLine();
                for (auto& d : drvs)
                    cout << left << setw(12) << d.getId()
                         << setw(18) << d.getName()
                         << setw(14) << d.getLicenseNo()
                         << setw(12) << d.getContact()
                         << d.getAssignedBusId() << "\n";
                cout << "\n  Total: " << drvs.size() << "\n";
            }
            pauseScreen();
        } else if (ch == 3) {
            clearScreen(); printHeader("ASSIGN DRIVER TO BUS");
            vector<Driver> drvs  = Driver::loadAll();
            vector<Bus>    buses = Bus::loadAll();

            if (drvs.empty())  { cout << "  No drivers!\n";  pauseScreen(); continue; }
            if (buses.empty()) { cout << "  No buses!\n";    pauseScreen(); continue; }

            cout << "  Drivers:\n";
            for (auto& d : drvs)
                cout << "    " << d.getId() << " - " << d.getName()
                     << " (Bus: " << d.getAssignedBusId() << ")\n";
            cout << "\n  Enter Driver ID : "; string did; getline(cin, did);

            Driver* drv = Driver::findById(drvs, did);
            if (!drv) { cout << "  Driver not found!\n"; pauseScreen(); continue; }

            cout << "\n  Buses:\n";
            for (auto& b : buses)
                cout << "    " << b.getBusNumber()
                     << " (Route: " << b.getRouteId()
                     << ", Driver: " << b.getDriverId() << ")\n";
            cout << "\n  Enter Bus Number : "; string bnum; getline(cin, bnum);

            Bus* bus = Bus::findByNumber(buses, bnum);
            if (!bus) { cout << "  Bus not found!\n"; }
            else {
                // Remove old driver from bus
                if (bus->getDriverId() != "-") {
                    Driver* old = Driver::findById(drvs, bus->getDriverId());
                    if (old) old->setAssignedBusId("-");
                }
                drv->setAssignedBusId(bnum);
                bus->setDriverId(did);
                Driver::saveAll(drvs);
                Bus::saveAll(buses);
                cout << "  [OK] Driver " << did << " assigned to Bus " << bnum << "!\n";
            }
            pauseScreen();
        } else if (ch == 4) {
            clearScreen(); printHeader("DELETE DRIVER");
            cout << "  Driver ID : "; string id; getline(cin, id);
            vector<Driver> drvs = Driver::loadAll();
            size_t before = drvs.size();
            drvs.erase(remove_if(drvs.begin(), drvs.end(),
                [&](Driver& d){ return d.getId() == id; }), drvs.end());
            if (drvs.size() < before) { Driver::saveAll(drvs); cout << "  [OK] Deleted!\n"; }
            else cout << "  Not found!\n";
            pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin: Schedules ---
void adminSchedules() {
    int ch;
    do {
        clearScreen();
        printHeader("SCHEDULE MANAGEMENT");
        cout << "  1. Add Schedule\n  2. View All Schedules\n  3. Delete Schedule\n  0. Back\n";
        ch = getIntInput("  Enter choice: ");

        if (ch == 1) {
            clearScreen(); printHeader("ADD SCHEDULE");
            cout << "  Schedule ID     : "; string sid; getline(cin, sid);
            cout << "  Bus Number      : "; string bid; getline(cin, bid);
            cout << "  Type (Full Day/Half Day): "; string st; getline(cin, st);
            cout << "  Departure Time  : "; string dep; getline(cin, dep);
            cout << "  Arrival Time    : "; string arr; getline(cin, arr);
            vector<Schedule> schs = Schedule::loadAll();
            schs.push_back(Schedule(sid, bid, st, dep, arr));
            Schedule::saveAll(schs);
            cout << "  [OK] Schedule added!\n";
            pauseScreen();
        } else if (ch == 2) {
            clearScreen(); printHeader("ALL SCHEDULES");
            vector<Schedule> schs = Schedule::loadAll();
            if (schs.empty()) { cout << "  No schedules.\n"; }
            else {
                cout << left << setw(12) << "ID" << setw(10) << "Bus"
                     << setw(12) << "Type" << setw(14) << "Departure" << "Arrival\n";
                printLine();
                for (auto& s : schs) s.display();
                cout << "\n  Total: " << schs.size() << "\n";
            }
            pauseScreen();
        } else if (ch == 3) {
            clearScreen(); printHeader("DELETE SCHEDULE");
            cout << "  Schedule ID : "; string sid; getline(cin, sid);
            vector<Schedule> schs = Schedule::loadAll();
            size_t before = schs.size();
            schs.erase(remove_if(schs.begin(), schs.end(),
                [&](Schedule& s){ return s.getScheduleId() == sid; }), schs.end());
            if (schs.size() < before) { Schedule::saveAll(schs); cout << "  [OK] Deleted!\n"; }
            else cout << "  Not found!\n";
            pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin: Seat Allocations ---
void adminSeatAllocations() {
    SeatAllocation sa;
    int ch;
    do {
        clearScreen();
        printHeader("SEAT ALLOCATION MANAGEMENT");
        cout << "  1. View All Seats\n";
        cout << "  2. Allocate Seat to Student\n";
        cout << "  3. Cancel Seat Allocation\n";
        cout << "  4. View Waiting List\n";
        cout << "  5. View Bus Occupancy\n";
        cout << "  0. Back\n";
        ch = getIntInput("  Enter choice: ");

        if (ch == 1) {
            clearScreen(); printHeader("ALL SEATS");
            vector<Bus> buses = Bus::loadAll();
            for (auto& b : buses) sa.displayAllSeats(b.getBusNumber());
            pauseScreen();
        } else if (ch == 2) {
            clearScreen(); printHeader("ALLOCATE SEAT");
            cout << "  Student ID  : "; string sid; getline(cin, sid);
            cout << "  Bus Number  : "; string bid; getline(cin, bid);
            cout << "  Preference (Window/Front/Any): "; string pref; getline(cin, pref);
            sa.allocateSeat(sid, bid, pref);
            pauseScreen();
        } else if (ch == 3) {
            clearScreen(); printHeader("CANCEL SEAT ALLOCATION");
            cout << "  Student ID  : "; string sid; getline(cin, sid);
            sa.cancelSeat(sid);
            pauseScreen();
        } else if (ch == 4) {
            sa.displayWaitingList();
            pauseScreen();
        } else if (ch == 5) {
            clearScreen(); printHeader("BUS OCCUPANCY");
            vector<Bus> buses = Bus::loadAll();
            for (auto& b : buses) sa.displayOccupancy(b.getBusNumber());
            pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin: Reports Menu ---
void adminReports() {
    Report rpt;
    int ch;
    do {
        clearScreen();
        printHeader("REPORTS");
        cout << "  1. Daily Bus Report\n";
        cout << "  2. Occupancy Report\n";
        cout << "  3. Route-Wise Report\n";
        cout << "  4. Full Day / Half Day Report\n";
        cout << "  5. Driver Report\n";
        cout << "  6. Student Seat Report\n";
        cout << "  0. Back\n";
        ch = getIntInput("  Enter choice: ");
        switch (ch) {
            case 1: rpt.generateDailyBusReport();    pauseScreen(); break;
            case 2: rpt.generateOccupancyReport();   pauseScreen(); break;
            case 3: rpt.generateRouteWiseReport();   pauseScreen(); break;
            case 4: rpt.generateFullHalfDayReport(); pauseScreen(); break;
            case 5: rpt.generateDriverReport();      pauseScreen(); break;
            case 6: rpt.generateStudentSeatReport(); pauseScreen(); break;
            case 0: break;
            default: cout << "  Invalid choice!\n"; pauseScreen();
        }
    } while (ch != 0);
}

// --- Admin Main Menu ---
void adminMenu(Admin admin) {
    int ch;
    do {
        admin.displayDashboard();
        cout << "  ADMIN MENU\n";
        printLine();
        cout << "  1. Manage Students\n";
        cout << "  2. Manage Buses\n";
        cout << "  3. Manage Routes\n";
        cout << "  4. Manage Drivers\n";
        cout << "  5. Manage Schedules\n";
        cout << "  6. Manage Seat Allocations\n";
        cout << "  7. Reports\n";
        cout << "  0. Logout\n";
        ch = getIntInput("  Enter choice: ");
        switch (ch) {
            case 1: adminStudents();       break;
            case 2: adminBuses();          break;
            case 3: adminRoutes();         break;
            case 4: adminDrivers();        break;
            case 5: adminSchedules();      break;
            case 6: adminSeatAllocations();break;
            case 7: adminReports();        break;
            case 0: cout << "  Logging out...\n"; break;
            default: cout << "  Invalid choice!\n"; pauseScreen();
        }
    } while (ch != 0);
}

Admin doAdminLogin() {
    clearScreen();
    printHeader("ADMIN LOGIN");
    cout << "  Admin ID   : "; string id; getline(cin, id);
    cout << "  Password   : "; string pw; getline(cin, pw);
    Admin admin;
    if (admin.login(id, pw)) {
        cout << "\n  Welcome, Administrator!\n";
        pauseScreen();
        return admin;
    }
    cout << "  [!] Invalid credentials!\n";
    pauseScreen();
    return Admin("", "", "");
}

// ============================================================
//  SYSTEM INITIALISATION
// ============================================================
void initSystem() {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif
    // Create files if missing
    auto ensure = [](const string& path) {
        ifstream f(path);
        if (!f.good()) { ofstream nf(path); }
    };
    ensure(STUDENTS_FILE);
    ensure(BUSES_FILE);
    ensure(ROUTES_FILE);
    ensure(DRIVERS_FILE);
    ensure(SEATS_FILE);
    ensure(SCHEDULES_FILE);
    ensure(WAITING_FILE);
}

// ============================================================
//  MAIN
// ============================================================
int main() {
    initSystem();

    int ch;
    do {
        clearScreen();
        printTitle("SMART COLLEGE BUS MANAGEMENT SYSTEM");
        cout << "\n";
        cout << "          Developed in C++ using OOP Principles\n\n";
        cout << "  +-------------------------------------------------+\n";
        cout << "  |  1.  Student Login                              |\n";
        cout << "  |  2.  Driver Login                               |\n";
        cout << "  |  3.  Admin Login  (ID: admin / PW: admin123)    |\n";
        cout << "  |  0.  Exit                                       |\n";
        cout << "  +-------------------------------------------------+\n\n";
        ch = getIntInput("  Select option: ");

        switch (ch) {
            case 1: {
                Student s = doStudentLogin();
                if (!s.getId().empty()) studentMenu(s);
                break;
            }
            case 2: {
                Driver d = doDriverLogin();
                if (!d.getId().empty()) driverMenu(d);
                break;
            }
            case 3: {
                Admin a = doAdminLogin();
                if (!a.getId().empty()) adminMenu(a);
                break;
            }
            case 0:
                clearScreen();
                printTitle("THANK YOU!");
                cout << "\n  Goodbye! Have a safe journey.\n\n";
                break;
            default:
                cout << "  Invalid option!\n";
                pauseScreen();
        }
    } while (ch != 0);

    return 0;
}
