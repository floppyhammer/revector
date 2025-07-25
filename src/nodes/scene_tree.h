#pragma once

#include "file_dialog.h"
#include "node.h"
#include "timer.h"
#include "ui/button.h"
#include "ui/check_button.h"
#include "ui/container/box_container.h"
#include "ui/container/collapse_container.h"
#include "ui/container/grid_container.h"
#include "ui/container/margin_container.h"
#include "ui/container/scroll_container.h"
#include "ui/container/tab_container.h"
#include "ui/label.h"
#include "ui/menu_button.h"
#include "ui/panel.h"
#include "ui/popup_menu.h"
#include "ui/progress_bar.h"
#include "ui/spin_box.h"
#include "ui/text_edit.h"
#include "ui/texture_rect.h"
#include "ui/tree.h"

namespace Pathfinder {
class Window;
}

namespace revector {

class SubWindow;

void transform_system(Node* root);

void propagate_draw(Node* node);

/// Run calc_minimum_size() depth-first.
void calc_minimum_size(Node* root);

/// Processing order: Input -> Update -> Draw.
class SceneTree {
    friend class App;

public:
    explicit SceneTree(Vec2I primary_window_size);

    void process(double dt);

    bool render();

    std::shared_ptr<Node> get_root() const;

    void notify_primary_window_size_changed(Vec2I new_size) const;

    void quit();

    bool has_quited() const;

    std::weak_ptr<Pathfinder::Window> get_primary_window() const;

private:
    /// Primary window
    std::shared_ptr<SubWindow> root;

    bool quited = false;
};

} // namespace revector
