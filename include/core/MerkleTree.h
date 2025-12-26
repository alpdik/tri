/**
 * @file MerkleTree.h
 * @author Alp Dikmen (Integrated by Umut Ertuğrul Daşgın)
 * @brief A Merkle Tree implementation compatible with StagingArea.
 * @version 1.0
 * @date 2025-12-25
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
    enum NodeType {
        BLOB,
        TREE
    };
    
    struct MerkleNode {
        std::string name;
        std::string hash;
        NodeType type;
        
        data_structures::DoublyLinkedList<MerkleNode*> children;

        MerkleNode(std::string n, NodeType t) : name(n), type(t) {}
        
        ~MerkleNode() {
            for (auto it = children.begin(); it != children.end(); ++it) {
                delete *it;
            }
        }
    };

    class MerkleTree {
    public:
        MerkleNode* root;

        explicit MerkleTree(const data_structures::DoublyLinkedList<entities::File>& staged_files) {
            root = new MerkleNode("root", TREE);
            build_from_staging(staged_files);
            
            calculate_hashes_recursive(root);
        }

        ~MerkleTree() {
            delete root;
        }

        std::string get_root_hash() const {
            return root ? root->hash : "";
        }

    private:
        std::string calculate_hash(const std::string& input) {
            std::hash<std::string> hasher;
            std::size_t hash_val = hasher(input);
            std::stringstream ss;
            ss << std::hex << std::setw(16) << std::setfill('0') << hash_val;
            return ss.str();
        }

        void build_from_staging(const data_structures::DoublyLinkedList<entities::File>& files) {
            for (auto it = files.begin(); it != files.end(); ++it) {

                MerkleNode* file_node = new MerkleNode(it->get_path(), BLOB);
                
                std::string header = "blob " + std::to_string(it->get_content().size()) + '\0';
                file_node->hash = calculate_hash(header + it->get_content());

                root->children.push_back(file_node);
            }
            
            sort_children_by_name(root->children);
        }

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

        void sort_children_by_name(data_structures::DoublyLinkedList<MerkleNode*>& kids) {
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
}