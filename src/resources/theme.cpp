#include "theme.h"

#include "default_resource.h"
#include "font.h"

extern "C" {
#include <font_kit_wrapper.h>
}

namespace revector {

void Theme::load_system_font() {
    font = DefaultResource::get_singleton()->get_default_font();

#ifdef WIN32
    std::array<const char*, 3> font_priority{
        "Microsoft YaHei",
        "SimSun",
        "SimHei",
    };
#else
    std::array<const char*, 3> font_priority{
        "Droid Sans Fallback",
        "Noto Sans CJK",
        "Noto Serif CJK",
    };
#endif

    FontBuffer font_buffer{};
    for (const auto& font_name : font_priority) {
        font_buffer = find_system_font(font_name);
        if (font_buffer.data && font_buffer.len > 0) {
            const auto font_data = std::vector<char>(font_buffer.data, font_buffer.data + font_buffer.len);
            free_font_buffer(font_buffer);

            // If the font is valid
            if (const auto new_font = Font::from_memory(font_data)) {
                font = new_font;
                break;
            }
        }
        Logger::warn("System font is not found/valid: " + std::string(font_name), "revector");
    }
}

} // namespace revector
