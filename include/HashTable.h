/*
 * File:    HashTable.h
 * Purpose: Custom hash table keyed by facility ID, using separate chaining.
 *          Does NOT use std::hash, unordered_map or map.
 * Author:  Member A
 * Used by: FileUtils (inserts while loading), main.cpp (ID search)
 */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "Facility.h"

// Result of an insertion attempt.
enum InsertResult {
    INSERT_OK,             // stored in an empty bucket
    INSERT_OK_COLLISION,   // stored, but the bucket already held another entry
    INSERT_DUPLICATE       // same ID (ignoring case) already stored; nothing changed
};

class HashTable {
public:
    // Initial number of buckets
    static const int DEFAULT_CAPACITY = 11;

    explicit HashTable(int initialCapacity = DEFAULT_CAPACITY);
    ~HashTable();

    // Tables own linked-list nodes, no copy
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Insert a facility keyed by its (Captial) ID. Duplicates are Denied.
    InsertResult insert(const Facility& facility);

    // Look up by ID (any capitalization). Returns nullptr if not found.
    const Facility* find(const std::string& id) const;

    // Custom string hash: h = (h * 31 + character) mod capacity, over the Capital key.
    int hashFunction(const std::string& key) const;


    int getCapacity() const { return capacity; }
    int getEntryCount() const { return entryCount; }  
    //Check
    int getCollisionCount() const { return collisionCount; }


    // Returns "hash table capacity 11, entries 10, collisions 1".
    std::string diagnosticText() const;

private:
    // One link of a bucket's chain.
    struct Node {
        Facility data;
        Node* next;
        explicit Node(const Facility& f) : data(f), next(nullptr) {}
    };


    std::vector<Node*> buckets;  // each bucket is a chain that can grow without limit
    int capacity;
    int entryCount;
    int collisionCount;          // only counts accepted inserts into a non-empty bucket
};



#endif
