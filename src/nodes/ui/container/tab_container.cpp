#include "tab_container.h"

#include "../../../resources/default_resource.h"
#include "scroll_container.h"

namespace revector {

TabContainer::TabContainer() {
    type = NodeType::TabContainer;

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();

    theme_bg = std::make_optional(default_theme->tab_container.styles["background"]);

    theme_button_panel = default_theme->tab_container.styles["tab_background"];

    button_scroll_container = std::make_shared<ScrollContainer>();
    button_scroll_container->enable_vscroll(false);
    add_embedded_child(button_scroll_container);

    button_container = std::make_shared<HBoxContainer>();
    button_scroll_container->add_child(button_container);

    tab_button_group = std::make_shared<ToggleButtonGroup>();
}

void TabContainer::adjust_layout() {
    // Adjust self size.
    size = get_effective_minimum_size().max(size);

    auto tab_button_height = button_container->get_effective_minimum_size().y;

    for (int i = 0; i < children.size(); i++) {
        if (!children[i]->is_ui_node()) {
            continue;
        }

        children[i]->set_visibility(i == current_tab);

        auto ui_child = dynamic_cast<NodeUi *>(children[i].get());
        ui_child->set_position({0, tab_button_height});
        ui_child->set_size({size.x, size.y - tab_button_height});
    }

    button_scroll_container->set_custom_minimum_size({size.x, tab_button_height});
}

void TabContainer::update(double dt) {
    Container::update(dt);
}

void TabContainer::calc_minimum_size() {
    Vec2F min_size;

    // Find the largest child size.
    for (auto &child : children) {
        // We should take account of invisible UI nodes.
        if (!child->is_ui_node()) {
            continue;
        }

        auto cast_child = dynamic_cast<NodeUi *>(child.get());
        auto child_min_size = cast_child->get_effective_minimum_size();

        min_size = min_size.max(child_min_size);
    }

    auto tab_button_height = button_container->get_effective_minimum_size().y;
    min_size.y += tab_button_height;

    calculated_minimum_size = min_size;
}

std::optional<uint32_t> TabContainer::get_current_tab() const {
    return current_tab;
}

void TabContainer::set_current_tab(uint32_t index) {
    current_tab = index;
    tab_buttons[index]->set_toggled(true);
}

void TabContainer::draw() {
    Container::draw();

    auto vs = VectorServer::get_singleton();

    auto global_pos = get_global_position();

    auto tab_button_height = button_container->get_effective_minimum_size().y;

    vs->draw_style_box(theme_button_panel.value(), global_pos, {size.x, tab_button_height});
}

void TabContainer::add_child(const std::shared_ptr<Node> &new_child) {
    Node::add_child(new_child);

    add_tab_button();

    if (children.size() == 1) {
        set_current_tab(0);
    }
}

void TabContainer::add_tab_button() {
    auto button = std::make_shared<Button>();
    button->set_pressed_style_to_toggled(true);
    button_container->add_child(button);

    uint32_t button_idx = tab_buttons.size();
    button->set_text("Tab " + std::to_string(button_idx));
    tab_buttons.push_back(button);

    tab_button_group->add_button(button);

    auto callback = [this, button_idx](const bool toggled) {
        if (toggled) {
            current_tab = button_idx;
        }
    };
    button->connect_signal("toggled", callback);
    button->set_toggle_mode(true);

    auto default_theme = DefaultResource::get_singleton()->get_default_theme();

    button->theme_normal = default_theme->tab_container.styles["tab_button_normal"];
    button->theme_hovered = button->theme_normal;
    button->theme_pressed = default_theme->tab_container.styles["tab_button_pressed"];
}

void TabContainer::set_tab_title(uint32_t tab_index, const std::string &title) {
    if (tab_index >= tab_buttons.size()) {
        return;
    }
    tab_buttons[tab_index]->set_text(title);
}

void TabContainer::set_tab_disabled(bool disabled) {
    for (auto &btn : tab_buttons) {
        btn->set_disabled(disabled);
    }
}

} // namespace revector
