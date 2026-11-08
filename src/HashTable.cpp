/*
Implements the separate-chaining hash table for facility IDs.
Rejected duplicate IDs never change the collision count.
*/


#include "HashTable.h"
#include "FileUtils.h"

// Creates 'initialCapacity' empty buckets (at least 1).
HashTable::HashTable(int initialCapacity)
    : buckets(initialCapacity > 0 ? initialCapacity : 1, nullptr),
      capacity(initialCapacity > 0 ? initialCapacity : 1),
      entryCount(0),
      collisionCount(0) {}


// Free every node in every chain.
HashTable::~HashTable() {
    for (size_t i = 0; i < buckets.size(); i++) {
        Node* current = buckets[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }

    }

}



// Polynomial string hash on the Capital key, reduced modulo the capacity
// at every step so the value never overflows.

int HashTable::hashFunction(const std::string& key) const {
    std::string normalized = toUpperCase(key);
    unsigned long hash = 0;

    for (size_t i = 0; i < normalized.size(); i++) {
        hash = (hash * 31 + static_cast<unsigned char>(normalized[i])) % capacity;
    }
    
    return static_cast<int>(hash);
}



// Insert a facility at the END of its bucket's chain

InsertResult HashTable::insert(const Facility& facility) {
    std::string key = toUpperCase(facility.getId());
    int index = hashFunction(key);


    // Check the chain
    Node* current = buckets[index];
    Node* last = nullptr;

    while (current != nullptr) {
        if (current->data.getId() == key) {
            return INSERT_DUPLICATE;
        }

        last = current;
        current = current->next;

    }

    // Key is new, so this insertion is accepted
    Node* newNode = new Node(facility);

    bool collided = (last != nullptr);   // bucket was already used

    if (collided) {
        last->next = newNode;
        collisionCount++;
    } else {
        buckets[index] = newNode;
    }
    entryCount++;
    return collided ? INSERT_OK_COLLISION : INSERT_OK;
}

// Hash the key, then search ONLY one bucket's chain 
const Facility* HashTable::find(const std::string& id) const {
    std::string key = toUpperCase(trim(id));

    Node* current = buckets[hashFunction(key)];

    //Needs Review
    while (current != nullptr) {
        if (current->data.getId() == key) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// Text used in the facilities load summary
std::string HashTable::diagnosticText() const {
    return "hash table capacity " + std::to_string(capacity) +
           ",entries " + std::to_string(entryCount) +
           ",collisions " + std::to_string(collisionCount);
}
