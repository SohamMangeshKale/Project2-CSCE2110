#ifndef GRAPHTREEMENU_H
#define GRAPHTREEMENU_H
class CampusGraph;
class DepartmentTree;
void findReachableBuildings(const CampusGraph& graph);   // menu option 5
void addDepartment(DepartmentTree& tree);                // menu option 6
void searchDepartment(const DepartmentTree& tree);       // menu option 7

#endif
