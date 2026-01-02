#ifndef ADMIN_MANAGER_H
#define ADMIN_MANAGER_H

#include "DBManager.h"
#include "Property.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include "ConsoleUtils.h"

#define NORMAL_PEN 0x07
#define HIGHLIGHTED_PEN 0x70

using namespace std;

extern bool isLoggedIn;
extern string currentUserEmail;

class AdminManager {
private:
    string trim(const char* s) {
        string str(s);
        size_t start = str.find_first_not_of(' ');
        if (start != string::npos)
            str = str.substr(start);
        else
            return "";
        while (!str.empty() && str.back() == ' ')
            str.pop_back();
        return str;
    }

    // Draw a fancy table for owners using DBManager
    void displayOwnersTable(DBManager* dbManager, int startX, int startY) {
        vector<pair<int, string>> owners = dbManager->getAllOwners(10);

        if (owners.empty()) {
            gotoxy(startX, startY);
            textattr(12);
            cout << "No owners available!";
            textattr(NORMAL_PEN);
            return;
        }

        // Draw table header
        gotoxy(startX, startY);
        textattr(240);
        cout << " ID   | Owner Name            ";
        textattr(NORMAL_PEN);

        gotoxy(startX, startY + 1);
        cout << "------+------------------------";

        // Draw owner data
        for (size_t i = 0; i < owners.size(); i++) {
            gotoxy(startX, startY + 2 + i);
            textattr(11);
            cout << " " << setw(4) << left << owners[i].first;
            textattr(15);
            cout << " | " << setw(20) << left << owners[i].second.substr(0, 20);
        }
        textattr(NORMAL_PEN);
    }

    void displayPropertiesTable(const vector<Property>& props, bool showOwnerId = false) {
        textattr(240);
        if (showOwnerId) {
            cout << " ID  | NAME              | LOCATION        | PRICE       | TYPE  | CONTACT      | ROOMS | BATHS | AREA    | OWNER ID | AVAIL\n";
        } else {
            cout << " ID    | NAME                | LOCATION          | PRICE              | TYPE      | AVAILABLE\n";
        }
        textattr(15);

        for (size_t i = 0; i < props.size(); i++) {
            if (showOwnerId) {
                cout << setw(4) << props[i].getId() << " | ";
                cout << setw(17) << props[i].getName().substr(0, 17) << " | ";
                cout << setw(15) << props[i].getLocation().substr(0, 15) << " | ";
                cout << "$" << setw(10) << fixed << setprecision(0) << props[i].getPrice() << " | ";
                cout << setw(5) << props[i].getType() << " | ";
                cout << setw(12) << props[i].getInfoNumber().substr(0, 12) << " | ";
                cout << setw(5) << props[i].getNoOfRooms() << " | ";
                cout << setw(5) << props[i].getNoOfBaths() << " | ";
                cout << setw(7) << fixed << setprecision(1) << props[i].getArea() << " | ";
                cout << setw(8) << "N/A" << " | ";
                cout << (props[i].getAvailable() ? "Yes" : "No") << "\n";
            } else {
                cout << setw(5) << props[i].getId() << " | ";
                cout << setw(18) << props[i].getName().substr(0, 18) << " | ";
                cout << setw(16) << props[i].getLocation().substr(0, 16) << " | ";
                cout << "$" << setw(16) << fixed << setprecision(2) << props[i].getPrice() << " | ";
                cout << setw(8) << props[i].getType() << " | ";
                cout << (props[i].getAvailable() ? "Yes" : "No") << "\n";
            }
        }
    }

public:
    void addProperty(DBManager* dbManager) {
        system("cls");
        int width = 50, height = 22;
        int startX = 5, startY = 2;

        // Draw frame
        for (int y = startY; y <= startY + height; y++) {
            gotoxy(startX, y);
            for (int x = startX; x <= startX + width; x++) {
                if (y == startY || y == startY + height) cout << "=";
                else if (x == startX || x == startX + width) cout << "|";
                else cout << " ";
            }
        }

        // Title
        gotoxy(startX + 15, startY);
        textattr(HIGHLIGHTED_PEN);
        cout << " ADD NEW PROPERTY ";
        textattr(NORMAL_PEN);

        // Labels for input fields
        gotoxy(startX + 2, startY + 2);  cout << "Property Name:";
        gotoxy(startX + 2, startY + 4);  cout << "Location:";
        gotoxy(startX + 2, startY + 6);  cout << "Price:";
        gotoxy(startX + 2, startY + 8);  cout << "Type (Buy/Rent):";
        gotoxy(startX + 2, startY + 10); cout << "Contact Number:";
        gotoxy(startX + 2, startY + 12); cout << "Number of Rooms:";
        gotoxy(startX + 2, startY + 14); cout << "Number of Baths:";
        gotoxy(startX + 2, startY + 16); cout << "Area:";
        gotoxy(startX + 2, startY + 18); cout << "Owner ID:";

        // Display owners table OUTSIDE the box on the right side
        int ownerTableX = startX + width + 5;
        gotoxy(ownerTableX, startY + 2);
        textattr(14);
        cout << "Available Owners:";
        textattr(NORMAL_PEN);
        displayOwnersTable(dbManager, ownerTableX, startY + 3);

        // Input fields setup
        int lineno = 9;
        int maxLen = 30;
        char sr[9] = {32, 32, 48, 32, 32, 48, 48, 48, 48};
        char er[9] = {126, 126, 57, 126, 126, 57, 57, 57, 57};

        int editorX = startX + 20;
        char** input = multiLineEditor(editorX, startY + 2, maxLen, sr, er, lineno, false);

        string name = trim(input[0]);
        string location = trim(input[1]);
        string priceStr = trim(input[2]);
        string type = trim(input[3]);
        string infoNumber = trim(input[4]);
        string roomsStr = trim(input[5]);
        string bathsStr = trim(input[6]);
        string areaStr = trim(input[7]);
        string ownerIdStr = trim(input[8]);

        for (int i = 0; i < lineno; i++) delete[] input[i];
        delete[] input;

        gotoxy(startX + 2, startY + height + 2);

        if (name.empty() || location.empty() || priceStr.empty() || type.empty() ||
            infoNumber.empty() || roomsStr.empty() || bathsStr.empty() ||
            areaStr.empty() || ownerIdStr.empty()) {
            textattr(12);
            cout << "All fields are required!";
            _getch();
            return;
        }

        if (type != "Buy" && type != "Rent" && type != "buy" && type != "rent") {
            textattr(12);
            cout << "Type must be 'Buy' or 'Rent'!";
            _getch();
            return;
        }

        if (type == "buy") type = "Buy";
        if (type == "rent") type = "Rent";

        double price = stod(priceStr);
        int rooms = stoi(roomsStr);
        int baths = stoi(bathsStr);
        double area = stod(areaStr);
        int ownerId = stoi(ownerIdStr);

        // Validate owner exists using DBManager
        if (!dbManager->ownerExists(ownerId)) {
            textattr(12);
            cout << "Owner ID does not exist!";
            _getch();
            return;
        }

        // Insert property using DBManager
        int newPropertyId = 0;
        if (dbManager->addProperty(name, location, price, type, infoNumber, rooms, baths, area, ownerId, newPropertyId)) {
            textattr(10);
            cout << "Property added successfully! Property ID: " << newPropertyId;
        } else {
            textattr(12);
            cout << "Failed to add property!";
        }
        _getch();
    }

