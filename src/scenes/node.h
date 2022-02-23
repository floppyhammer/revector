#ifndef FLINT_NODE_H
#define FLINT_NODE_H

#include "sub_viewport.h"
#include "../core/engine.h"

#include <vector>
#include <memory>

namespace Flint {
    class Node {
    public:
        virtual void update(double delta);

        virtual void draw();

        virtual void notify(Signal signal);

        void add_child(const std::shared_ptr<Node> &p_child);

        /**
         * Get the viewport this node belongs to.
         * @return A pointer to the viewport.
         */
        virtual std::shared_ptr<SubViewport> get_viewport();

        Node *get_parent();

        std::vector<std::shared_ptr<Node>> get_children();

        void remove_child(size_t index);

    protected:
        std::vector<std::shared_ptr<Node>> children;

        // Don't use a shared pointer as it causes circular references.
        Node *parent;
    };
}

#endif //FLINT_NODE_H
