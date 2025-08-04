#include <iostream>
#include <random>

#include "app.h"

using namespace revector;

using Pathfinder::Vec2;
using Pathfinder::Vec3;

class MyNode : public Node {
    void custom_ready() override {
        auto vbox_container = std::make_shared<VBoxContainer>();
        vbox_container->set_separation(8);
        vbox_container->set_position({100, 100});
        add_child(vbox_container);

        {
            auto button = std::make_shared<Button>();
            button->container_sizing.flag_h = ContainerSizingFlag::ShrinkStart;
            vbox_container->add_child(button);

            auto callback = []() { Logger::info("Button triggered"); };
            button->connect_signal("triggered", callback);
        }

        {
            auto button = std::make_shared<Button>();
            button->set_icon_normal(
                ResourceManager::get_singleton()->load<VectorImage>(revector::get_asset_dir("icons/Node_Button.svg")));
            button->container_sizing.flag_h = ContainerSizingFlag::ShrinkStart;
            vbox_container->add_child(button);
        }

        {
            auto check_button = std::make_shared<CheckButton>();
            check_button->container_sizing.flag_h = ContainerSizingFlag::ShrinkStart;
            vbox_container->add_child(check_button);

            auto callback = [](bool toggled) { Logger::info("Button toggled"); };
            check_button->connect_signal("toggled", callback);
        }
    }
};

int main() {
    App app({960, 480}, true);

    app.get_tree_root()->add_child(std::make_shared<MyNode>());

    app.main_loop();

    return EXIT_SUCCESS;
}