    void deleteProperty(DBManager* dbManager) {
        system("cls");

        // Show all properties in a table format
        textattr(11);
        cout << "====== ALL PROPERTIES ======\n\n";

        vector<Property> props = dbManager->getAllPropertiesAdmin();

        if (props.empty()) {
            textattr(12);
            cout << "No properties found in the system!";
            _getch();
            return;
        }

        displayPropertiesTable(props, false);

        cout << "\n";
        textattr(14);
        cout << "Enter Property ID to delete: ";
        textattr(15);

        int id;
        cin >> id;
        cin.ignore();

        // Check if property exists using DBManager
        string propName;
        if (!dbManager->propertyExists(id, propName)) {
            textattr(12);
            cout << "\nProperty not found!";
            _getch();
            return;
        }

        cout << "\n";
        textattr(14);
        cout << "Are you sure you want to delete property: " << propName << "? (Y/N): ";
        textattr(NORMAL_PEN);

        char confirm = _getch();
        if (confirm != 'Y' && confirm != 'y') {
            cout << "\n";
            textattr(8);
            cout << "Deletion cancelled.";
            _getch();
            return;
        }

        // Delete property using DBManager
        cout << "\n";
        if (dbManager->deleteProperty(id)) {
            textattr(10);
            cout << "Property '" << propName << "' deleted successfully.";
        } else {
            textattr(12);
            cout << "Failed to delete property!";
        }
        _getch();
    }

