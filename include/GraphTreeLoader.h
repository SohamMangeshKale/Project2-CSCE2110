#ifndef GRAPHTREELOADER_H
#define GRAPHTREELOADER_H
#include <string>
#include "FileUtils.h"
class CampusGraph;
class DepartmentTree;
LoadResult loadBuildings(const std::string& dataDir, CampusGraph& graph);
LoadResult loadConnections(const std::string& dataDir, CampusGraph& graph);
LoadResult loadDepartments(const std::string& dataDir, DepartmentTree& tree);



#endif
