#pragma once

#include "opensans_regular_ttf.h"
#include "theme.h"

namespace revector {

class Font;

class DefaultResource {
public:
    DefaultResource() = default;

    static DefaultResource *get_singleton() {
        static DefaultResource singleton;
        return &singleton;
    }

    void init(bool dark_mode) {
        default_theme = dark_mode ? Theme::dark_new() : Theme::light_new();
        default_font =
            std::make_shared<Font>(std::vector<char>(std::begin(DEFAULT_FONT_DATA), std::end(DEFAULT_FONT_DATA)));
    }

    std::shared_ptr<Theme> get_default_theme() {
        return default_theme;
    }

    std::shared_ptr<Font> get_default_font() {
        return default_font;
    }

    void set_default_font(std::shared_ptr<Font> font) {
        default_font = font;
    }

private:
    std::shared_ptr<Theme> default_theme;
    std::shared_ptr<Font> default_font;
};

} // namespace revector
