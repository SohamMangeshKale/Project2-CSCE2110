/*
Implements the Facility record. */



#include "Facility.h"
#include "FileUtils.h"


// Store the fields; the ID is normalized to uppercase (case-insensitive IDs).

Facility::Facility(const std::string& idText, const std::string& nameText,
                   const std::string& buildingText, const std::string& categoryText): id(toUpperCase(idText)), name(nameText),building(buildingText), category(categoryText) {}

// Format all 4 fields for display.
std::string Facility::toDisplayString() const {
    return id + " | " + name + " | " + building + " | " + category;
}

// A valid ID has length of 4: 'F' then 3 digits.
bool Facility::isValidId(const std::string& normalizedId) {
    if (normalizedId.size() != 4 || normalizedId[0] != 'F') {
        return false;
    }

    
    for (size_t i = 1; i < 4; i++) {
        if (normalizedId[i] < '0' || normalizedId[i] > '9') {
            return false;
        }
    }

    return true;


}
