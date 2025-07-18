#include "default_resource.h"

#include "font.h"
#include "opensans_regular_ttf.h"

namespace revector {

void DefaultResource::init(bool dark_mode) {
    default_theme = dark_mode ? Theme::dark_new() : Theme::light_new();
    default_font =
        std::make_shared<Font>(std::vector<char>(std::begin(DEFAULT_FONT_DATA), std::end(DEFAULT_FONT_DATA)));
}

} // namespace revector
