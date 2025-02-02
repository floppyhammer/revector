#ifndef FLINT_APP_H
#define FLINT_APP_H

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
#include "servers/translation_server.h"

namespace Flint {
class App {
public:
    explicit App(Vec2I primary_window_size);

    ~App();

    void main_loop();

    SceneTree* get_tree() const;

    std::shared_ptr<Node> get_tree_root() const;

    void set_window_title(const std::string& title);

    void set_fullscreen(bool fullscreen);

private:
    std::unique_ptr<SceneTree> tree;

    std::shared_ptr<Pathfinder::Texture> vector_target_;
};

} // namespace Flint

#endif // FLINT_APP_H
