#include "theme.h"

#include "default_resource.h"
#include "font.h"

extern "C" {
#include <font_kit_wrapper.h>
}

namespace revector {

void Theme::load_system_font() {
#ifdef WIN32
    auto font_buffer = find_system_font("SimHei");
#else
    auto font_buffer = find_system_font("Droid Sans Fallback");
#endif

    if (font_buffer.data != nullptr && font_buffer.len > 0) {
        auto font_data = std::vector<char>(font_buffer.data, font_buffer.data + font_buffer.len);

        font = std::make_shared<Font>(font_data);

        free_font_buffer(font_buffer);
    } else {
        font = DefaultResource::get_singleton()->get_default_font();
    }
}

} // namespace revector
