// merkle_tree/merkle_tree.h
#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include "data_structures/DoublyLinkedList.h"

enum NodeType {
    BLOB,
    TREE
};

struct MerkleNode {
    std::string name;
    std::string hash;
    NodeType type;
    std::string fullPath;

    data_structures::DoublyLinkedList<MerkleNode*> children;

    MerkleNode(std::string n, NodeType t, std::string p);
    ~MerkleNode();
};

class MerkleTree {
public:
    MerkleNode* root;

    explicit MerkleTree(std::string path);
    ~MerkleTree();

    void printTree(MerkleNode* node, int depth = 0);

private:
    std::string readFileContent(const std::string& path);
    MerkleNode* buildRecursive(const std::string& currentPath);
};

#endif // MERKLE_TREE_H