#include "popup_menu.h"

#include <string>

#include "../../common/utils.h"
#include "../../resources/default_resource.h"
#include "../scene_tree.h"

namespace revector {

PopupMenu::PopupMenu() {
    type = NodeType::PopupMenu;

    margin_container_ = std::make_shared<MarginContainer>();
    margin_container_->name = "PopupMenu embedded margin container";
    margin_container_->set_anchor_flag(AnchorFlag::FullRect);
    add_embedded_child(margin_container_);

    scroll_container_ = std::make_shared<ScrollContainer>();
    scroll_container_->enable_hscroll(false);
    margin_container_->add_child(scroll_container_);

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();
    theme_bg_ = std::make_optional(default_theme->popup_menu.styles["background"]);

    // Add an extra one-pixel margin to avoid rendering glitch of a scroll RenderTarget.
    auto glitch_margin_container = std::make_shared<MarginContainer>();
    glitch_margin_container->name = "PopupMenu embedded glitch fixing margin container";
    glitch_margin_container->set_margin_all(1);
    glitch_margin_container->set_mouse_filter(MouseFilter::Pass);
    scroll_container_->add_child(glitch_margin_container);

    vbox_container_ = std::make_shared<VBoxContainer>();
    vbox_container_->set_mouse_filter(MouseFilter::Pass);
    glitch_margin_container->add_child(vbox_container_);

    // auto callback = [this] { set_visibility(false); };
    // connect_signal("focus_released", callback);
}

void PopupMenu::update(double dt) {
    auto global_position = get_global_position();

    margin_container_->set_custom_minimum_size(custom_minimum_size);
    // float offset_y = 0;
    // for (auto &item : items_) {
    //     item->position = {0, offset_y};
    //     offset_y += item_height_;
    //     item->update(global_position, {size.x, item_height_});
    // }

    size = size.max(calculated_minimum_size);

    NodeUi::update(dt);
}

void PopupMenu::draw() {
    if (!visible_) {
        return;
    }

    auto vector_server = VectorServer::get_singleton();

    NodeUi::draw();

    if (theme_bg_.has_value()) {
        vector_server->draw_style_box(theme_bg_.value(), get_global_position(), size);
    }

    // auto global_position = get_global_position();
    // for (auto &item : items_) {
    //     item->draw(global_position);
    // }
}

void PopupMenu::input(InputEvent &event) {
    if (!visible_) {
        return;
    }

    // auto global_position = get_global_position();
    //
    // // If a popup menu is shown, it captures mouse events anyway.
    // bool consume_flag = true;
    //
    // if (event.type == InputEventType::MouseMotion) {
    //     auto args = event.args.mouse_motion;
    //     auto local_mouse_position = args.position - global_position;
    //
    //     for (auto &item : items_) {
    //         item->hovered = false;
    //     }
    //
    //     if (RectF(global_position, global_position + size).contains_point(args.position)) {
    //         consume_flag = true;
    //
    //         if (items_.empty()) {
    //             return;
    //         }
    //         int item_index = int(local_mouse_position.y / item_height_);
    //         item_index = std::clamp(item_index, 0, (int)items_.size() - 1);
    //         items_[item_index]->hovered = true;
    //     }
    // }
    //
    // if (event.type == InputEventType::MouseButton) {
    //     auto args = event.args.mouse_button;
    //
    //     // Hide menu.
    //     if (RectF(global_position, global_position + size).contains_point(args.position)) {
    //         consume_flag = true;
    //         set_visibility(false);
    //
    //         if (items_.empty()) {
    //             return;
    //         }
    //         int item_index = int(local_mouse_position.y / item_height_);
    //         item_index = std::clamp(item_index, 0, (int)items_.size() - 1);
    //
    //         when_item_selected(item_index);
    //     } else {
    //         consume_flag = true;
    //         set_visibility(false);
    //     }
    // }
    //
    // for (auto &item : items_) {
    //     item->input(event, global_position);
    // }
    //
    // event.consumed = true;

    NodeUi::input(event);
}

void PopupMenu::set_visibility(bool visible) {
    visible_ = visible;
    if (visible_) {
        auto render_server = RenderServer::get_singleton();
        auto window = render_server->window_builder_->get_window(get_window_index());

        // We updated its global position in MenuButton before calling set_visibility.
        auto global_position = popup_position;

        float menu_width = std::max(size.x, margin_container_->get_effective_minimum_size().x);
        float menu_top_space = global_position.y;
        float menu_bottom_space = window.lock()->get_logical_size().y - global_position.y - button_height;

        float min_menu_height = vbox_container_->get_effective_minimum_size().y + margin_container_->get_margin().top +
                                margin_container_->get_margin().bottom + 2; // 2 comes from the glitch margin container.

        bool drop_down = true;
        float actual_menu_height = std::min(min_menu_height, menu_bottom_space);

        // No enough space for dropping down.
        if (min_menu_height > menu_bottom_space) {
            if (menu_top_space > menu_bottom_space) {
                drop_down = false;
                actual_menu_height = std::min(min_menu_height, menu_top_space);
            }
        }

        if (drop_down) {
            set_position(popup_position + Vec2F{0, button_height});
            set_size({menu_width, actual_menu_height});
        } else {
            set_position(popup_position - Vec2F{0, actual_menu_height});
            set_size({menu_width, actual_menu_height});
        }
    } else {
        when_popup_hide();
    }
}

void PopupMenu::clear_items() {
    vbox_container_->remove_all_children();
    items_.clear();
    meta_.clear();
}

void PopupMenu::calc_minimum_size() {
    calculated_minimum_size = margin_container_->get_effective_minimum_size();
}

void PopupMenu::set_popup_position(Vec2F new_position, float new_button_height) {
    popup_position = new_position;
    button_height = new_button_height;
}

void PopupMenu::create_item(const std::string &text) {
    auto new_item = std::make_shared<Button>();
    new_item->set_mouse_filter(MouseFilter::Pass);
    new_item->set_text(text);

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();
    new_item->theme_normal = default_theme->popup_menu.styles["item_normal"];
    new_item->theme_hovered = default_theme->popup_menu.styles["item_hovered"];
    new_item->theme_pressed = new_item->theme_hovered;
    vbox_container_->add_child(new_item);
    vbox_container_->set_separation(0);
    vbox_container_->set_mouse_filter(MouseFilter::Pass);

    int item_index = vbox_container_->get_children().size() - 1;

    auto callback = [item_index, this] {
        set_visibility(false);
        when_item_selected(item_index);
    };
    new_item->connect_signal("triggered", callback);

    items_.push_back(new_item);
    meta_.emplace_back();
}

void PopupMenu::set_item_meta(uint32_t item_index, std::string meta) {
    meta_[item_index] = std::move(meta);
}

void PopupMenu::set_item_height(float new_item_height) {
    item_height_ = new_item_height;
    // TODO
}

float PopupMenu::get_item_height() const {
    return item_height_;
}

int PopupMenu::get_item_count() const {
    return items_.size();
}

std::string PopupMenu::get_item_text(uint32_t item_index) const {
    return items_[item_index]->get_text();
}

std::string PopupMenu::get_item_mata(uint32_t item_index) const {
    return meta_[item_index];
}

void PopupMenu::connect_signal(const std::string &signal, const AnyCallable<void> &callback) {
    NodeUi::connect_signal(signal, callback);

    if (signal == "item_selected") {
        item_selected_callbacks.push_back(callback);
    }
    if (signal == "popup_hide") {
        popup_hide_callbacks.push_back(callback);
    }
}

void PopupMenu::when_item_selected(uint32_t item_index) {
    for (auto &callback : item_selected_callbacks) {
        try {
            callback.operator()<uint32_t>(std::move(item_index));
        } catch (std::bad_any_cast &) {
            Logger::error("Mismatched signal argument types!", "revector");
        }
    }
}

void PopupMenu::when_popup_hide() {
    for (auto &callback : popup_hide_callbacks) {
        try {
            callback();
        } catch (std::bad_any_cast &) {
            Logger::error("Mismatched signal argument types!", "revector");
        }
    }
}

} // namespace revector
