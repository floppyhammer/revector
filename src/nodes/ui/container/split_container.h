#pragma once

#include <optional>

#include "../../../resources/style_box.h"
#include "../button.h"
#include "../label.h"
#include "box_container.h"

namespace revector {

class SplitContainer : public Container {
public:
    SplitContainer();

    void update(double dt) override;

    void draw() override;

    void calc_minimum_size() override;

    void adjust_layout() override;

    void input(InputEvent &event) override;

    void connect_signal(const std::string &signal, const AnyCallable<void> &callback) override;

    void set_separation(float new_separation);

private:
    void when_grabber_moved();

    std::vector<AnyCallable<void>> grabber_moved_callbacks;

    Vec2F size_before_collapse_;

    ColorU theme_color_ = {};

    float grabber_pos_ = 0;
    std::optional<Vec2F> grabber_pressed_pos_;
    Vec2F grabber_pressed_offset;
    float grabber_size_ = 12;

    std::optional<StyleBox> theme_title_bar_;
    std::optional<StyleBox> theme_panel_;
};

} // namespace revector
