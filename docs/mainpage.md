# Tri VCS - Version Control System

## Overview

**Tri VCS** is a version control system built from the ground up in C++. It provides core Git-like functionality including file staging, commits, branching, merging, and commit history management.

## Features

### Core Functionality

- **File Staging**: Stage files with content for commit
- **Commits**: Create commits with messages and author information
- **Branching**: Create and manage multiple branches
- **Branch Switching**: Checkout different branches
- **Merging**: Merge branches with conflict detection
- **Commit History**: View commit logs and history
- **File Viewing**: Built-in file viewer to inspect repository contents

### Architecture

The system is built using several key components:

#### Core Modules

- **Repository**: Main repository management
- **StagingArea**: Handles file staging operations
- **StorageEngine**: Manages file storage and retrieval
- **MerkleTree**: Implements Merkle tree for content addressing
- **GraphAlgorithms**: Graph traversal algorithms for commit DAGs
- **GraphManager**: Manages commit graph structure
- **MergeEngine**: Handles branch merging operations
- **ReferenceManager**: Manages branch and tag references

#### Data Structures

- **DoublyLinkedList**: Custom doubly linked list implementation
- **HashTable**: Hash table for efficient lookups
- **Stack**: Stack data structure
- **Queue**: Queue data structure

#### Entities

- **Commit**: Represents a commit in the repository
- **Branch**: Represents a branch reference
- **File**: Represents a file in the repository

## Getting Started

### Building the Project

```bash
make
```

### Running the Application

```bash
make run
```

### Available Commands

- `add (file) (content)` - Stage a file with content
- `view (file)` - View contents of a file
- `commit (msg) (author)` - Create a commit with message and author
- `log` - Show commit history
- `branch (name)` - Create a new branch
- `checkout (name)` - Switch to a branch
- `merge (branch)` - Merge a branch into current branch
- `demo` - Run automated demo
- `exit` - Exit the program

## Documentation

This documentation provides detailed information about:

- Class interfaces and implementations
- Data structure designs
- Algorithm descriptions
- Usage examples

Browse the documentation using the navigation menu to explore the codebase.

## Project Information

- **Repository**: [https://github.com/alpdik/tri.git](https://github.com/alpdik/tri.git)
- **Language**: C++
- **Build System**: Make
