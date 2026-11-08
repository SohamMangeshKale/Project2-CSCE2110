/*
Double Check this please
*/

#include "FileUtils.h"
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "HashTable.h"
#include <fstream>
#include <iostream>


// Remove leading and trailing whitespace space, tab, \r, \n, \f, \v
std::string trim(const std::string& text) {

    const char* whitespace = " \t\r\n\f\v";
    size_t first = text.find_first_not_of(whitespace);

    if (first == std::string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Convert ASCII letters to Capital
std::string toUpperCase(const std::string& text) {

    std::string result = text;

    for (size_t i = 0; i < result.size(); i++) {

        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = static_cast<char>(result[i] - 'a' + 'A');
        }

    }
    return result;
}

// Compare two strings ignoring ASCII capitalization

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toUpperCase(a) == toUpperCase(b);
}

// Split on the delimiter and trim every field. "A|" gives two fields ("A" and "").
std::vector<std::string> splitFields(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::string current;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == delimiter) {
            fields.push_back(trim(current));
            current.clear();
        } else {
            current += line[i];
        }
    }
    fields.push_back(trim(current));
    return fields;
}

//Private wale hai
namespace {

// Join directory and file name with a '/'.
std::string buildPath(const std::string& dir, const std::string& fileName) {

    if (dir.empty()) return fileName;
    char last = dir[dir.size() - 1];

    if (last == '/' || last == '\\') return dir + fileName;
    return dir + "/" + fileName;

}

// Open a data file, if failed, print error, treat file as empty 
bool openDataFile(const std::string& dir, const std::string& fileName, std::ifstream& in) {
    in.open(buildPath(dir, fileName).c_str());

    if (!in.is_open()) {
        std::cout << "ERROR: cannot open " << buildPath(dir, fileName)
                  << " - treating " << fileName << " as empty and continuing.\n";
        return false;
    }

    return true;
}

// Read the next record
bool readNextRecord(std::ifstream& in, std::string& record, int& lineNumber) {
    std::string raw;

    while (std::getline(in, raw)) {

        lineNumber++;

        if (lineNumber == 1 && raw.size() >= 3 &&
            static_cast<unsigned char>(raw[0]) == 0xEF &&
            static_cast<unsigned char>(raw[1]) == 0xBB &&
            static_cast<unsigned char>(raw[2]) == 0xBF) {
            raw.erase(0, 3);
        }

        record = trim(raw);

        if (record.empty() || record[0] == '#') {
            continue;
        }

        return true;


    }

    return false;
}

// Print one rejection warning and count

void reject(const std::string& fileName, int lineNumber, const std::string& reason, const std::string& record,  LoadResult& result) {
    std::cout << "WARNING: " << fileName << " line " << lineNumber << ": "<< reason << " [" << record << "]\n";
    result.rejected++;

}

// Print the load summary line (LOAD-4).

void printSummary(const std::string& fileName, const LoadResult& result, const std::string& extra) {
    std::cout << fileName << ": " << result.accepted << " accepted, "<< result.rejected << " rejected";
    if (!extra.empty()) {
        std::cout << " (" << extra << ")";
    }
    std::cout << "\n";
}

// True if any field in  list is empty.
bool hasEmptyField(const std::vector<std::string>& fields) {

    for (size_t i = 0; i < fields.size(); i++) {
    
        if (fields[i].empty()) return true;
    }
    return false;
}

} // end anonymous namespaces


//loaders
// buildings.txt: one nonempty, unique building name per line.
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
                reject(fileName, lineNumber, "wrong field count (expected 1, found " + std::to_string(fields.size()) + ")", record, result);
            } else if (!graph.addBuilding(fields[0])) {
                reject(fileName, lineNumber, "duplicate building name (ignoring case)", record, result);
            } else {
                result.accepted++;
            }

        }

    }

    printSummary(fileName, result, "");
    return result;

}

// connections.txt: BuildingA|BuildingB, both must exist no self, duplicate or reversed duplicate,
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
                reject(fileName, lineNumber, "wrong field count (expected 2, found " +std::to_string(fields.size()) + ")", record, result);
                continue;
            }

            if (hasEmptyField(fields)) {
                reject(fileName, lineNumber, "empty building name", record, result);
                continue;
            }

            ConnectionResult outcome = graph.addConnection(fields[0], fields[1]);

            if (outcome == CONNECTION_ADDED) {
                result.accepted++;
            } else if (outcome == CONNECTION_UNKNOWN_A) {
                reject(fileName, lineNumber, "unknown building '" + fields[0] + "'", record, result);
            } else if (outcome == CONNECTION_UNKNOWN_B) {
                reject(fileName, lineNumber, "unknown building '" + fields[1] + "'", record, result);
            } else if (outcome == CONNECTION_SELF) {
                reject(fileName, lineNumber, "self-connection not allowed", record, result);
            } else {
                reject(fileName, lineNumber, "duplicate connection (including reversed order)", record, result);
            }
        }
    }

    printSummary(fileName, result, "");
    return result;

}

