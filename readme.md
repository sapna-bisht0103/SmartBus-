# Smart College Bus Management System

## 📌 Project Overview

The **Smart College Bus Management System** is an Object-Oriented Programming (OOP) based C++ project designed to manage college buses, students, drivers, routes, schedules, and seat allocation.

The system helps students easily find available buses, check routes and timings, view their assigned seats, and cancel their seats. Drivers can manage their assigned buses and update bus status, while administrators can manage the complete system and generate different reports.

---

## 🎯 Objectives

The main objectives of this project are:

* To manage college buses efficiently.
* To provide students with an easy bus-search and seat-booking system.
* To manage routes, stops, and bus timings.
* To allocate seats automatically based on availability and preference.
* To maintain a waiting list when seats are unavailable.
* To manage driver and bus assignments.
* To provide Full Day and Half Day bus schedules.
* To allow drivers to update bus status.
* To provide administrators with complete system management.
* To generate useful reports related to buses, seats, routes, drivers, and students.

---

## 🛠️ Technologies Used

* **Programming Language:** C++
* **Programming Concept:** Object-Oriented Programming (OOP)
* **Data Storage:** File Handling
* **User Interface:** GUI / Console interface depending on implementation
* **Compiler:** GCC / MinGW
* **Development Environment:** Visual Studio Code

---

# 👥 Project Modules

The project is divided into four major modules.

## 👨‍🎓 Member 1 – Student & Login Module

### Responsibilities

* Student login using College ID and Password
* Student dashboard
* View available buses
* View Full Day / Half Day status
* View routes, stops, and timings
* Search bus or route
* View assigned seat
* Cancel seat booking
* Display student details

### Main Classes

```text
Student
StudentLogin
StudentDashboard
```

### Module Flow

```text
Student
   ↓
Login
   ↓
Student Dashboard
   ↓
Search Bus / Route
   ↓
Check Schedule
   ↓
View Seat
   ↓
Cancel Seat (if required)
```

---

# 🚌 Member 2 – Bus, Route & Driver Module

### Responsibilities

* Bus management
* Add and view bus details
* Manage routes and stops
* Manage bus timings
* Manage Full Day / Half Day schedules
* Manage driver details
* Assign driver to bus
* Driver login
* Driver dashboard
* Update bus status

### Bus Status

```text
Not Started
      ↓
   Running
      ↓
  Completed
```

### Main Classes

```text
Bus
Route
Driver
Schedule
DriverLogin
DriverDashboard
```

### Module Flow

```text
Bus / Route / Driver
        ↓
    Management
        ↓
Driver Assignment
        ↓
Driver Login
        ↓
Driver Dashboard
        ↓
Update Bus Status
```

---

# 💺 Member 3 – Seat Allocation Module

### Responsibilities

* Display available seats
* Allocate seats to students
* Cancel seat allocation
* Manage seat preferences
* Automatic seat allocation
* Maintain waiting list
* Calculate bus occupancy
* Maintain student-seat mapping

### Seat Preferences

```text
1. Window
2. Front
3. Any
```

### Seat Status

```text
Available
    ↓
  Booked
    ↓
Available
```

### Main Classes

```text
Seat
SeatAllocation
WaitingList
```

### Module Flow

```text
Student Request
      ↓
Check Bus Availability
      ↓
Check Available Seats
      ↓
Check Seat Preference
      ↓
Allocate Seat
      ↓
Update Occupancy
```

If no seat is available:

```text
No Seat Available
       ↓
   Waiting List
       ↓
Seat Becomes Available
       ↓
Automatic Allocation
```

---

# 👨‍💼 Member 4 – Admin & Reports Module

### Responsibilities

* Admin login
* Admin dashboard
* Student management
* Bus management
* Route management
* Driver management
* Seat allocation management
* Generate daily bus reports
* Generate occupancy reports
* Generate route-wise reports
* Generate Full Day / Half Day reports
* Generate driver reports
* Generate student-seat reports

### Main Classes

```text
Admin
Report
StudentManager
BusManager
RouteManager
DriverManager
```

### Module Flow

```text
Admin Login
     ↓
Admin Dashboard
     ↓
Manage Students
Manage Buses
Manage Routes
Manage Drivers
Manage Seats
     ↓
Generate Reports
```

---

# 🧩 OOP Concepts Used

The project demonstrates important Object-Oriented Programming concepts.

## 1. Class and Object

Classes are used to represent real-world entities.

Examples:

```cpp
class Student
{
    // Student data and functions
};

class Bus
{
    // Bus data and functions
};

class Driver
{
    // Driver data and functions
};
```

Objects are created from these classes.

---

## 2. Encapsulation

Data members are kept private and accessed through public functions.

Example:

```cpp
class Student
{
private:
    string password;

public:
    void setPassword(string p);
    string getPassword();
};
```

This protects important student information.

---

## 3. Inheritance

A common `User` class can be created and inherited by different types of users.

```text
             User
          /    |    \
         /     |     \
    Student  Driver  Admin
```

Example:

```cpp
class User
{
protected:
    string name;
    string id;
};

class Student : public User
{
    // Student-specific functions
};

class Driver : public User
{
    // Driver-specific functions
};

class Admin : public User
{
    // Admin-specific functions
};
```

