#pragma once

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <vector>

#include "common/geometry.h"
#include "nodes/scene_tree.h"
#include "render/blit.h"
#include "servers/input_server.h"
#include "servers/translation_server.h"

namespace revector {
class App {
public:
    explicit App(Vec2I primary_window_size, bool dark_mode, bool use_vulkan = true);

    ~App();

    void main_loop();

    [[nodiscard]] std::shared_ptr<Node> get_tree_root() const;

    void set_window_title(const std::string& title);

    void set_fullscreen(bool fullscreen);

private:
    std::unique_ptr<SceneTree> tree;

    bool dark_mode_ = false;
};

} // namespace revector
