#pragma once

#include <optional>

#include "../common/geometry.h"

namespace revector {

struct StyleBox {
    ColorU bg_color = ColorU(27, 27, 27, 255);

    ColorU border_color = ColorU(67, 67, 67, 255);
    float border_width = 0;
    float corner_radius = 8;

    std::optional<RectF> border_widths;
    // Top-left, top-eight, bottom-left, bottom-right.
    std::optional<RectF> corner_radii;

    ColorU shadow_color;
    float shadow_size = 0;
    Vec2F shadow_offset;

    static StyleBox from_empty() {
        StyleBox box;
        box.bg_color = ColorU::transparent_black();

        box.border_color = ColorU::transparent_black();
        box.border_width = 0;
        box.corner_radius = {};

        box.shadow_color = ColorU::transparent_black();
        box.shadow_size = 0;
        box.shadow_offset = Vec2F();

        return box;
    }

    StyleBox lerp_style_box(const StyleBox target_box, const float t) const {
        StyleBox box;
        box.border_width = Pathfinder::lerp(border_width, target_box.border_width, t);
        box.corner_radius = Pathfinder::lerp(corner_radius, target_box.corner_radius, t);

        box.bg_color = bg_color.lerp(target_box.bg_color, t);
        box.border_color = border_color.lerp(target_box.border_color, t);

        // todo: lerp corner radii
        if (corner_radii.has_value()) {
            if (target_box.corner_radii.has_value()) {
                box.corner_radii = target_box.corner_radii;
            }
        } else if (target_box.corner_radii.has_value()) {
            box.corner_radii = target_box.corner_radii;
        }

        if (border_widths.has_value()) {
            if (target_box.border_widths.has_value()) {
                box.border_widths = target_box.border_widths;
            }
        } else if (target_box.border_widths.has_value()) {
            box.border_widths = target_box.border_widths;
        }

        return box;
    }
};

struct StyleLine {
    ColorU color = ColorU(163, 163, 163, 255);

    float width = 2;
};

} // namespace revector
