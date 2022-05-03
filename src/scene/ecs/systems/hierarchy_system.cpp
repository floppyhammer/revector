#include "hierarchy_system.h"

#include "../components/components.h"

#include <queue>

namespace Flint {
    void traverse_children(const std::weak_ptr<Coordinator>& p_coordinator, Entity node) {
        auto coordinator = p_coordinator.lock();
        auto &relations = coordinator->get_component<HierarchicalRelations>(node);

        auto current_child = relations.first_child;

        while (true) {
            if (current_child.has_value()) {
                Entity child_entity = current_child.value();
                auto &child_relations = coordinator->get_component<HierarchicalRelations>(child_entity);
                std::cout << child_entity << "  ";
                traverse_children(p_coordinator, child_entity);
                current_child = child_relations.next_sibling;
            } else {
                std::cout << "| ";
                break;
            }
        }
    }

    void HierarchySystem::traverse(const std::weak_ptr<Coordinator>& p_coordinator, Entity root) {
        traverse_children(p_coordinator, root);
    }
}
