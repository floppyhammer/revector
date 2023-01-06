#include <iostream>
#include <random>
#include <stdexcept>

#include "app.h"

using namespace Flint;

using Pathfinder::Vec2;
using Pathfinder::Vec3;

const uint32_t WINDOW_WIDTH = 640;
const uint32_t WINDOW_HEIGHT = 480;

int main() {
    App app(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Build scene tree. Use a block, so we don't increase ref counts for the node.
    {
        auto panel = std::make_shared<Panel>();
        panel->enable_title_bar(false);
        panel->set_size({WINDOW_WIDTH, WINDOW_HEIGHT});
        panel->set_anchor_flag(AnchorFlag::FullRect);
        panel->apply_fullscreen_style();
        app.tree->replace_scene(panel);

        auto margin_container = std::make_shared<MarginContainer>();
        margin_container->set_position({0, 0});
        margin_container->set_size({WINDOW_WIDTH, WINDOW_HEIGHT});
        margin_container->set_margin_all(32);
        margin_container->set_anchor_flag(AnchorFlag::FullRect);
        panel->add_child(margin_container);

        auto vstack_container = std::make_shared<VStackContainer>();
        vstack_container->set_separation(16);
        margin_container->add_child(vstack_container);

        vstack_container->add_child(std::make_shared<Label>("你好"));

        vstack_container->add_child(std::make_shared<Label>("Hello"));

        auto label = std::make_shared<Label>("مرحبا");
        label->set_language(Language::Arabic);
        vstack_container->add_child(label);
    }

    app.main_loop();

    app.cleanup();

    return EXIT_SUCCESS;
}
