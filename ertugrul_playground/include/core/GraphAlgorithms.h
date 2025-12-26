/**
 * @file GraphAlgorithms.h
 * @author Umut Ertuğrul Daşgın
 * @brief 
 * @version 1.0
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <string>
#include "../data_structures/Stack.h"
#include "../data_structures/Queue.h"
#include "../data_structures/HashTable.h"
#include "../entities/Commit.h"

namespace core {
    class GraphAlgorithms {
    public:
        GraphAlgorithms() = delete;

        static data_structures::Stack<entities::Commit*> get_history_dfs(entities::Commit* start) {
            data_structures::Stack<entities::Commit*> history;

            if (!start) return history;

            data_structures::HashTable<std::string, bool> visited;
            data_structures::Stack<entities::Commit*> st;

            st.push(start);
            visited.put(start->get_id(), true);

            while (!st.empty()) {
                entities::Commit* curr = st.top();
                st.pop();

                history.push(curr);

                if (curr->get_parent1()) {
                    std::string p1_id = curr->get_parent1()->get_id();

                    if (!visited.contains(p1_id)) {
                        st.push(curr->get_parent1());
                        visited.put(p1_id, true);
                    }
                }

                if (curr->get_parent2()) {
                    std::string p2_id = curr->get_parent2()->get_id();

                    if (!visited.contains(p2_id)) {
                        st.push(curr->get_parent2());
                        visited.put(p2_id, true);
                    }
                }
            }

            return history;
        }

        static entities::Commit* find_merge_base(entities::Commit* c1, entities::Commit* c2) {
            if (!c1 || !c2) return nullptr;
            if (c1->get_id() == c2->get_id()) return c1;

            data_structures::HashTable<std::string, bool> ancestors1;
            data_structures::Queue<entities::Commit*> q;

            q.enqueue(c1);
            ancestors1.put(c1->get_id(), true);

            while (!q.empty()) {
                entities::Commit* curr = q.front();
                q.dequeue();

                if (curr->get_parent1()) {
                    std::string p1 = curr->get_parent1()->get_id();
                    if (!ancestors1.contains(p1)) {
                        ancestors1.put(p1, true);
                        q.enqueue(curr->get_parent1());
                    }
                }

                if (curr->get_parent2()) {
                    std::string p2 = curr->get_parent2()->get_id();
                    if (!ancestors1.contains(p2)) {
                        ancestors1.put(p2, true);
                        q.enqueue(curr->get_parent2());
                    }
                }
            }

            data_structures::Queue<entities::Commit*> q2;
            data_structures::HashTable<std::string, bool> visited2;

            q2.enqueue(c2);
            visited2.put(c2->get_id(), true);

            while (!q2.empty()) {
                entities::Commit* curr = q2.front();
                q2.dequeue();

                if (ancestors1.contains(curr->get_id())) { return curr; }

                if (curr->get_parent1()) {
                    std::string p1 = curr->get_parent1()->get_id();
                    if (!visited2.contains(p1)) {
                        visited2.put(p1, true);
                        q2.enqueue(curr->get_parent1());
                    }
                }

                if (curr->get_parent2()) {
                    std::string p2 = curr->get_parent2()->get_id();
                    if (!visited2.contains(p2)) {
                        visited2.put(p2, true);
                        q2.enqueue(curr->get_parent2());
                    }
                }
            }

            return nullptr;
        }
    };
}