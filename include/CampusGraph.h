
#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H
#include <string>
#include <vector>



//trying to add a walkway.
enum ConnectionResult {
    CONNECTION_ADDED,
    CONNECTION_UNKNOWN_A,   // first building  not in the graph
    CONNECTION_UNKNOWN_B,   // second building not in the graph
    CONNECTION_SELF,        // building connected itself
    CONNECTION_DUPLICATE    // already connected
};


class CampusGraph {

public:
    CampusGraph();

    // Add a building. Returns false if name is empty or already exists 
    bool addBuilding(const std::string& name);

    // Index of  building or -1 if unknown
    int findBuilding(const std::string& name) const;

    // Building name with spelling from buildings.txt
    const std::string& getBuildingName(int index) const;

    // Add undirected connection, updating both endpoints
    ConnectionResult addConnection(const std::string& nameA, const std::string& nameB);

    int getBuildingCount() const { return static_cast<int>(names.size()); }
    int getConnectionCount() const { return connectionCount; }
    int getDegree(int index) const;

    // Display the adjacency list, can be empty
    void printGraph() const;

    // Display total buildings and degree (Needs Checking)
    void printCountsAndDegrees() const;

    // BFS using a queue. Fills visitOrder, returns false for unknown start
    bool bfs(const std::string& startName, std::vector<int>& visitOrder,
             std::vector<int>& distanceFrom) const;

    // Run BFS and print result. Returns false for an unknown start building
    bool printReachable(const std::string& startName) const;

private:
    std::vector<std::string> names;              // buildings in insertion order
    std::vector<std::vector<int> > adjacency;    // adjacency[i] = neighbors of i, in insertion order
    int connectionCount;                         // each undirected edge counted once
};


#endif
