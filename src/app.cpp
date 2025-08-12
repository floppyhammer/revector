#include "app.h"

#include <cstdint>
#include <memory>

#include "resources/default_resource.h"
#include "resources/resource_manager.h"
#include "servers/engine.h"
#include "servers/input_server.h"
#include "servers/render_server.h"
#include "servers/vector_server.h"

namespace revector {

App::App(Vec2I primary_window_size, const bool dark_mode) {
    // Set logger level.
    Logger::set_global_level(Logger::Level::Info);
    Logger::set_module_level("revector", Logger::Level::Info);

    dark_mode_ = dark_mode;

    DefaultResource::get_singleton()->init(dark_mode_);

    auto render_server = RenderServer::get_singleton();

    auto window_builder = Pathfinder::WindowBuilder::new_impl(Pathfinder::BackendType::Vulkan, primary_window_size);
    render_server->window_builder_ = window_builder;

    // Create the main window.
    auto primary_window = render_server->window_builder_->get_window(0);

    // Create device and queue.
    render_server->device_ = window_builder->request_device();
    render_server->queue_ = window_builder->create_queue();

    auto vector_server = VectorServer::get_singleton();
    vector_server->init(primary_window.lock()->get_physical_size(),
                        render_server->device_,
                        render_server->queue_,
                        Pathfinder::RenderLevel::D3d9);

    tree = std::make_unique<SceneTree>(primary_window_size);
}

App::~App() {
    // Clean up the scene tree.
    tree.reset();

    VectorServer::get_singleton()->cleanup();
    Logger::verbose("Cleaned up VectorServer.", "revector");

    RenderServer::get_singleton()->destroy();
    Logger::verbose("Cleaned up RenderServer.", "revector");
}

std::shared_ptr<Node> App::get_tree_root() const {
    return tree->get_root();
}

void App::set_window_title(const std::string& title) {
    auto render_server = RenderServer::get_singleton();
    auto primary_window = render_server->window_builder_->get_window(0);
    primary_window.lock()->set_window_title(title);
}

void App::set_fullscreen(bool fullscreen) {
    auto render_server = RenderServer::get_singleton();

    render_server->window_builder_->set_fullscreen(fullscreen);
}

void App::main_loop() {
    bool closing_app = false;

    while (!closing_app) {
        InputServer::get_singleton()->clear_events();
        RenderServer::get_singleton()->window_builder_->poll_events();

        // Engine processing.
        Engine::get_singleton()->tick();

        // Get frame time.
        auto dt = Engine::get_singleton()->get_dt();

        // Update the scene tree.
        tree->process(dt);

        closing_app = tree->render();
    }

    RenderServer::get_singleton()->window_builder_->stop_and_destroy_swapchains();
}

} // namespace revector
