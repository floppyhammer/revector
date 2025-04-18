#pragma once

#include "opensans_regular_ttf.h"
#include "theme.h"

namespace revector {

class Font;

class DefaultResource {
public:
    DefaultResource() {
        default_theme = std::make_shared<Theme>();
        default_font =
            std::make_shared<Font>(std::vector<char>(std::begin(DEFAULT_FONT_DATA), std::end(DEFAULT_FONT_DATA)));
    }

    static DefaultResource *get_singleton() {
        static DefaultResource singleton;
        return &singleton;
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
