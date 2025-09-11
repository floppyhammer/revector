#include <iostream>
#include <random>

#include "app.h"

using namespace revector;

class MyNode : public NodeUi {
    void custom_ready() override {
        anchor_mode = AnchorFlag::FullRect;

        auto slider = std::make_shared<Slider>();
        slider->set_position({200, 200});
        slider->set_size({400, 40});
        add_child(slider);

        auto callback = [](float value) { std::cout << value << std::endl; };
        slider->connect_signal("value_changed", callback);
    }
};

int main() {
    App app({960, 480}, true);

    app.get_tree_root()->add_child(std::make_shared<MyNode>());

    app.main_loop();

    return EXIT_SUCCESS;
}
