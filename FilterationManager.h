#ifndef SEARCH_MANAGER_H
#define SEARCH_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include <conio.h>
#include "sqlite3.h"
#include "DBManager.h"
#include "PropertyManager.h"
using namespace std;
string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c){ return tolower(c); });
    return s;
}

class SearchFilter
{
private:
    double maxPrice;
    string type;
    string location;
    int minRooms;
    int minBaths;
    double minArea;

public:
    SearchFilter() : maxPrice(999999999), type(""), location(""),
                     minRooms(0), minBaths(0), minArea(0.0) {}

    double getMaxPrice() const { return maxPrice; }
    string getType() const { return type; }
    string getLocation() const { return location; }
    int getMinRooms() const { return minRooms; }
    int getMinBaths() const { return minBaths; }
    double getMinArea() const { return minArea; }

    void setMaxPrice(double price) { maxPrice = price; }
    void setType(const string &t) { type = t; }
    void setLocation(const string &loc) { location = loc; }
    void setMinRooms(int rooms) { minRooms = rooms; }
    void setMinBaths(int baths) { minBaths = baths; }
    void setMinArea(double area) { minArea = area; }
};

class SearchManager
{

public:
    SearchManager(DBManager *db = nullptr) : dbManager(db) {}

    void setDBManager(DBManager *db)
    {
        dbManager = db;
    }

    void interactiveSearch(sqlite3 *db)
    {
        bool filtering = true;
        int selectedFilter = 0;
        const int numOptions = 9;
        bool needsClear = true;

        while (filtering)
        {
            if (needsClear)
        {
            system("cls");
            needsClear = false;
        }
            drawFilterMenu(selectedFilter, numOptions);
            displayCurrentFilters();


            int key = _getch();
            if (key == 0 || key == 224)
            {
                key = _getch();
                if (key == 72)
                    selectedFilter = (selectedFilter == 0) ? numOptions - 1 : selectedFilter - 1;
                else if (key == 80)
                    selectedFilter = (selectedFilter == numOptions - 1) ? 0 : selectedFilter + 1;
            }
            else if (key == 13)
            {

                if (selectedFilter == 6)
                {
                    viewFilteredResults(db);
                    needsClear = true;
                }
                else if (selectedFilter == 7)
                {
                    resetFilters();
                    needsClear = true;
                }
                else if (selectedFilter == 8)
                {
                    filtering = false;
                    system("cls");
                    setAttr(11);
                    setXY(30, 10);
                    cout << "Returning to Main Menu...";
                    setAttr(15);
                    _getch();
                }
                else
                {
                    handleFilterInput(selectedFilter);
                    needsClear = true;
                }
            }

            else if (key == 27)
            {
                system("cls");
                exit(0);
            }
        }
    }

private:
    DBManager *dbManager;
    PropertyManager pm;
    SearchFilter currentFilter;
    // =============== Helpers Function ===============
    void drawFilterMenu(int selected, int numOptions)
    {

        setAttr(11);
        setXY(20, 2);
        cout << "====================================================";
        setXY(20, 3);
        cout << "           ADVANCED PROPERTY SEARCH & FILTER";
        setXY(20, 4);
        cout << "====================================================";

        const char *filterOptions[] = {
            "Maximum Price ($)",
            "Property Type (Rent/Buy)",
            "Location",
            "Minimum Rooms",
            "Minimum Bathrooms",
            "Minimum Area (m)",
            "Apply Filters & View Results",
            "Reset Filters",
            "Back to Menu"};

        int boxX = 22, boxY = 6, boxWidth = 50;

        for (int i = 0; i <= numOptions * 2; i++)
        {
            setXY(boxX, boxY + i);
            cout << "|";
            setXY(boxX + boxWidth, boxY + i);
            cout << "|";
        }
        for (int i = 0; i <= boxWidth; i++)
        {
            setXY(boxX + i, boxY);
            cout << "-";
            setXY(boxX + i, boxY + numOptions * 2);
            cout << "-";
        }

        for (int i = 0; i < numOptions; i++)
        {
            int yPos = boxY + 1 + i * 2;
            setXY(boxX + 2, yPos);
            setAttr(i == selected ? 240 : 15);
            cout << left << setw(30) << filterOptions[i];
        }
        setAttr(15);
    }

