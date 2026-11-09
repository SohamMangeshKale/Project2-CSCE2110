#include "GraphTreeLoader.h"
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include <fstream>
#include <iostream>


// buildings.txt: one nonempty, unique (ignoring case) building name per line.
LoadResult loadBuildings(const std::string& dataDir, CampusGraph& graph) {
    const std::string fileName = "buildings.txt";
    LoadResult result;
    std::ifstream in;
     
    if (openDataFile(dataDir, fileName, in)) {
        std::string record;
        int lineNumber = 0;
        while (readNextRecord(in, record, lineNumber)) {
            std::vector<std::string> fields = splitFields(record, '|');
            if (fields.size() != 1) {
                rejectRecord(fileName, lineNumber, "wrong field count (expected 1, found " + std::to_string(fields.size()) + ")", record, result);
            } else if (!graph.addBuilding(fields[0])) {
                rejectRecord(fileName, lineNumber, "duplicate building name (ignoring case)", record, result);
            } else {
                result.accepted++;
            }
        }
    }

    printLoadSummary(fileName, result, "");
    return result;

}

// connections.txt: BuildingA|BuildingB, both must exist; no self, duplicate or reversed duplicate.
LoadResult loadConnections(const std::string& dataDir, CampusGraph& graph) {
    const std::string fileName = "connections.txt";
    LoadResult result;
    std::ifstream in;

    if (openDataFile(dataDir, fileName, in)) {
        std::string record;
        int lineNumber = 0;
        while (readNextRecord(in, record, lineNumber)) {
            std::vector<std::string> fields = splitFields(record, '|');
            if (fields.size() != 2) {
                rejectRecord(fileName, lineNumber, "wrong field count (expected 2, found " +
                             std::to_string(fields.size()) + ")", record, result);
                continue;
            }
            if (hasEmptyField(fields)) {
                rejectRecord(fileName, lineNumber, "empty building name", record, result);
                continue;
            }
            ConnectionResult outcome = graph.addConnection(fields[0], fields[1]);
            if (outcome == CONNECTION_ADDED) {
                result.accepted++;
            } else if (outcome == CONNECTION_UNKNOWN_A) {
                rejectRecord(fileName, lineNumber, "unknown building '" + fields[0] + "'", record, result);
            } else if (outcome == CONNECTION_UNKNOWN_B) {
                rejectRecord(fileName, lineNumber, "unknown building '" + fields[1] + "'", record, result);
            } else if (outcome == CONNECTION_SELF) {
                rejectRecord(fileName, lineNumber, "self-connection not allowed", record, result);
            } else {
                rejectRecord(fileName, lineNumber, "duplicate connection (including reversed order)",
                             record, result);
            }
        }
    }

    printLoadSummary(fileName, result, "");
    return result;
}

// departments.txt: DepartmentName|ParentName The first valid record is the root (parent NONE).
LoadResult loadDepartments(const std::string& dataDir, DepartmentTree& tree) {
    const std::string fileName = "departments.txt";
    LoadResult result;
    std::ifstream in;

    if (openDataFile(dataDir, fileName, in)) {
        std::string record;
        int lineNumber = 0;
        while (readNextRecord(in, record, lineNumber)) {
            std::vector<std::string> fields = splitFields(record, '|');
            if (fields.size() != 2) {
                rejectRecord(fileName, lineNumber, "wrong field count (expected 2, found " +
                             std::to_string(fields.size()) + ")", record, result);
                continue;
            }
            if (hasEmptyField(fields)) {
                rejectRecord(fileName, lineNumber, "empty department or parent name", record, result);
                continue;
            }
            bool parentIsNone = equalsIgnoreCase(fields[1], "NONE");


            if (tree.isEmpty()) {
                // The first valid record must be the root.
                if (!parentIsNone) {
                    rejectRecord(fileName, lineNumber,
                                 "first department must be the root with parent NONE", record, result);
                } else {
                    tree.addRoot(fields[0]);
                    result.accepted++;
                }
                continue;
            }




            if (parentIsNone) {
                rejectRecord(fileName, lineNumber, "root already exists; only one root is allowed",
                             record, result);
                continue;
            }
            AddDepartmentResult outcome = tree.addChild(fields[0], fields[1]);
            if (outcome == ADD_OK) {
                result.accepted++;
            } else if (outcome == ADD_DUPLICATE) {
                rejectRecord(fileName, lineNumber, "duplicate department name (ignoring case)", record, result);
            } else {
                rejectRecord(fileName, lineNumber, "unknown parent '" + fields[1] + "'", record, result);
            }
        }
    }








    
    printLoadSummary(fileName, result, "");
    return result;
}
