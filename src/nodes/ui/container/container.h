#pragma once

#include "../node_ui.h"

namespace revector {

/// A Container adjusts layouts for its UI children automatically.
class Container : public NodeUi {
public:
    Container();

    void update(double dt) override;

    void calc_minimum_size() override;

protected:
    /// The most important method for containers.
    /// This adjusts its own size (not position), adjusts its children's sizes and local positions.
    virtual void adjust_layout();

    std::vector<NodeUi *> get_visible_ui_children() const;
};

} // namespace revector
