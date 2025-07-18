#pragma once

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

    void init(bool dark_mode);

    std::shared_ptr<Theme> get_default_theme() {
        return default_theme;
    }

    std::shared_ptr<Font> get_default_font() {
        return default_font;
    }

private:
    std::shared_ptr<Theme> default_theme;
    std::shared_ptr<Font> default_font;
};

} // namespace revector
