//soham finished till Void SearchDacility as discussed
// Needs commit
//Needs Review


#include <iostream>
#include <string>
#include <vector>
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "Facility.h"
#include "FileUtils.h"
#include "HashTable.h"

// All data the menu works with,

struct CampusData {
    CampusGraph graph;
    std::vector<Facility> facilities;   // accepted facilities in file order
    HashTable idTable;                  // custom hash table for ID lookup
    DepartmentTree departments;
};


// input helpers 

// Read one full line, Returns false when input has ended so loops cannot run forever
bool readLine(std::string& line) {
    return static_cast<bool>(std::getline(std::cin, line));
}

// Show a prompt and read a trimmed lineReturns false at end of input.
bool prompt(const std::string& message, std::string& answer) {
    std::cout << message;
    if (!readLine(answer)) {
        return false;
    }
    answer = trim(answer);
    return true;
}

// Read a menu choice. Returns -1 for invalid inputafter printing an error, and -2 at end of input. Reading whole lines means bad input never leaves the stream in a failed state

int readMenuChoice() {
    std::string text;

    if (!prompt("Enter your choice: ", text)) {
        return -2;
    }

    if (text.empty()) {
        std::cout << "Invalid input: please enter a number from 1 to 10.\n";
        return -1;
    }

    if (text.size() > 6) {
        std::cout << "Invalid input: '" << text << "' is not a valid menu number.\n";
        return -1;
    }

    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            std::cout << "Invalid input: '" << text << "' is not a number. Enter 1 to 10.\n";
            return -1;
        }

    }

    return std::stoi(text);

}


void printMenu() {
    std::cout << "\n===== Smart Campus Navigation - Milestone 1 =====\n"<< " 1. View all facilities\n"<< " 2. Search facility by ID\n"<< " 3. View campus graph\n"<< " 4. Display graph counts and degrees\n"<< " 5. Find reachable buildings (BFS)\n"<< " 6. Add department\n"<< " 7. Search department\n"<< " 8. Display department hierarchy\n"<< " 9. Run preorder traversal\n" << "10. Exit\n";
}

// menu actions ->

// Option 1: list facilities in file order with a total

void viewAllFacilities(const CampusData& data) {

    if (data.facilities.empty()) {
        std::cout << "No facilities are loaded.\n";
        return;
    }

    std::cout << "Facilities (ID | Name | Building | Category):\n";

    for (size_t i = 0; i < data.facilities.size(); i++) {
        std::cout << "  " << data.facilities[i].toDisplayString() << "\n";
    }

    std::cout << "Total facilities: " << data.facilities.size() << "\n";
}

// Option 2: ID search through the hash table (three outcomes).

void searchFacilityById(const CampusData& data) {
    std::string input;

    if (!prompt("Enter facility ID (example F001): ", input)) 
    return;

    if (input.empty()) {
        std::cout << "Error: the ID cannot be blank. An ID is F followed by exactly three digits.\n";
        return;
    }

    std::string id = toUpperCase(input); 

    if (!Facility::isValidId(id)) {
        std::cout << "Invalid format: an ID is F followed by exactly three digits.\n";
        return;
    }

    const Facility* found = data.idTable.find(id);

    if (found == nullptr) {
        std::cout << "Not found: no facility with ID " << id << "\n";
    } else {
        std::cout << "Found: " << found->toDisplayString() << "\n";
    }
    
}