    void displayCurrentFilters()
    {
        setAttr(14);
        int x = 75, y = 6;
        setXY(x, y);
        cout << "CURRENT FILTERS:";

        setAttr(10);
        setXY(x, y += 2);
        cout << "Max Price:   $" << fixed << setprecision(2) << currentFilter.getMaxPrice();
        setXY(x, ++y);
        cout << "Type:        " << (currentFilter.getType().empty() ? "Any" : currentFilter.getType());
        setXY(x, ++y);
        cout << "Location:    " << (currentFilter.getLocation().empty() ? "Any" : currentFilter.getLocation());
        setXY(x, ++y);
        cout << "Rooms        " << currentFilter.getMinRooms();
        setXY(x, ++y);
        cout << "Bathrooms    " << currentFilter.getMinBaths();
        setXY(x, ++y);
        cout << "Area         " << fixed << setprecision(0) << currentFilter.getMinArea() << " m";

        setAttr(15);
    }

    void handleFilterInput(int filterIndex)
    {
        system("cls");
        setAttr(14);
        setXY(30, 5);

        switch (filterIndex)
        {
        case 0:
        {
            cout << "Enter Maximum Price ($): ";
            double tempMaxPrice;
            cin >> tempMaxPrice;
            currentFilter.setMaxPrice(tempMaxPrice);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                currentFilter.setMaxPrice(999999999);
            }
            break;
        }

        case 1:
        {
            cout << "Enter Property Type (Rent/Buy or empty): ";
            cin.clear();
            string tempFilterType;
            getline(cin, tempFilterType);
            if (toLower(tempFilterType)=="rent")
                currentFilter.setType("Rent");
            else if (toLower(tempFilterType)== "buy" )
                currentFilter.setType("Buy");
            else if (!tempFilterType.empty())
                currentFilter.setType("");
            break;
        }

        case 2:
        {
            cout << "Enter Location (partial match): ";
            cin.clear();
            string tempLocation;
            getline(cin, tempLocation);
            currentFilter.setLocation(tempLocation);
            break;
        }

        case 3:
        {
            cout << "Enter Minimum Rooms: ";
            int tempMinRooms;
            cin >> tempMinRooms;
            currentFilter.setMinRooms(tempMinRooms);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                currentFilter.setMinRooms(0);
            }
            break;
        }

        case 4:
        {
            cout << "Enter Minimum Bathrooms: ";
            int tempMinBaths;
            cin >> tempMinBaths;
            currentFilter.setMinBaths(tempMinBaths);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                currentFilter.setMinBaths(0);
            }
            break;
        }

        case 5:
        {
            cout << "Enter Minimum Area (m): ";
            double tempMinArea;
            cin >> tempMinArea;
            currentFilter.setMinArea(tempMinArea);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                currentFilter.setMinArea(0);
            }
            break;
        }
        }

        if (filterIndex < 6)
        {
            setAttr(10);
            setXY(30, 7);
            cout << "Filter updated successfully!";
            setAttr(15);
            setXY(30, 9);
            cout << "Press any key to continue...";
            _getch();
            system("cls");

        }
    }

    void viewFilteredResults(sqlite3 *db)
    {
        auto props = dbManager->filterProperties(currentFilter.getMaxPrice(), currentFilter.getType(), currentFilter.getLocation(), currentFilter.getMinRooms(), currentFilter.getMinBaths(), currentFilter.getMinArea());
        pm.DisplayProperties(props, db, dbManager);
    }

    void resetFilters()
    {
        currentFilter = SearchFilter();
        system("cls");
        setAttr(10);
        setXY(30, 10);
        cout << "All filters have been reset!";
        setAttr(15);
        setXY(30, 12);
        cout << "Press any key to continue...";
        _getch();
    }
};

#endif
