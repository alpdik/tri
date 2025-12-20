#!/usr/bin/env sh
set -eu

# Run this from: tri/merkle_tree
# Builds: merkle_tree_test

g++ -std=c++17 -I ../include \
  merkle_tree.cpp test_case.cpp \
  -lssl -lcrypto \
  -o merkle_tree_test