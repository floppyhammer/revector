#pragma once

#include <functional>
#include <optional>

#include "../../resources/style_box.h"
#include "label.h"
#include "node_ui.h"

namespace revector {

class ProgressBar : public NodeUi {
public:
    ProgressBar();

    void update(double dt) override;

    void draw() override;

    void set_position(Vec2F new_position) override;

    void set_size(Vec2F new_size) override;

    void calc_minimum_size() override;

    void set_value(float new_value);

    float get_value() const;

    void set_min_value(float new_value);

    float get_min_value() const;

    void set_max_value(float new_value);

    float get_max_value() const;

    void set_step(float new_step);

    void value_changed();

    void connect_signal(const std::string &signal, const AnyCallable<void> &callback) override;

    void set_label_visibility(bool new_visibility);

    void set_label_font_size(float new_font_size);

    void set_lerp_enabled(bool new_lerp_enabled);

protected:
    float value = 50;
    float target_value = 50;
    float min_value = 0;
    float max_value = 100;
    float step = 1;
    float ratio;

    bool label_visible = true;

    bool lerp_enabled = false;

    std::optional<StyleBox> theme_progress, theme_bg, theme_fg;

    std::shared_ptr<Label> label;

    std::vector<AnyCallable<void>> on_value_changed;
};

} // namespace revector
