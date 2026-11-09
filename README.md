# Project2-CSCE2110
SMART CAMPUS NAVIGATION AND FACILITY ANALYTICS SYSTEM
Project 2 – CSCE 2110
Milestone 1

1. PROJECT DESCRIPTION

This project is a C++ application that manages campus facility information and provides campus navigation and department hierarchy features.

The program uses three major data structures:

- A custom hash table for searching facilities by facility ID.
- An undirected graph to represent campus buildings and walkway connections.
- A general tree to represent the academic department hierarchy.

The program loads campus information from text files, validates records, reports invalid entries, and provides an interactive menu for users.

2. MILESTONE 1 FEATURES

The Milestone 1 implementation includes:

- Loading building, connection, facility, and department data.
- Validating input records and rejecting invalid or duplicate entries.
- Displaying all accepted facilities.
- Searching for a facility by ID using a custom hash table.
- Displaying the campus graph and building degrees.
- Finding reachable buildings using Breadth-First Search (BFS).
- Adding departments.
- Searching for departments.
- Displaying the department hierarchy.
- Performing preorder traversal.
- Handling invalid menu input and empty data structures.

3. PROJECT DIRECTORY STRUCTURE

Project2-CSCE2110/
|
|-- data/
|   |-- buildings.txt
|   |-- connections.txt
|   |-- facilities.txt
|   |-- departments.txt
|
|-- include/
|   |-- Facility.h
|   |-- FileUtils.h
|   |-- HashTable.h
|   |-- CampusGraph.h
|   |-- DepartmentTree.h
|   |-- GraphTreeLoader.h
|   |-- GraphTreeMenu.h
|
|-- src/
|   |-- Facility.cpp
|   |-- FileUtils.cpp
|   |-- HashTable.cpp
|   |-- CampusGraph.cpp
|   |-- DepartmentTree.cpp
|   |-- GraphTreeLoader.cpp
|   |-- GraphTreeMenu.cpp
|   |-- main.cpp
|
|-- testdata/
|   |-- collision/
|   |-- crlf/
|   |-- emptytree/
|   |-- errors/
|   |-- missingfacilities/
|   |-- rootonly/
|
|-- tests/
|   |-- member_a_tests.sh
|   |-- member_b_tests.sh
|   |-- run_tests.sh
|   |-- test_lib.sh
|
|-- README.txt
|-- CONTRIBUTIONS.txt
|-- design_document.txt
|-- test_results.txt
|-- .gitignore

4. BUILD INSTRUCTIONS

The project must be compiled using a C++ compiler such as g++.

Exact build command:
[INSERT THE VERIFIED BUILD COMMAND HERE]

Example only, if compatible with the source files:
g++ -std=c++17 -Iinclude src/*.cpp -o campus

The program should load the four required files in this order:

1. buildings.txt
2. connections.txt
3. facilities.txt
4. departments.txt

6. INPUT DATA FORMAT

buildings.txt:
One building name per line.

connections.txt:
BuildingA|BuildingB

facilities.txt:
FacilityID|FacilityName|BuildingName|Category

departments.txt:
DepartmentName|ParentName

7. TESTING

Additional test files are stored separately in the testdata/ directory. These files are used to test validation, missing files, hash collisions, empty structures, and other edge cases.

Testing outcomes are recorded in test_results.txt.

8. PLATFORM REQUIREMENT

The project must compile and run successfully on the UNT CSE CELL machines. 
