#pragma once

#include <optional>

#include "../../resources/style_box.h"
#include "node_ui.h"

namespace revector {

class Panel : public NodeUi {
public:
    Panel();

    void draw() override;

    std::optional<StyleBox> theme_override_bg_;
};

} // namespace revector
