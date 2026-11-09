#include "DepartmentTree.h"
#include "FileUtils.h"
#include <iostream>


DepartmentTree::DepartmentTree() : root(nullptr) {}
DepartmentTree::~DepartmentTree() {

    for (size_t i = 0; i < allNodes.size(); i++) {
        delete allNodes[i];
    }
}

// Linear lookup by name ignoring case.
DepartmentNode* DepartmentTree::findNode(const std::string& name) const {
    std::string cleaned = trim(name);

    for (size_t i = 0; i < allNodes.size(); i++) {

        if (equalsIgnoreCase(allNodes[i]->name, cleaned)) {
            return allNodes[i];
        }
    }
    return nullptr;

}

bool DepartmentTree::contains(const std::string& name) const {
    return findNode(name) != nullptr;
}

// The first department becomes the root   
bool DepartmentTree::addRoot(const std::string& name) {
    std::string cleaned = trim(name);
    if (root != nullptr || cleaned.empty()) {
        return false;
    }
    root = new DepartmentNode(cleaned);
    allNodes.push_back(root);
    return true;
}

// Add a child under an existing parent; siblings stay in insertion order.
AddDepartmentResult DepartmentTree::addChild(const std::string& name, const std::string& parentName) {
    if (root == nullptr) return ADD_NO_ROOT;
    if (findNode(name) != nullptr) return ADD_DUPLICATE;
    DepartmentNode* parent = findNode(parentName);
    if (parent == nullptr) return ADD_UNKNOWN_PARENT;

    DepartmentNode* child = new DepartmentNode(trim(name));
    child->parent = parent;
    parent->children.push_back(child);
    allNodes.push_back(child);
    return ADD_OK;
}

// Show name, parent ("None" for root), direct children and depth (edges from root).
bool DepartmentTree::printDepartmentInfo(const std::string& name) const {
    const DepartmentNode* node = findNode(name);
    if (node == nullptr) {
        return false;
    }

    int depth = 0;
    for (const DepartmentNode* p = node->parent; p != nullptr; p = p->parent) {
        depth++;
    }

    std::cout << "Department: " << node->name << "\n";
    std::cout << "Parent: " << (node->parent == nullptr ? "None" : node->parent->name) << "\n";
    std::cout << "Direct children: " << node->children.size();
    for (size_t i = 0; i < node->children.size(); i++) {
        std::cout << (i == 0 ? " (" : ", ") << node->children[i]->name;
    }

    if (!node->children.empty()) std::cout << ")";
    std::cout << "\n";
    std::cout << "Depth: " << depth << "\n";

    return true;

}

// Recursive preorder: print the node, then each child in order.
void DepartmentTree::printHierarchyFrom(const DepartmentNode* node, int depth) const {
    std::cout << std::string(depth * 4, ' ') << node->name << "\n";

    for (size_t i = 0; i < node->children.size(); i++) {
        printHierarchyFrom(node->children[i], depth + 1);
    }

}

void DepartmentTree::printHierarchy() const {

    if (root == nullptr) {
        std::cout << "The department tree is empty.\n";
        return;
    }

    printHierarchyFrom(root, 0);
}

// Preorder traversal: visit node first, then its children left to right.
void DepartmentTree::collectPreorder(const DepartmentNode* node, std::vector<std::string>& result) const {
    result.push_back(node->name);

    for (size_t i = 0; i < node->children.size(); i++) {
        collectPreorder(node->children[i], result);
    }

}

void DepartmentTree::printPreorder() const {

    if (root == nullptr) {
        std::cout << "The department tree is empty.\n";
        return;
    }

    std::vector<std::string> sequence;
    collectPreorder(root, sequence);
    std::cout << "Preorder traversal: ";

    for (size_t i = 0; i < sequence.size(); i++) {
        if (i > 0) std::cout << " -> ";
        std::cout << sequence[i];
    }
    
    std::cout << "\n";
}
