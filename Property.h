#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>

using namespace std;

class Property
{
private:
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

public:

    Property() : id(0), name(""), location(""), price(0.0), type(""),
                 available(0), infoNumber(""), noOfRooms(0), noOfBaths(0), area(0.0) {}


    int getId() const { return id; }
    string getName() const { return name; }
    string getLocation() const { return location; }
    double getPrice() const { return price; }
    string getType() const { return type; }
    int getAvailable() const { return available; }
    string getInfoNumber() const { return infoNumber; }
    int getNoOfRooms() const { return noOfRooms; }
    int getNoOfBaths() const { return noOfBaths; }
    double getArea() const { return area; }


    void setId(int i) { id = i; }
    void setName(const string &n) { name = n; }
    void setLocation(const string &l) { location = l; }
    void setPrice(double p) { price = p; }
    void setType(const string &t) { type = t; }
    void setAvailable(int a) { available = a; }
    void setInfoNumber(const string &info) { infoNumber = info; }
    void setNoOfRooms(int rooms) { noOfRooms = rooms; }
    void setNoOfBaths(int baths) { noOfBaths = baths; }
    void setArea(double a) { area = a; }
};

#endif
