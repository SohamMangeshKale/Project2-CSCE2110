#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H
#include <string>
#include <vector>

// One department in the tree.
struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;                    // nullptr for the root
    std::vector<DepartmentNode*> children;     // insertion order
    explicit DepartmentNode(const std::string& departmentName): 
    name(departmentName), parent(nullptr) {}
};

// Result of adding a non-root department.
enum AddDepartmentResult {
    ADD_OK,
    ADD_DUPLICATE,        // name already exists (ignoring case)
    ADD_UNKNOWN_PARENT,   // parent not in tree
    ADD_NO_ROOT           // tree is empty, so a root must be added first
};


class DepartmentTree {
public:
    DepartmentTree();
    ~DepartmentTree();

    // The tree owns heap nodes, so copying is disabled.
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;
    bool isEmpty() const { return root == nullptr; }
    int getSize() const { return static_cast<int>(allNodes.size()); }
    bool addRoot(const std::string& name);
    AddDepartmentResult addChild(const std::string& name, const std::string& parentName);
    bool contains(const std::string& name) const;
    bool printDepartmentInfo(const std::string& name) const;
    void printHierarchy() const;

    // Display the preorder sequence on one line, without indentation.
    void printPreorder() const;

private:
    DepartmentNode* findNode(const std::string& name) const;
    void printHierarchyFrom(const DepartmentNode* node, int depth) const;
    void collectPreorder(const DepartmentNode* node, std::vector<std::string>& result) const;

    DepartmentNode* root;
    std::vector<DepartmentNode*> allNodes;   // every node, in insertion order (used for lookup and cleanup)
};


//Needs Checking//


#endif