---

## 4. Polymorphism

Different user types can have their own implementation of common functions such as `login()` or `displayDashboard()`.

```cpp
virtual void displayDashboard();
```

Student, Driver, and Admin can implement the function differently.

---

## 5. Abstraction

Complex internal operations such as seat allocation and report generation are hidden from the user.

For example:

```cpp
allocateSeat();
```

The user only needs to call the function without knowing all the internal allocation logic.

---

## 6. Constructors

Constructors are used to initialize objects.

Example:

```cpp
Student(int id, string name)
{
    studentId = id;
    studentName = name;
}
```

---

## 7. File Handling

File handling is used to store data permanently.

The system can store:

* Student information
* Bus information
* Route information
* Driver information
* Seat information
* Schedule information

Example:

```cpp
ofstream file("students.txt");

file << studentId << " "
     << studentName << endl;

file.close();
```

---

# 🏗️ System Architecture

```text
                  SMART COLLEGE BUS
                  MANAGEMENT SYSTEM
                           |
        +------------------+------------------+
        |                  |                  |
     Student             Driver             Admin
        |                  |                  |
   Login/Dashboard    Login/Dashboard    Login/Dashboard
        |                  |                  |
        +------------------+------------------+
                           |
                    Core Management
                           |
        +------------------+------------------+
        |                  |                  |
       Bus               Route              Driver
        |                  |                  |
        +------------------+------------------+
                           |
                    Seat Allocation
                           |
                 +---------+---------+
                 |                   |
             Seat Booking       Waiting List
                 |
             Occupancy
                 |
              Reports
```

---

# 🔄 Complete System Workflow

```text
                    START
                      |
                      ↓
                 Select Role
                      |
        +-------------+-------------+
        |             |             |
     Student        Driver         Admin
        |             |             |
      Login          Login          Login
        |             |             |
   Dashboard      Dashboard      Dashboard
        |             |             |
   Search Bus    Assigned Bus    Management
        |             |             |
   Check Route   Update Status   Manage Data
        |                           |
   Check Seat                   Reports
        |
   Allocate Seat
        |
   View / Cancel Seat
        |
       END
```

---

# 📊 Reports

The system provides different reports for administrators.

### Daily Bus Report

Displays daily bus information and schedules.

### Occupancy Report

Displays:

* Total seats
* Booked seats
* Available seats
* Occupancy percentage

### Route-Wise Report

Displays buses and students according to routes.

### Full Day / Half Day Report

Displays buses and schedules based on their day type.

### Driver Report

Displays:

* Driver details
* Assigned bus
* Assigned route
* Bus status

### Student Seat Report

Displays:

* Student ID
* Student name
* Bus number
* Route
* Seat number
* Seat preference

---

# 📁 Suggested Project Structure

```text
SmartCollegeBusManagementSystem/
│
├── README.md
│
├── include/
│   ├── User.h
│   ├── Student.h
│   ├── Driver.h
│   ├── Admin.h
│   ├── Bus.h
│   ├── Route.h
│   ├── Schedule.h
│   ├── Seat.h
│   ├── SeatAllocation.h
│   ├── WaitingList.h
│   └── Report.h
│
├── src/
│   ├── main.cpp
│   ├── Student.cpp
│   ├── Driver.cpp
│   ├── Admin.cpp
│   ├── Bus.cpp
│   ├── Route.cpp
│   ├── Schedule.cpp
│   ├── Seat.cpp
│   ├── SeatAllocation.cpp
│   ├── WaitingList.cpp
│   └── Report.cpp
│
├── data/
│   ├── students.txt
│   ├── buses.txt
│   ├── routes.txt
│   ├── drivers.txt
│   ├── seats.txt
│   └── schedules.txt
│
└── docs/
    ├── Project_Report.pdf
    └── Project_Presentation.pptx
```

---

# 👥 Team Contribution

| Member   | Module              | Main Responsibility                      |
| -------- | ------------------- | ---------------------------------------- |
| Member 1 | Student & Login     | Student dashboard, login, bus search     |
| Member 2 | Bus, Route & Driver | Bus, routes, schedules, drivers          |
| Member 3 | Seat Allocation     | Seat booking, cancellation, waiting list |
| Member 4 | Admin & Reports     | Admin management and reports             |

---

# 🎯 Expected Outcome

The final system will provide a centralized platform for managing college transportation. Students can find buses and manage their seats, drivers can manage their assigned buses and status, and administrators can control the system and generate reports.

The project demonstrates how **Object-Oriented Programming in C++** can be applied to solve a real-world college transportation management problem.

---

# 🚀 Future Enhancements

Possible future improvements include:

* Mobile application
* Online database integration
* QR-based student verification
* GPS-based live bus tracking
* Push notifications
* Online seat booking
* Real-time bus location
* Cloud-based data storage
* Advanced analytics dashboard

---

# 👨‍💻 Team Project

**Project Name:** Smart College Bus Management System
**Language:** C++
**Concept:** Object-Oriented Programming
**Project Type:** College Academic Project
