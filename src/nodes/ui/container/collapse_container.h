#pragma once

#include <optional>

#include "../../../resources/style_box.h"
#include "../button.h"
#include "../label.h"
#include "box_container.h"

namespace revector {

enum class CollapseButtonType {
    Default,
    Check,
};

class CollapseContainer : public Container {
public:
    CollapseContainer(CollapseButtonType button_type);

    void update(double dt) override;

    void draw() override;

    void calc_minimum_size() override;

    void set_title(std::string title);

    void adjust_layout() override;

    void set_color(ColorU color);

    void set_collapse(bool collapse);

    void connect_signal(const std::string &signal, const AnyCallable<void> &callback) override;

private:
    void when_collapsed(bool collapsed);

    bool collapsed_ = false;

    std::shared_ptr<Button> collapse_button_;

    std::vector<AnyCallable<void>> collapsed_callbacks;

    float title_bar_height_ = 32;
    float margin_ = 8;

    Vec2F size_before_collapse_;

    ColorU theme_color_ = {};

    std::optional<StyleBox> theme_title_bar_;
    std::optional<StyleBox> theme_panel_;
};

} // namespace revector
