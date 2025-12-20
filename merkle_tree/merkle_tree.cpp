// merkle_tree/merkle_tree.cpp

#include "merkle_tree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <openssl/sha.h>

namespace fs = std::filesystem;

static std::string calculate_sha1(const std::string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) ss << std::setw(2) << (int)hash[i];
    return ss.str();
}

// Simple in-place sort for DoublyLinkedList by swapping stored pointers.
static void sort_children_by_name(data_structures::DoublyLinkedList<MerkleNode*>& kids) {
    if (kids.size() < 2) return;

    for (auto it = kids.begin(); it != kids.end(); ++it) {
        auto jt = it;
        ++jt;
        for (; jt != kids.end(); ++jt) {
            if ((*jt)->name < (*it)->name) {
                std::swap(*it, *jt);
            }
        }
    }
}

MerkleNode::MerkleNode(std::string n, NodeType t, std::string p)
    : name(std::move(n)), type(t), fullPath(std::move(p)) {}

MerkleNode::~MerkleNode() {
    for (auto child : children) {
        delete child;
    }
}

MerkleTree::MerkleTree(std::string path) {
    if (!fs::exists(path)) {
        std::cerr << "Error: Path does not exist: " << path << std::endl;
        root = nullptr;
        return;
    }
    root = buildRecursive(path);
}

MerkleTree::~MerkleTree() {
    delete root;
}

std::string MerkleTree::readFileContent(const std::string& path) {
    std::ifstream t(path, std::ios::binary);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

MerkleNode* MerkleTree::buildRecursive(const std::string& currentPath) {
    fs::path p(currentPath);
    std::string name = p.filename().string();

    if (!fs::is_directory(p)) {
        MerkleNode* node = new MerkleNode(name, BLOB, currentPath);
        std::string content = readFileContent(currentPath);
        std::string header = "blob " + std::to_string(content.size()) + '\0';
        node->hash = calculate_sha1(header + content);
        return node;
    }

    MerkleNode* dirNode = new MerkleNode(name, TREE, currentPath);

    for (const auto& entry : fs::directory_iterator(currentPath)) {
        auto filename = entry.path().filename().string();
        if (!filename.empty() && filename[0] == '.') continue; // Skip hidden
        MerkleNode* child = buildRecursive(entry.path().string());
        dirNode->children.push_back(child);
    }

    sort_children_by_name(dirNode->children);

    std::string dirContent = "tree ";
    for (auto child : dirNode->children) {
        dirContent += child->hash + child->name;
    }
    dirNode->hash = calculate_sha1(dirContent);

    return dirNode;
}

// IDK if this function is neccesary or not but I added it anyway.
void MerkleTree::printTree(MerkleNode* node, int depth) {
    if (!node) return;
    std::string indent(depth * 4, ' ');
    std::string typeStr = (node->type == TREE) ? "[DIR] " : "[FILE]";
    std::cout << indent << typeStr << " " << node->name
              << " (" << node->hash << ")" << std::endl;

    for (auto child : node->children) {
        printTree(child, depth + 1);
    }
}