// facilities.txt: FacilityID|FacilityName|BuildingName|Category
// Checks in order: field count, empty fields, ID format, unknown building, duplicate ID
LoadResult loadFacilities(const std::string& dataDir, const CampusGraph& graph, std::vector<Facility>& facilities, HashTable& table) {
    const std::string fileName = "facilities.txt";
    LoadResult result;
    std::ifstream in;

    if (openDataFile(dataDir, fileName, in)) {
        std::string record;
        int lineNumber = 0;

        while (readNextRecord(in, record, lineNumber)) {
            std::vector<std::string> fields = splitFields(record, '|');

            if (fields.size() != 4) {
                reject(fileName, lineNumber, "wrong field count (expected 4, found " +
                       std::to_string(fields.size()) + ")", record, result);
                continue;
            }

            if (hasEmptyField(fields)) {
                reject(fileName, lineNumber, "empty required field", record, result);
                continue;
            }

            // Normalize the ID to Capital BEFORE validating or hashing it.
            std::string id = toUpperCase(fields[0]);

            if (!Facility::isValidId(id)) {
                reject(fileName, lineNumber, "invalid facility ID '" + fields[0] +
                       "' (must be F plus exactly three digits)", record, result);
                continue;
            }

            int buildingIndex = graph.findBuilding(fields[2]);
            if (buildingIndex == -1) {
                reject(fileName, lineNumber, "unknown building '" + fields[2] + "'", record, result);
                continue;
            }

            // Reuse the spelling of a category that was already accepted (ignoring case).
            std::string category = fields[3];
            for (size_t i = 0; i < facilities.size(); i++) {

                if (equalsIgnoreCase(facilities[i].getCategory(), category)) {
                    category = facilities[i].getCategory();
                    break;
                }

            }

            // Building spelling comes from buildings.txt (via the graph).
            Facility facility(id, fields[1], graph.getBuildingName(buildingIndex), category);
            InsertResult outcome = table.insert(facility);

            if (outcome == INSERT_DUPLICATE) {
                reject(fileName, lineNumber, "duplicate facility ID " + id + " (first record kept)",
                       record, result);
                continue;
            }

            if (outcome == INSERT_OK_COLLISION) {
                std::cout << "NOTE: hash collision: " << id << " went to bucket "
                          << table.hashFunction(id) << ", which was already occupied.\n";
            }

            facilities.push_back(facility);   // keeps file order 
            result.accepted++;

        }
    }

    printSummary(fileName, result, table.diagnosticText());
    return result;

}

// departments.txt: DepartmentName|ParentName. The first valid record is the root (parent NONE).
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
                reject(fileName, lineNumber, "wrong field count (expected 2, found " +std::to_string(fields.size()) + ")", record, result);
                continue;
            }

            if (hasEmptyField(fields)) {
                reject(fileName, lineNumber, "empty department or parent name", record, result);
                continue;
            }

            bool parentIsNone = equalsIgnoreCase(fields[1], "NONE");

            if (tree.isEmpty()) {
                // The first valid record must be the root.
                if (!parentIsNone) {
                    reject(fileName, lineNumber, "first department must be the root with parent NONE",record, result);
                } else {
                    tree.addRoot(fields[0]);
                    result.accepted++;
                }
                continue;
            }

            if (parentIsNone) {
                reject(fileName, lineNumber, "root already exists; only one root is allowed", record, result);
                continue;
            }

            AddDepartmentResult outcome = tree.addChild(fields[0], fields[1]);

            if (outcome == ADD_OK) {

                result.accepted++;
            } else if (outcome == ADD_DUPLICATE) {
                reject(fileName, lineNumber, "duplicate department name (ignoring case)", record, result);
            } else {
                reject(fileName, lineNumber, "unknown parent '" + fields[1] + "'", record, result);
            }

        }

    }

    printSummary(fileName, result, "");
    return result;
    
}
