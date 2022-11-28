#include "node_3d.h"

// The perspective projection matrix generated by GLM will use the OpenGL depth range of -1.0 to 1.0 by default.
// We need to configure it to use the Vulkan range of 0.0 to 1.0 using the GLM_FORCE_DEPTH_ZERO_TO_ONE definition.
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <chrono>

#include "../../../common/io.h"
#include "../../../render/swap_chain.h"
#include "../sub_viewport.h"
#include "camera_3d.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Flint {
Node3D::Node3D() {
    type = NodeType::Node3D;
}

Node3D::~Node3D() {
}

void Node3D::update(double delta) {
    update_mvp();
}

void Node3D::update_mvp() {
    // Prepare MVP data.
    ModelViewProjection mvp{};

    // Determined by model transform.
    mvp.model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));
    mvp.model = glm::scale(mvp.model, glm::vec3(scale.x, scale.y, scale.z));
    mvp.model = glm::rotate(mvp.model,
                            (float)CoreServer::get_singleton()->get_elapsed() * 0.2f * glm::radians(90.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));

    // FIXME: Should get camera from the viewport.
    // Determined by camera.
    Camera3D camera;
    camera.position = glm::vec3(2.0f, 2.0f, 2.0f);

    mvp.view = glm::lookAt(camera.position, glm::vec3(0.0f, 0.0f, 0.0f), camera.get_up_direction());

    Node *viewport_node = get_viewport();

    if (viewport_node) {
        auto viewport = dynamic_cast<SubViewport *>(viewport_node);

        // Set projection matrix. Determined by viewport.
        mvp.proj = glm::perspective(glm::radians(viewport->fov),
                                    (float)viewport->get_extent().x / (float)viewport->get_extent().y,
                                    viewport->z_near,
                                    viewport->z_far);

        // GLM was originally designed for OpenGL,
        // where the Y coordinate of the clip coordinates is inverted.
        mvp.proj[1][1] *= -1;
    } else {
        auto viewport_extent = SwapChain::getSingleton()->swapChainExtent;

        // Set projection matrix. Determined by viewport.
        mvp.proj = glm::perspective(
            glm::radians(45.0f), (float)viewport_extent.width / (float)viewport_extent.height, 0.1f, 10.0f);

        // GLM was originally designed for OpenGL,
        // where the Y coordinate of the clip coordinates is inverted.
        mvp.proj[1][1] *= -1;
    }

    push_constant.mvp = mvp.calculate_mvp();
}
} // namespace Flint
