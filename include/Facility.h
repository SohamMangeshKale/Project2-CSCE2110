/*
Declares the Facility record (ID, name, building, category).
Member A
 HashTable, FileUtils
 Needs Review before Final, Check karna hai
 */
#ifndef FACILITY_H
#define FACILITY_H


#include <string>

class Facility {
public:

    // Builds a facility.
    Facility(const std::string& id, const std::string& name,
             const std::string& building, const std::string& category);


    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getBuilding() const { return building; }
    const std::string& getCategory() const { return category; }


    // One display line
    std::string toDisplayString() const;

    // True if the ID is 'F' followed by exactly 3 digits.
    static bool isValidId(const std::string&  normalizedId);


private:
    std::string id;        // always Capital
    std::string name;
    std::string building;  // spelling taken frm buildings.txt
    std::string category;
};

#endif
