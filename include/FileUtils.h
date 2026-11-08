/*
This is correct, No  Review needed
String helpers
*/

#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <string>
#include <vector>
#include "Facility.h"


class CampusGraph;
class HashTable;
class DepartmentTree;


// Counts reported in each load summary line.
struct LoadResult {
    int accepted;
    int rejected;
    LoadResult() : accepted(0), rejected(0) {}
};


// string helpers 
std::string trim(const std::string& text);                   
std::string toUpperCase(const std::string& text);            
bool equalsIgnoreCase(const std::string& a, const std::string& b);
std::vector<std::string> splitFields(const std::string& line, char delimiter);  // fields are trimmed

// file loaders (each prints warnings and one summary line)

LoadResult loadBuildings(const std::string& dataDir, CampusGraph& graph);
LoadResult loadConnections(const std::string& dataDir, CampusGraph& graph);
LoadResult loadFacilities(const std::string& dataDir, const CampusGraph& graph,std::vector<Facility>& facilities, HashTable& table);
LoadResult loadDepartments(const std::string& dataDir, DepartmentTree& tree);



#endif
