/**
 * @file MerkleTree.h
 * @brief Defines Merkle tree structures for content hashing.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın / Ömer Kağan Zafer.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <functional>
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/File.h"

namespace core {

/**
 * @brief Identifies the type of a Merkle tree node.
 */
enum NodeType {
    BLOB,
    TREE
};

/**
 * @brief Represents a node in the Merkle tree.
 * * BLOB nodes represent file content
 * * TREE nodes group child nodes
 * * Each node owns its children
 */
struct MerkleNode {

    std::string name;
    std::string hash;
    NodeType type;
    data_structures::DoublyLinkedList<MerkleNode*> children;

    /**
     * @brief Creates a node with a name and type.
     * @param n Node name.
     * @param t Node type.
     */
    MerkleNode(std::string n, NodeType t) : name(n), type(t) {}

    /**
     * @brief Releases all owned child nodes.
     */
    ~MerkleNode() {
        for (auto it = children.begin(); it != children.end(); ++it) {
            delete *it;
        }
    }
};

/**
 * @brief Builds a Merkle tree from staged files.
 * * Owns the root node
 * * Produces deterministic hashes
 */
class MerkleTree {
public:
    MerkleNode* root;

    /**
     * @brief Constructs a tree from staged files.
     * @param staged_files Files to include in the tree.
     */
    explicit MerkleTree(const data_structures::DoublyLinkedList<entities::File>& staged_files) {
        root = new MerkleNode("root", TREE);
        build_from_staging(staged_files);
        calculate_hashes_recursive(root);
    }

    /**
     * @brief Releases the entire tree structure.
     */
    ~MerkleTree() {
        delete root;
    }

    /**
     * @brief Returns the root hash.
     * @return Root hash or empty string.
     */
    std::string get_root_hash() const {
        return root ? root->hash : "";
    }

private:
    /**
     * @brief Computes a hash from input data.
     * @param input Data to hash.
     * @return Hexadecimal hash string.
     */
    std::string calculate_hash(const std::string& input) {
        std::hash<std::string> hasher;
        std::size_t hash_val = hasher(input);
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << hash_val;
        return ss.str();
    }

    /**
     * @brief Creates blob nodes from staged files.
     * @param files Staged files.
     */
    void build_from_staging(const data_structures::DoublyLinkedList<entities::File>& files) {
        for (auto it = files.begin(); it != files.end(); ++it) {

            MerkleNode* file_node = new MerkleNode(it->get_path(), BLOB);

            std::string header =
                "blob " + std::to_string(it->get_content().size()) + '\0';
            file_node->hash = calculate_hash(header + it->get_content());

            root->children.push_back(file_node);
        }

        sort_children_by_name(root->children);
    }

    /**
     * @brief Computes hashes bottom-up.
     * @param node Current tree node.
     */
    void calculate_hashes_recursive(MerkleNode* node) {
        if (!node) return;
        if (node->type == BLOB) return;

        std::string combined_content = "tree ";
        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            calculate_hashes_recursive(*it);
            combined_content += (*it)->hash + (*it)->name;
        }

        node->hash = calculate_hash(combined_content);
    }

    /**
     * @brief Sorts child nodes by name.
     * @param kids Child node list.
     */
    void sort_children_by_name(
        data_structures::DoublyLinkedList<MerkleNode*>& kids
    ) {
        if (kids.empty()) return;

        bool swapped;
        do {
            swapped = false;
            auto it = kids.begin();
            auto next_it = kids.begin();
            ++next_it;

            while (next_it != kids.end()) {
                if ((*it)->name > (*next_it)->name) {
                    MerkleNode* temp = *it;
                    *it = *next_it;
                    *next_it = temp;
                    swapped = true;
                }
                ++it;
                ++next_it;
            }
        } while (swapped);
    }
};

} // namespace core
