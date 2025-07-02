#pragma once

#include <pathfinder/prelude.h>

#include "../common/geometry.h"
#include "../servers/render_server.h"
#include "image.h"

namespace revector {

class RenderImage : public Image {
public:
    explicit RenderImage(Vec2I _size) : Image(_size) {
        Pathfinder::TextureDescriptor desc = {
            size,
            Pathfinder::TextureFormat::Rgba8Unorm,
        };

        type = ImageType::Render;

        texture_ = RenderServer::get_singleton()->device_->create_texture(desc, "render image");
    }

    explicit RenderImage(const std::shared_ptr<Pathfinder::Texture>& existing_texture)
        : Image(existing_texture->get_size()) {
        type = ImageType::Render;

        texture_ = existing_texture;
    }

    std::shared_ptr<Pathfinder::Texture> get_texture() const {
        return texture_;
    }

protected:
    std::shared_ptr<Pathfinder::Texture> texture_;
};

} // namespace revector
