# Real Estate Property Management System

A comprehensive C++ console-based application for managing real estate properties with user authentication, property browsing, and administrative features.

## Overview

This Property Management System provides a complete solution for managing real estate properties. It features role-based access control with separate interfaces for guests, regular users, and administrators. The system uses SQLite for data persistence and includes advanced search and filtering capabilities.

##  Features

### For Guests (Unauthenticated Users)
- **View Properties**: Browse available properties in an interactive table view
- **Search Properties**: Filter properties by location, price range, type, rooms, bathrooms, and area
- **User Registration**: Create new account to access additional features

### For Registered Users
- All guest features, plus:
- **Contact Information**: View property contact details (phone numbers)
- **Full Property Details**: Access complete property information

### For Administrators
- All user features, plus:
- **Add Properties**: Create new property listings
- **Update Properties**: Modify existing property information
- **Delete Properties**: Remove property listings
- **Lock/Unlock Properties**: Control property availability
- **Advanced Management**: Comprehensive property management dashboard

##  Architecture

### Core Components

- **DBManager**: Handles all database operations and SQLite integration
- **PropertyManager**: Manages property viewing and display functionality
- **UserManager**: Handles user authentication (login/signup)
- **AdminManager**: Provides administrative property management features
- **FilterationManager**: Implements property search and filtering logic
- **ConsoleUtils**: Utility functions for console UI management

### Data Structures

#### Property
```cpp
struct Property {
    int id;
    string name;
    string location;
    double price;
    string type;
    int available;
    string infoNumber;
    int noOfRooms;
    int noOfBaths;
    double area;
};
```

#### User
```cpp
struct User {
    int id;
    string email;
    bool isAdmin;
};
```

## 🚀 Getting Started

### Prerequisites

- C++ Compiler (GCC/MinGW recommended)
- SQLite3 library
- Windows OS (uses Windows-specific console functions)
- Code::Blocks IDE (optional, project file included)

### Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd SWE-team
```

2. Compile the project:
```bash
g++ main.cpp UserManager.cpp sqlite3.c -o real-estate-system
```

Or open `swe-team.cbp` in Code::Blocks and build the project.

3. Run the executable:
```bash
./bin/Debug/swe-team
```

## 📖 Usage

### First Time Setup

On first run, the application will automatically create the SQLite database with the required tables:
- `properties` - Stores property information
- `users` - Stores user accounts and roles

### Navigation

- **Arrow Keys**: Navigate through menus and property lists
- **Enter**: Select menu option or view property details
- **ESC**: Return to previous menu or exit
- **Keyboard Input**: Type values for search filters and forms

### Sample Workflow

1. **Guest Access**: View and search properties without logging in
2. **Sign Up**: Create an account to access contact information
3. **Login**: Authenticate to unlock full features
4. **Admin Access**: Administrators can manage the property database

## 🗂️ Project Structure

```
SWE-team/
├── main.cpp                    # Application entry point
├── DBManager.h                 # Database management (562 lines)
├── PropertyManager.h           # Property viewing and display
├── UserManager.h/cpp           # User authentication
├── AdminManager.h              # Admin functionality
├── FilterationManager.h        # Search and filtering
├── ConsoleUtils.h              # Console UI utilities
├── Property.h                  # Property data structure
├── sqlite3.h/c                 # SQLite database engine
├── bin/Debug/                  # Compiled executables
├── DOC/                        # Detailed documentation
│   ├── main.md
│   ├── DBManager.md
│   ├── PropertyManager.md
│   ├── UserManager.md
│   └── AdminManager.md
└── docs/                       # UML diagrams
    ├── database-schema.puml
    ├── login-activity-diagram-v3.puml
    ├── login-sequence-diagram.puml
    └── sign up-activity-diagram.puml
```

##  Documentation

Detailed documentation for each module is available in the `DOC/` directory:

- [Main Application Flow](DOC/main.md)
- [Database Manager](DOC/DBManager.md)
- [Property Manager](DOC/PropertyManager.md)
- [User Manager](DOC/UserManager.md)
- [Admin Manager](DOC/AdminManager.md)

UML diagrams for system design are available in the `docs/` directory.

##  Technical Details

### Database Schema

The application uses SQLite with the following main tables:

**Properties Table:**
- `id` (INTEGER PRIMARY KEY)
- `name` (TEXT)
- `location` (TEXT)
- `price` (REAL)
- `type` (TEXT)
- `available` (INTEGER)
- `infoNumber` (TEXT)
- `noOfRooms` (INTEGER)
- `noOfBaths` (INTEGER)
- `area` (REAL)

**Users Table:**
- `id` (INTEGER PRIMARY KEY)
- `email` (TEXT UNIQUE)
- `password` (TEXT)
- `isAdmin` (INTEGER)

### Search Capabilities

The system supports filtering properties by:
- Location (partial match)
- Price range (min/max)
- Property type (apartment, villa, land, etc.)
- Number of rooms
- Number of bathrooms
- Area (square meters)

##  UI Features

- ASCII art header with styled branding
- Color-coded console output
- Interactive keyboard navigation
- Multi-line text editor for property descriptions
- Tabular property display with pagination
- Context-aware menus based on user role

##  Security Features

- Password-based authentication
- Role-based access control (Guest/User/Admin)
- Session management with login state tracking
- Protected admin operations

##  Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


## 👥 Team

SWE-Team @ ITI

