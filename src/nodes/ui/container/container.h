#pragma once

#include "../node_ui.h"

namespace revector {

/**
 * Containers adjust their container children's layouts automatically.
 */
class Container : public NodeUi {
public:
    Container();

    void update(double dt) override;

    /// Calculates the minimum size of this node, considering all its children's sizing effect.
    void calc_minimum_size() override;

protected:
    /// The most important method for containers. Adjusts its own size (but not position),
    /// adjusts its children's sizes and local positions.
    virtual void adjust_layout();

    std::vector<NodeUi *> get_visible_ui_children() const;
};

} // namespace revector
