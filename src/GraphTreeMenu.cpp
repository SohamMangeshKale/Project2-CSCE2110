#include "GraphTreeMenu.h"
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "FileUtils.h"
#include <iostream>
#include <string>

// Option 5: BFS from a user-entered building.
void findReachableBuildings(const CampusGraph& graph) {
    std::string name;
    if (!promptLine("Enter starting building: ", name)) return;
    if (name.empty()) {
        std::cout << "Error: building name cannot be blank.\n";
        return;
    }
    if (!graph.printReachable(name)) {
        std::cout << "Error: unknown building '" << name << "'.\n";
    }
}


// Option 6: add a department (in memory only; departments.txt is never changed).
void addDepartment(DepartmentTree& tree) {
    std::string name;
    if (!promptLine("Enter new department name: ", name)) return;
    if (name.empty()) {
        std::cout << "Error: department name cannot be blank.\n";
        return;
    }

    // Empty tree: the first department becomes the root, no parent prompt.
    if (tree.isEmpty()) {
        tree.addRoot(name);
        std::cout << "Added '" << name << "' as the root department.\n";
        return;
    }

    if (tree.contains(name)) {
        std::cout << "Error: department '" << name << "' already exists (names ignore capitalization).\n";
        return;
    }
    std::string parent;

    if (!promptLine("Enter parent department name: ", parent)) return;
    
    if (parent.empty()) {
        std::cout << "Error: parent name cannot be blank.\n";
        return;
    }
    AddDepartmentResult outcome = tree.addChild(name, parent);
    if (outcome == ADD_OK) {
        std::cout << "Added '" << name << "' under '" << parent << "'.\n";
    } else if (outcome == ADD_UNKNOWN_PARENT) {
        std::cout << "Error: unknown parent department '" << parent << "'. Tree unchanged.\n";
    } else {
        std::cout << "Error: department '" << name << "' already exists. Tree unchanged.\n";
    }
}


// Option 7: search a department by name.
void searchDepartment(const DepartmentTree& tree) {
    std::string name;
    if (!promptLine("Enter department name: ", name)) return;
    if (name.empty()) {
        std::cout << "Error: department name cannot be blank.\n";
        return;
    }

    if (!tree.printDepartmentInfo(name)) {
        std::cout << "Not found: no department named '" << name << "'.\n";
    }
}

//This seems correct, dont change as of now