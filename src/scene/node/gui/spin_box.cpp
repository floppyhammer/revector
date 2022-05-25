#include "spin_box.h"

#include "../../../resources/vector_texture.h"
#include "../../../common/math/rect.h"

namespace Flint {
    SpinBox::SpinBox() {
        type = NodeType::SpinBox;

        theme_hovered = std::optional(StyleBox());
        theme_hovered.value().border_color = ColorU(200, 200, 200, 255);
        theme_hovered.value().border_width = 2;

        theme_pressed = std::optional(StyleBox());
        theme_pressed.value().bg_color = ColorU(70, 70, 70, 255);
        theme_pressed.value().border_color = ColorU(200, 200, 200, 255);
        theme_pressed.value().border_width = 2;

        debug_size_box.border_color = ColorU::green();

        // Don't add the label as a child since it's not a normal node but part of the SpinBox.
        label = std::make_shared<Label>();
        label->set_text("SpinBox");
        label->set_mouse_filter(MouseFilter::IGNORE);
        label->set_horizontal_alignment(Alignment::Center);
        label->set_vertical_alignment(Alignment::Center);

        auto vector_texture = VectorTexture::from_empty(24, 24);
        VShape vshape;
        vshape.shape.add_circle({}, 8);
        vshape.shape.translate({vector_texture->get_width() * 0.5f, vector_texture->get_height() * 0.5f});
        vshape.stroke_color = ColorU(163, 163, 163, 255);
        vshape.stroke_width = 2;
        vector_texture->set_vshapes({vshape});
        icon_rect = std::make_shared<TextureRect>();
        icon_rect->sizing_flag = ContainerSizingFlag::EXPAND;
        icon_rect->set_texture(vector_texture);

        container = std::make_shared<BoxContainer>();
        container->set_parent(this);
        container->add_child(icon_rect);
        container->add_child(label);
        container->set_separation(0);
        container->set_size(size);
    }

    Vec2<float> SpinBox::calculate_minimum_size() const {
        auto container_size = container->calculate_minimum_size();

        return container_size.max(minimum_size);
    }

    void SpinBox::input(std::vector<InputEvent> &input_queue) {
        auto global_position = get_global_position();

        for (auto &event: input_queue) {
            bool consume_flag = false;

            if (event.type == InputEventType::MouseMotion) {
                auto args = event.args.mouse_motion;

                if (event.is_consumed()) {
                    hovered = false;
                    pressed = false;
                    pressed_inside = false;
                } else {
                    if (Rect<float>(global_position, global_position + size).contains_point(args.position)) {
                        hovered = true;
                        consume_flag = true;
                    } else {
                        hovered = false;
                        pressed = false;
                        pressed_inside = false;
                    }
                }
            }

            if (event.type == InputEventType::MouseSpinBox) {
                auto args = event.args.mouse_SpinBox;

                if (event.is_consumed()) {
                    if (!args.pressed) {
                        if (Rect<float>(global_position, global_position + size).contains_point(args.position)) {
                            pressed = false;
                            pressed_inside = false;
                        }
                    }
                } else {
                    if (Rect<float>(global_position, global_position + size).contains_point(args.position)) {
                        pressed = args.pressed;
                        if (pressed) {
                            pressed_inside = true;
                        } else {
                            if (pressed_inside) on_pressed();
                        }
                        consume_flag = true;
                    }
                }
            }

            if (consume_flag) {
                event.consume();
            }
        }

        Control::input(input_queue);
    }

    void SpinBox::update(double dt) {
        Control::update(dt);

//        if (icon.has_value()) {
//            auto icon_size = icon.value().size;
//            label->set_size({size.x - icon_size.x, size.y});
//            label->set_position({icon_size.x, 0});
//        } else {
//            label->set_size({size.x, size.y});
//            label->set_position({0, 0});
//        }
        container->propagate_update(dt);

        label->update(dt);
    }

    void SpinBox::draw(VkCommandBuffer p_command_buffer) {
        if (!visible) return;

        auto canvas = VectorServer::get_singleton()->canvas;

        auto global_position = get_global_position();

        // Draw bg.
        std::optional<StyleBox> active_style_box;
        if (hovered) {
            active_style_box = pressed ? theme_pressed : theme_hovered;
        } else {
            active_style_box = theme_normal;
        }

        if (active_style_box.has_value()) {
            active_style_box.value().add_to_canvas(global_position, size, canvas);
        }

        container->propagate_draw(p_command_buffer);

        Control::draw(p_command_buffer);
    }

    void SpinBox::set_position(Vec2<float> p_position) {
        position = p_position;
    }

    void SpinBox::set_size(Vec2<float> p_size) {
        if (size == p_size) return;

        auto path = get_node_path();

        auto final_size = p_size.max(container->calculate_minimum_size());
        final_size = final_size.max(minimum_size);

        container->set_size(final_size);
        size = final_size;
    }

    void SpinBox::on_pressed() {
        for (auto &callback: on_pressed_callbacks) {
            callback();
        }
    }

    void SpinBox::connect_signal(std::string signal, std::function<void()> callback) {
        if (signal == "on_pressed") {
            on_pressed_callbacks.push_back(callback);
        }
    }

    void SpinBox::set_text(const std::string &text) {
        label->set_text(text);
        label->set_visibility(text.empty());
    }

    void SpinBox::set_icon(const std::shared_ptr<Texture> &p_icon) {
        icon_rect->set_texture(p_icon);
    }
}
