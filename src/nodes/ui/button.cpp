#include "button.h"

#include <optional>

#include "../../common/geometry.h"
#include "../../resources/default_resource.h"
#include "../../resources/vector_image.h"

namespace revector {

Button::Button() {
    type = NodeType::Button;

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();

    theme_normal = default_theme->button.styles["normal"];

    theme_hovered = default_theme->button.styles["hovered"];

    theme_pressed = default_theme->button.styles["pressed"];

    theme_disabled = default_theme->button.styles["disabled"];

    // Don't add the label as a child since it's not a normal node but part of the button.
    label = std::make_shared<Label>();
    label->set_text("Button");
    label->set_mouse_filter(MouseFilter::Ignore);
    label->set_horizontal_alignment(Alignment::Center);
    label->set_vertical_alignment(Alignment::Center);
    label->set_text_style(TextStyle{default_theme->button.colors["text"]});
    label->theme_background = StyleBox::from_empty();
    label->container_sizing.flag_h = ContainerSizingFlag::Fill;

    icon_rect = std::make_shared<TextureRect>();
    icon_rect->set_stretch_mode(TextureRect::StretchMode::KeepCentered);
    icon_rect->set_mouse_filter(MouseFilter::Ignore);

    hbox_container = std::make_shared<HBoxContainer>();
    hbox_container->add_child(icon_rect);
    hbox_container->add_child(label);
    hbox_container->set_separation(2);
    hbox_container->set_mouse_filter(MouseFilter::Ignore);

    margin_container = std::make_shared<MarginContainer>();
    margin_container->set_margin_all(4);
    margin_container->add_child(hbox_container);
    margin_container->set_size(size);
    margin_container->set_mouse_filter(MouseFilter::Ignore);

    add_embedded_child(margin_container);

    // To make a button responsive, no animation should be set when it is pressed/hovered.
    // Animations should only happen upon unpressing/unhovering.

    pressed_callbacks.emplace_back([this]() {
        target_style_box = theme_pressed;
        active_style_box = theme_pressed;
        lerp_elapsed_ = lerp_duration_;
    });

    released_callbacks.emplace_back([this]() {
        if (hovered && theme_hovered.has_value()) {
            target_style_box = theme_hovered.value();
            active_style_box = theme_hovered.value();
        } else {
            if (pressed || toggled) {
                target_style_box = theme_pressed;
                active_style_box = theme_pressed;
            } else {
                target_style_box = theme_normal;
                active_style_box = theme_normal;
            }
        }
        lerp_elapsed_ = lerp_duration_;
    });

    toggled_callbacks.emplace_back([this](bool toggled) {
        lerp_elapsed_ = lerp_duration_;
        if (toggled) {
            target_style_box = theme_pressed;
            active_style_box = theme_pressed;
        } else {
            if (hovered && theme_hovered.has_value()) {
                target_style_box = theme_hovered.value();
                active_style_box = theme_hovered.value();
            } else {
                target_style_box = theme_normal;
                active_style_box = theme_normal;
            }
        }
    });

    callbacks_cursor_entered.emplace_back([this] {
        hovered = true;
        InputServer::get_singleton()->set_cursor(get_window_index(), CursorShape::Hand);

        if (theme_hovered.has_value()) {
            target_style_box = theme_hovered.value();
            lerp_elapsed_ = 0;
        }
    });

    callbacks_cursor_exited.emplace_back([this] {
        hovered = false;
        InputServer::get_singleton()->set_cursor(get_window_index(), CursorShape::Arrow);

        if (pressed || toggled) {
            target_style_box = theme_pressed;
            lerp_elapsed_ = 0;
        } else {
            target_style_box = theme_normal;
            lerp_elapsed_ = 0;
        }
    });
}

void Button::calc_minimum_size() {
    auto container_size = margin_container->get_effective_minimum_size();

    calculated_minimum_size = container_size;
}

void Button::ready() {
    if (ready_) {
        return;
    }

    ready_ = true;

    if (toggle_mode && toggled) {
        target_style_box = theme_pressed;
        active_style_box = theme_pressed;
    } else {
        target_style_box = theme_normal;
        active_style_box = theme_normal;
    }

    lerp_elapsed_ = lerp_duration_;

    custom_ready();
}

void Button::input(InputEvent &event) {
    auto global_position = get_global_position();

    bool consume_flag = false;

    if (!disabled_) {
        if (event.type == InputEventType::MouseMotion) {
            auto args = event.args.mouse_motion;

            if (event.consumed) {
                if (!toggle_mode) {
                    pressed = false;
                }
            } else {
                if (RectF(global_position, global_position + size).contains_point(args.position)) {
                    consume_flag = true;
                } else {
                    if (!toggle_mode) {
                        pressed = false;
                    }
                }
            }
        }

        if (event.type == InputEventType::MouseButton) {
            const auto args = event.args.mouse_button;

            // Consumed by other UI nodes.
            if (event.consumed) {
                if (!args.pressed) {
                    if (RectF(global_position, global_position + size).contains_point(args.position)) {
                        if (!toggle_mode) {
                            pressed = false;
                        }
                    }
                }
            } else {
                if (RectF(global_position, global_position + size).contains_point(args.position)) {
                    // Mouse button pressed
                    if (args.pressed) {
                        pressed_position = args.position;
                        pressed = true;
                        notify_pressed();
                    }
                    // Mouse button released
                    else {
                        // Release on a pressed button
                        if (pressed) {
                            if (pressed_position.has_value() &&
                                (args.position - pressed_position.value()).length() < 8) {
                                if (toggle_mode) {
                                    if (group) {
                                        // Let group handle this event.
                                        group->notify_toggled(this);
                                    } else {
                                        toggled = !toggled;
                                        notify_toggled(toggled);
                                    }
                                } else {
                                    notify_triggered();
                                }
                            }

                            pressed = false;
                            pressed_position = {};
                            notify_released();
                        }
                    }

                    consume_flag = true;
                }
            }
        }
    }

    NodeUi::input(event);

    if (consume_flag) {
        event.consumed = true;
    }
}

void Button::update(double dt) {
    NodeUi::update(dt);

    if (animated_) {
        if (lerp_elapsed_ < lerp_duration_) {
            lerp_elapsed_ += dt;
            float t = std::clamp(lerp_elapsed_ / lerp_duration_, 0.0f, 1.0f);
            active_style_box = active_style_box.lerp_style_box(target_style_box, t);
        }
    } else {
        active_style_box = target_style_box;
    }
}

void Button::draw() {
    if (!visible_) {
        return;
    }

    auto vector_server = VectorServer::get_singleton();

    auto global_position = get_global_position();

    icon_rect->set_texture(icon_normal_);

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();

    if (disabled_) {
        label->set_text_style(TextStyle{default_theme->button.colors["text_disabled"]});
    } else {
        label->set_text_style(TextStyle{default_theme->button.colors["text"]});
    }

    if (icon_pressed_) {
        if (toggle_mode) {
            if (toggled) {
                icon_rect->set_texture(icon_pressed_);
            }
        } else if (pressed) {
            icon_rect->set_texture(icon_pressed_);
        }
    }

    // Draw style box.
    if (!flat_) {
        // Consider modulating.
        target_style_box.bg_color = ColorU(target_style_box.bg_color.to_f32() * modulate.to_f32());
        target_style_box.border_color = ColorU(target_style_box.border_color.to_f32() * modulate.to_f32());

        vector_server->draw_style_box(active_style_box, global_position, size);
    }

    NodeUi::draw();
}

void Button::set_position(Vec2F new_position) {
    position = new_position;
}

void Button::set_size(Vec2F p_size) {
    if (size == p_size) {
        return;
    }

    auto path = get_node_path();

    auto final_size = p_size.max(margin_container->get_effective_minimum_size());
    final_size = final_size.max(custom_minimum_size);

    margin_container->set_size(final_size);
    size = final_size;
}

void Button::notify_pressed() {
    for (auto &callback : pressed_callbacks) {
        callback();
    }
}

void Button::notify_released() {
    for (auto &callback : released_callbacks) {
        callback();
    }
}

void Button::notify_triggered() {
    for (auto &callback : triggered_callbacks) {
        callback();
    }
}

void Button::notify_toggled(bool toggled) {
    if (!toggle_mode) {
        return;
    }

    for (auto &callback : toggled_callbacks) {
        try {
            callback.operator()<bool>(std::move(toggled));
        } catch (std::bad_any_cast &) {
            Logger::error("Mismatched signal argument types!", "revector");
        }
    }
}

void Button::connect_signal(const std::string &signal, const AnyCallable<void> &callback) {
    NodeUi::connect_signal(signal, callback);

    if (signal == "hovered") {
        hovered_callbacks.push_back(callback);
    }
    if (signal == "pressed") {
        pressed_callbacks.push_back(callback);
    }
    if (signal == "released") {
        released_callbacks.push_back(callback);
    }
    if (signal == "triggered") {
        triggered_callbacks.push_back(callback);
    }
    if (signal == "toggled") {
        toggled_callbacks.push_back(callback);
    }
}

void Button::set_text(const std::string &text) {
    if (text.empty()) {
        label->set_text("");
        label->set_visibility(false);
        return;
    }

    label->set_text(text);
    label->set_visibility(true);
}

std::string Button::get_text() const {
    return label->get_text();
}

void Button::set_icon_normal(const std::shared_ptr<Image> &icon) {
    icon_normal_ = icon;
}

void Button::set_icon_pressed(const std::shared_ptr<Image> &icon) {
    icon_pressed_ = icon;
}

void Button::set_icon_expand(bool enable) {
    if (enable) {
        icon_rect->container_sizing.flag_h = ContainerSizingFlag::Fill;
    } else {
        icon_rect->container_sizing.flag_h = ContainerSizingFlag::NoExpand;
    }
}

void Button::set_toggle_mode(bool enable) {
    toggle_mode = enable;
}

void Button::set_toggled(bool p_toggled) {
    if (disabled_ || !toggle_mode) {
        return;
    }

    if (toggled == p_toggled) {
        return;
    }

    notify_toggled(p_toggled);
    toggled = p_toggled;
}

void Button::trigger() {
    notify_triggered();
}

void Button::set_animated(bool animated) {
    animated_ = animated;
}

void ToggleButtonGroup::notify_toggled(const Button *toggled_button) {
    // Un-toggle other buttons.
    for (auto &b : buttons) {
        b.lock()->set_toggled(b.lock().get() == toggled_button);
    }
}

void ToggleButtonGroup::add_button(const std::weak_ptr<Button> &new_button) {
    new_button.lock()->group = this;
    buttons.push_back(new_button);
}

} // namespace revector
