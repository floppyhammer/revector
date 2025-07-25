#include "blit.h"

// SPV
#include "../shaders/generated/blit_frag_spv.h"
#include "../src/shaders/generated/blit_vert_spv.h"
// GLSL
#include "../shaders/generated/blit_frag.h"
#include "../shaders/generated/blit_vert.h"

using namespace Pathfinder;

namespace revector {

Blit::Blit(const std::shared_ptr<Device> &_device, const std::shared_ptr<Queue> &_queue, TextureFormat target_format) {
    device = _device;
    queue = _queue;

    // Set up vertex data (and buffer(s)) and configure vertex attributes.
    float vertices[] = {
        // Positions, UVs.
        -1.0, -1.0,  0.0, 0.0, // 0
        1.0,  -1.0,  1.0, 0.0, // 1
        1.0,  1.0,   1.0, 1.0, // 2
        -1.0, -1.00, 0.0, 0.0, // 3
        1.0,  1.0,   1.0, 1.0, // 4
        -1.0, 1.0,   0.0, 1.0  // 5
    };

    vertex_buffer = device->create_buffer({BufferType::Vertex, sizeof(vertices), MemoryProperty::DeviceLocal},
                                          "Blit vertex buffer");

    sampler = device->create_sampler(SamplerDescriptor{});

    auto encoder = device->create_command_encoder("Upload Blit vertex buffer");
    encoder->write_buffer(vertex_buffer, 0, sizeof(vertices), (void *)vertices);
    _queue->submit_and_wait(encoder);

    // Pipeline.
    {
        std::vector<char> vert_source, frag_source;
        if (device->get_backend_type() == BackendType::Vulkan) {
            vert_source = std::vector<char>(std::begin(blit_vert_spv), std::end(blit_vert_spv));
            frag_source = std::vector<char>(std::begin(blit_frag_spv), std::end(blit_frag_spv));
        } else {
            vert_source = std::vector<char>(std::begin(blit_vert), std::end(blit_vert));
            frag_source = std::vector<char>(std::begin(blit_frag), std::end(blit_frag));
        }

        std::vector<VertexInputAttributeDescription> attribute_descriptions;

        uint32_t stride = 4 * sizeof(float);

        attribute_descriptions.push_back({0, 2, DataType::f32, stride, 0, VertexInputRate::Vertex});

        attribute_descriptions.push_back({0, 2, DataType::f32, stride, 2 * sizeof(float), VertexInputRate::Vertex});

        auto blend_state = BlendState::from_over();

        descriptor_set = device->create_descriptor_set();
        descriptor_set->add_or_update({
            Descriptor::sampled(0, ShaderStage::Fragment, "uTexture"),
        });

        pipeline = device->create_render_pipeline(device->create_shader_module(vert_source, ShaderStage::Vertex, ""),
                                                  device->create_shader_module(frag_source, ShaderStage::Fragment, ""),
                                                  attribute_descriptions,
                                                  blend_state,
                                                  descriptor_set,
                                                  target_format,
                                                  "blit pipeline");
    }
}

void Blit::set_texture(const std::shared_ptr<Texture> &new_texture) {
    texture = new_texture;

    descriptor_set->add_or_update({
        Descriptor::sampled(0, ShaderStage::Fragment, "uTexture", texture, sampler),
    });
}

void Blit::draw(const std::shared_ptr<CommandEncoder> &encoder) {
    encoder->bind_render_pipeline(pipeline);

    encoder->bind_vertex_buffers({vertex_buffer});

    encoder->bind_descriptor_set(descriptor_set);

    encoder->draw(0, 6);
}

} // namespace revector