    void viewPropertiesByOwner(DBManager* dbManager) {
        system("cls");

        textattr(14);
        cout << "====== VIEW PROPERTIES BY OWNER ======\n\n";
        textattr(NORMAL_PEN);

        // Display owners table
        displayOwnersTable(dbManager, 5, 3);

        cout << "\n\n";
        textattr(14);
        cout << "Enter Owner ID: ";
        textattr(15);

        int ownerId;
        cin >> ownerId;
        cin.ignore();

        // Get properties by owner using DBManager
        string ownerName;
        vector<Property> props = dbManager->getPropertiesByOwner(ownerId, ownerName);

        if (props.empty()) {
            textattr(12);
            cout << "\nNo properties found for this owner.\n";
            _getch();
            return;
        }

        system("cls");
        textattr(11);
        cout << "====== Properties for Owner: " << ownerName << " (ID: " << ownerId << ") ======\n\n";
        textattr(240);
        cout << " ID  | NAME              | LOCATION        | PRICE       | TYPE  | CONTACT      | ROOMS | BATHS | AREA    | AVAIL\n";
        textattr(15);

        for (size_t i = 0; i < props.size(); i++) {
            cout << setw(4) << props[i].getId() << " | ";
            cout << setw(17) << props[i].getName().substr(0, 17) << " | ";
            cout << setw(15) << props[i].getLocation().substr(0, 15) << " | ";
            cout << "$" << setw(10) << fixed << setprecision(0) << props[i].getPrice() << " | ";
            cout << setw(5) << props[i].getType() << " | ";
            cout << setw(12) << props[i].getInfoNumber().substr(0, 12) << " | ";
            cout << setw(5) << props[i].getNoOfRooms() << " | ";
            cout << setw(5) << props[i].getNoOfBaths() << " | ";
            cout << setw(7) << fixed << setprecision(1) << props[i].getArea() << " | ";
            cout << (props[i].getAvailable() ? "Yes" : "No") << "\n";
        }

        _getch();
    }

    void lockUnlockProperty(DBManager* dbManager) {
        system("cls");

        textattr(14);
        cout << "====== LOCK/UNLOCK PROPERTY ======\n\n";
        textattr(NORMAL_PEN);

        int id;
        cout << "Enter Property ID to lock/unlock: ";
        cin >> id;
        cin.ignore();

        string propName;
        bool newStatus;

        if (dbManager->togglePropertyAvailability(id, propName, newStatus)) {
            textattr(10);
            cout << "\nProperty '" << propName << "' " << (newStatus ? "unlocked" : "locked") << " successfully.\n";
        } else {
            textattr(12);
            cout << "\nProperty not found or failed to update.\n";
        }
        _getch();
    }

