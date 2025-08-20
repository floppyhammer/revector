#include "split_container.h"

#include <string>

#include "../../../common/utils.h"
#include "../../../resources/default_resource.h"
#include "../check_button.h"

namespace revector {

SplitContainer::SplitContainer() {
    type = NodeType::SplitContainer;

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();

    container_sizing.expand_h = true;
}

void SplitContainer::adjust_layout() {
    // Get the minimum size.
    auto min_size = get_effective_minimum_size();

    // Adjust own size.
    size = size.max(min_size);

    // Calculate grabber position first.
    for (int child_idx = 0; child_idx < children.size(); child_idx++) {
        auto child = children[child_idx];

        if (child_idx > 1) {
            break;
        }

        if (child->is_ui_node()) {
            auto cast_child = dynamic_cast<NodeUi *>(child.get());
            auto child_min_size = cast_child->get_effective_minimum_size();

            if (child_idx == 0) {
                grabber_pos_ = std::max(child_min_size.x, grabber_pos_);
            } else {
                grabber_pos_ = std::min(size.x - child_min_size.x - grabber_size_, grabber_pos_);
            }
        }
    }

    // Adjust child size.
    for (int child_idx = 0; child_idx < children.size(); child_idx++) {
        auto child = children[child_idx];

        if (child_idx > 1) {
            break;
        }

        if (child->is_ui_node()) {
            auto cast_child = dynamic_cast<NodeUi *>(child.get());

            Vec2F child_size;
            Vec2F child_pos;
            if (child_idx == 0) {
                child_size = {grabber_pos_, size.y};
                child_pos = {0, 0};
            } else {
                child_size = {size.x - grabber_pos_ - grabber_size_, size.y};
                child_pos = {grabber_pos_ + grabber_size_, 0};
            }
            cast_child->set_position(child_pos);
            cast_child->set_size(child_size);
        }
    }
}

void SplitContainer::connect_signal(const std::string &signal, const AnyCallable<void> &callback) {
    Container::connect_signal(signal, callback);

    if (signal == "grabber_moved") {
        grabber_moved_callbacks.push_back(callback);
    }
}

void SplitContainer::when_grabber_moved() {
    for (auto &callback : grabber_moved_callbacks) {
        try {
            callback();
        } catch (std::bad_any_cast &) {
            Logger::error("Mismatched signal argument types!", "revector");
        }
    }
}

void SplitContainer::calc_minimum_size() {
    // Get the minimum child size.
    Vec2F min_size{};
    int valid_count = 0;

    for (const auto &child : children) {
        if (child->is_ui_node() && valid_count < 2) {
            auto cast_child = dynamic_cast<NodeUi *>(child.get());
            auto child_min_size = cast_child->get_effective_minimum_size();
            min_size += child_min_size;
        }
        valid_count++;
    }
    min_size.x += grabber_size_;

    calculated_minimum_size = min_size;
}

void SplitContainer::update(double dt) {
    NodeUi::update(dt);

    adjust_layout();
}

void SplitContainer::input(InputEvent &event) {
    auto global_position = get_global_position();

    bool consume_flag = false;

    if (event.type == InputEventType::MouseMotion) {
        auto args = event.args.mouse_motion;

        if (grabber_pressed_pos_.has_value()) {
            grabber_pos_ = (args.position.x) - global_position.x - grabber_pressed_offset.x;
        }

        if (!event.consumed) {
            if (RectF(global_position + Vec2F(grabber_pos_, 0),
                      global_position + Vec2F(grabber_pos_, 0) + Vec2F(grabber_size_, size.y))
                    .contains_point(args.position)) {
                InputServer::get_singleton()->set_cursor(get_window_index(), CursorShape::ResizeH);
            }
        }
    }

    if (event.type == InputEventType::MouseButton) {
        const auto args = event.args.mouse_button;

        if (!args.pressed) {
            if (grabber_pressed_pos_.has_value()) {
                grabber_pressed_pos_ = {};
            }
        }

        // Consumed by other UI nodes.
        if (!event.consumed) {
            if (args.pressed) {
                if (RectF(global_position + Vec2F(grabber_pos_, 0),
                          global_position + Vec2F(grabber_pos_, 0) + Vec2F(grabber_size_, size.y))
                        .contains_point(args.position)) {
                    grabber_pressed_pos_ = args.position;
                    grabber_pressed_offset = args.position - global_position - Vec2F(grabber_pos_, 0);
                }
            }
            consume_flag = true;
        }
    }

    NodeUi::input(event);

    if (consume_flag) {
        event.consumed = true;
    }
}

void SplitContainer::draw() {
    if (!visible_) {
        return;
    }

    const auto vector_server = VectorServer::get_singleton();

    const auto global_position = get_global_position();

    if (grabber_pressed_pos_.has_value()) {
        const auto start = global_position + Vec2F(grabber_pos_ + grabber_size_ * 0.5, size.y * 0.25);
        const auto end = start + Vec2F(0, size.y * 0.5);

        vector_server->draw_line(start, end, 2, ColorU::white());
    }
}

void SplitContainer::set_separation(float new_separation) {
    grabber_size_ = new_separation;
}

} // namespace revector
