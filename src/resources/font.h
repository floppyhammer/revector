#ifndef FLINT_FONT_H
#define FLINT_FONT_H

#include <hb.h>
#include <pathfinder.h>
#include <stb/stb_truetype.h>

#include <cstdio>
#include <cstdlib>

#include "common/logger.h"
#include "resource.h"

using Pathfinder::ColorU;
using Pathfinder::RectF;
using Pathfinder::RectI;
using Pathfinder::Vec2F;

namespace Flint {

struct HarfBuzzRes {
    hb_blob_t *blob{}; /* or hb_blob_create_from_file_or_fail() */
    hb_face_t *face{};
    hb_font_t *font{};

    HarfBuzzRes() = default;

    explicit HarfBuzzRes(const std::vector<char> &bytes) {
        blob = hb_blob_create(bytes.data(),
                              bytes.size(),
                              HB_MEMORY_MODE_READONLY,
                              nullptr,
                              nullptr); /* or hb_blob_create_from_file_or_fail() */
        face = hb_face_create(blob, 0);
        font = hb_font_create(face);
    }

    ~HarfBuzzRes() {
        if (font) {
            hb_font_destroy(font);
        }
        if (face) {
            hb_face_destroy(face);
        }
        if (blob) {
            hb_blob_destroy(blob);
        }
    }
};

struct TextStyle {
    ColorU color = ColorU::white();
    ColorU stroke_color;
    float stroke_width = 0;
    bool debug = false;
};

struct Glyph {
    // Glyph index (font specific) or UTF-32 codepoint (for the invalid glyphs).
    // A particular glyph ID within the font does not necessarily correlate to a predictable Unicode codepoint.
    uint16_t index = 0;

    bool emoji = false;

    int32_t x_offset = 0; // Offset from the origin of the glyph on baseline.
    int32_t y_offset = 0;

    float x_advance = 0; // Advance to the next glyph along baseline (x for horizontal layout, y for vertical).
    float y_advance = 0;

    // Glyph path. The points are in the glyph's baseline coordinates.
    Pathfinder::Path2d path;

    // Only emojis have SVG data.
    std::vector<char> svg;

    /// Glyph box in the baseline coordinates.
    RectF box;

    /// Glyph path's bounding box in the baseline coordinates.
    RectF bbox;
};

class Font : public Resource {
public:
    explicit Font(const std::string &path);

    ~Font() override;

    void set_size(uint32_t new_font_size);

    uint32_t get_size() const;

    Pathfinder::Path2d get_glyph_path(uint16_t glyph_index) const;

    std::vector<char> get_glyph_svg(uint16_t glyph_index) const;

    /// Paragraphs and lines are different concepts.
    /// Paragraphs are seperated by line breaks, while lines are results of automatic layout.
    /// A paragraph may contain one or more lines.
    void get_glyphs(const std::string &text, std::vector<Glyph> &glyphs, std::vector<Pathfinder::Range> &para_ranges);

    int32_t find_index(int codepoint);

    float get_advance(uint16_t glyph_index) const;

    RectI get_bounds(uint16_t glyph_index) const;

    int get_ascent() const;

    int get_descent() const;

private:
    /// Stores font data, should not be freed until font is deleted.
    unsigned char *stbtt_buffer{};

    stbtt_fontinfo stbtt_info{};

    uint32_t size = 32;

    float scale;

    int32_t ascent;
    int32_t descent;

    std::shared_ptr<HarfBuzzRes> harfbuzz_res;

    std::unordered_map<uint16_t, Glyph> glyph_cache;

    std::vector<char> font_data;

private:
    void get_metrics();
};

} // namespace Flint

#endif // FLINT_FONT_H