    void updateProperty(DBManager* dbManager) {
        system("cls");

        textattr(14);
        cout << "====== UPDATE PROPERTY ======\n\n";
        textattr(NORMAL_PEN);

        int id;
        cout << "Enter Property ID to update: ";
        cin >> id;
        cin.ignore();

        Property p;
        int currentOwnerId;

        if (!dbManager->getPropertyByIdWithOwner(id, p, currentOwnerId)) {
            textattr(12);
            cout << "\nProperty not found.\n";
            _getch();
            return;
        }

        system("cls");
        int width = 50, height = 24;
        int startX = 5, startY = 2;

        // Draw frame
        for (int y = startY; y <= startY + height; y++) {
            gotoxy(startX, y);
            for (int x = startX; x <= startX + width; x++) {
                if (y == startY || y == startY + height) cout << "=";
                else if (x == startX || x == startX + width) cout << "|";
                else cout << " ";
            }
        }

        // Title
        gotoxy(startX + 15, startY);
        textattr(HIGHLIGHTED_PEN);
        cout << " UPDATE PROPERTY ";
        textattr(NORMAL_PEN);

        // Display current property info
        gotoxy(startX + 2, startY + 2);
        textattr(11);
        cout << "Property ID: " << p.getId();
        textattr(NORMAL_PEN);

        // Labels
        int labelY = startY + 4;
        gotoxy(startX + 2, labelY);      cout << "Name:";
        gotoxy(startX + 2, labelY + 2);  cout << "Location:";
        gotoxy(startX + 2, labelY + 4);  cout << "Price:";
        gotoxy(startX + 2, labelY + 6);  cout << "Type:";
        gotoxy(startX + 2, labelY + 8);  cout << "Contact:";
        gotoxy(startX + 2, labelY + 10); cout << "Rooms:";
        gotoxy(startX + 2, labelY + 12); cout << "Baths:";
        gotoxy(startX + 2, labelY + 14); cout << "Area:";
        gotoxy(startX + 2, labelY + 16); cout << "Owner ID:";

        gotoxy(startX + 2, labelY + 18);
        textattr(8);
        cout << "Blank = keep: " << p.getName().substr(0, 10) << "..";
        textattr(NORMAL_PEN);

        // Display owners table OUTSIDE the box on the right side
        int ownerTableX = startX + width + 5;
        gotoxy(ownerTableX, startY + 2);
        textattr(14);
        cout << "Available Owners:";
        textattr(NORMAL_PEN);
        displayOwnersTable(dbManager, ownerTableX, startY + 3);

        int lineno = 9;
        int maxLen = 30;
        char sr[9] = {32, 32, 48, 32, 32, 48, 48, 48, 48};
        char er[9] = {126, 126, 57, 126, 126, 57, 57, 57, 57};

        int editorX = startX + 15;
        char** editedLines = multiLineEditor(editorX, labelY, maxLen, sr, er, lineno, false);

        string newName = strlen(editedLines[0]) > 0 ? trim(editedLines[0]) : p.getName();
        string newLocation = strlen(editedLines[1]) > 0 ? trim(editedLines[1]) : p.getLocation();
        string newPriceStr = trim(editedLines[2]);
        string newType = strlen(editedLines[3]) > 0 ? trim(editedLines[3]) : p.getType();
        string newContact = strlen(editedLines[4]) > 0 ? trim(editedLines[4]) : p.getInfoNumber();
        string newRoomsStr = trim(editedLines[5]);
        string newBathsStr = trim(editedLines[6]);
        string newAreaStr = trim(editedLines[7]);
        string newOwnerIdStr = trim(editedLines[8]);

        double newPrice = newPriceStr.empty() ? p.getPrice() : stod(newPriceStr);
        int newRooms = newRoomsStr.empty() ? p.getNoOfRooms() : stoi(newRoomsStr);
        int newBaths = newBathsStr.empty() ? p.getNoOfBaths() : stoi(newBathsStr);
        double newArea = newAreaStr.empty() ? p.getArea() : stod(newAreaStr);
        int newOwnerId = newOwnerIdStr.empty() ? currentOwnerId : stoi(newOwnerIdStr);

        for (int i = 0; i < lineno; i++) delete[] editedLines[i];
        delete[] editedLines;

        if (newType != "Buy" && newType != "Rent" && newType != "buy" && newType != "rent") {
            gotoxy(startX + 2, startY + height + 2);
            textattr(12);
            cout << "Type must be 'Buy' or 'Rent'!";
            _getch();
            return;
        }

        // Normalize type
        if (newType == "buy") newType = "Buy";
        if (newType == "rent") newType = "Rent";

        // Validate owner exists using DBManager
        if (!dbManager->ownerExists(newOwnerId)) {
            gotoxy(startX + 2, startY + height + 2);
            textattr(12);
            cout << "Owner ID does not exist!";
            _getch();
            return;
        }

        // Update property using DBManager
        gotoxy(startX + 2, startY + height + 2);
        if (dbManager->updateProperty(p.getId(), newName, newLocation, newPrice, newType, newContact, newRooms, newBaths, newArea, newOwnerId)) {
            textattr(10);
            cout << "Property updated successfully!";
        } else {
            textattr(12);
            cout << "Failed to update property.";
        }

        _getch();
    }

    // ================= VIEW ALL PROPERTIES (ADMIN VERSION) =================
    void viewAllPropertiesAdmin(DBManager* dbManager) {
        system("cls");

        textattr(11);
        cout << "====== ALL PROPERTIES (ADMIN VIEW) ======\n\n";

        vector<Property> props = dbManager->getAllPropertiesAdmin();

        if (props.empty()) {
            textattr(12);
            cout << "No properties found in the system!";
            _getch();
            return;
        }

        textattr(240);
        cout << " ID  | NAME              | LOCATION        | PRICE       | TYPE  | CONTACT      | ROOMS | BATHS | AREA    | AVAIL\n";
        textattr(15);

        for (size_t i = 0; i < props.size(); i++) {
            cout << setw(4) << props[i].getId() << " | ";
            cout << setw(17) << props[i].getName().substr(0, 17) << " | ";
            cout << setw(15) << props[i].getLocation().substr(0, 15) << " | ";
            cout << "$" << setw(10) << fixed << setprecision(0) << props[i].getPrice() << " | ";
            cout << setw(5) << props[i].getType() << " | ";
            cout << setw(12) << props[i].getInfoNumber().substr(0, 12) << " | ";
            cout << setw(5) << props[i].getNoOfRooms() << " | ";
            cout << setw(5) << props[i].getNoOfBaths() << " | ";
            cout << setw(7) << fixed << setprecision(1) << props[i].getArea() << " | ";
            cout << (props[i].getAvailable() ? "Yes" : "No") << "\n";
        }

        _getch();
    }
};

#endif
