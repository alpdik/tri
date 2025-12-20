// merkle_tree/merkle_tree.h
// merkle_tree/merkle_tree.cpp
/**
 * @file merkle_tree.h
 * @author Alp Dikmen
 * @brief A simple merkle tree implementation using UED's dll
 * @version 0.1
 * @date 2025-12-20
 *
 * @copyright Copyright (c) 2025
 *
 */
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