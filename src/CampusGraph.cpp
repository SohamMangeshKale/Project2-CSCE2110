
#include "CampusGraph.h"
#include "FileUtils.h"
#include <iostream>
#include <queue>


CampusGraph::CampusGraph() : connectionCount(0) {}


// Add a new building with an empty neighbor list
bool CampusGraph::addBuilding(const std::string& name) {
    std::string cleaned = trim(name);

    if (cleaned.empty() || findBuilding(cleaned) != -1) {
        return false;
    }

    names.push_back(cleaned);
    adjacency.push_back(std::vector<int>());


    return true;
}

// Linear search by name, ignoring case; returns -1 when not found,
int CampusGraph::findBuilding(const std::string& name) const {
    std::string cleaned = trim(name);

    for (size_t i = 0; i < names.size(); i++) {

        if (equalsIgnoreCase(names[i], cleaned)) {
            return static_cast<int>(i) ;
        }

    }
    return -1;
}

const std::string& CampusGraph::getBuildingName(int index) const {
    return names[index] ;
}

// Validate both buildings, then update both neighbor lists (undirected).
ConnectionResult CampusGraph::addConnection(const std::string& nameA, const std::string& nameB) {
    int a = findBuilding(nameA);
    int b = findBuilding(nameB) ;

    if (a == -1) return CONNECTION_UNKNOWN_A;

    if (b == -1) return CONNECTION_UNKNOWN_B;

    if (a == b) return CONNECTION_SELF;

    // Duplicate check covers reversed pairs because both lists are always updated together.
    for (size_t i = 0; i < adjacency[a].size(); i++) {

        if (adjacency[a][i] == b) {
            return CONNECTION_DUPLICATE;
        }

    }

    adjacency[a].push_back(b);
    adjacency[b].push_back(a);

    connectionCount++;   // counted once, not once per endpoint
    return CONNECTION_ADDED;
}

// Degree = number of distinct neighbors.
int CampusGraph::getDegree(int index) const {
    return static_cast<int>(adjacency[index].size());
}

// Print "Building: neighbor1, neighbor2" for each building in insertion order.
void CampusGraph::printGraph() const {

    if (names.empty()) {
        std::cout << "No buildings are loaded.\n";
        return;
    }

    std::cout << "Campus graph (adjacency list):\n";

    for (size_t i = 0; i < names.size(); i++) {
        std::cout << "  " << names[i] << ": ";

        if (adjacency[i].empty()) {
            std::cout << "(no connections)";
        }

        for (size_t j = 0; j < adjacency[i].size(); j++) {
            if (j > 0) std::cout << ", ";
            std::cout << names[adjacency[i][j]];
        }

        std::cout << "\n";

    }

}

// Print totals and one degree/building
void CampusGraph::printCountsAndDegrees() const {

    if (names.empty()) {
        std::cout << "No buildings are loaded.\n";
        std::cout << "Total buildings: 0\nTotal connections: 0\n";
        return;
    }

    std::cout << "Total buildings: " << names.size() << "\n";
    std::cout << "Total connections: " << connectionCount << "\n";
    std::cout << "Degree of each building:\n";

    for (size_t i = 0; i < names.size(); i++) {
        std::cout << "  " << names[i] << ": " << adjacency[i].size() << "\n";
    }

}

// Standard BFS: a queue plus a visited array. A building is marked visited when it is ENQUEUED
bool CampusGraph::bfs(const std::string& startName, std::vector<int>& visitOrder,
                      std::vector<int>& distanceFrom) const {
    int start = findBuilding(startName);

    if (start == -1) {
        return false;
    }

    visitOrder.clear();
    distanceFrom.assign(names.size(), -1);

    std::vector<bool> visited(names.size(), false);
    std::queue<int> waiting;

    visited[start] = true;
    distanceFrom[start] = 0;
    waiting.push(start);

    while (!waiting.empty()) {

        int current = waiting.front();

        waiting.pop();

        visitOrder.push_back(current);

        // Neighbors are used in insertion order (never sorted).
        for (size_t i = 0; i < adjacency[current].size(); i++) {
            int neighbor = adjacency[current][i];

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distanceFrom[neighbor] = distanceFrom[current] + 1;
                waiting.push(neighbor);
            }

        }

    }

    return true;
}

// Run BFS and display each reachable building with its distance.
bool CampusGraph::printReachable(const std::string& startName) const {
    std::vector<int> order;
    std::vector<int> distance;

    if (!bfs(startName, order, distance)) {
        return false;
    }

    std::cout << "Buildings reachable from " << names[findBuilding(startName)]
              << " (BFS visit order):\n";

    for (size_t i = 0; i < order.size(); i++) {
        std::cout << "  " << names[order[i]] << " (distance " << distance[order[i]] << ")\n";
    }

    std::cout << "Reachable: " << order.size() << " of " << names.size() << " buildings\n";
    return true;
    
}
