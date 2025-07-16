#pragma once

#include <map>

#include "style_box.h"

namespace revector {

class Font;

class Theme {
public:
    Theme() = default;

    static std::shared_ptr<Theme> dark_new() {
        auto theme = std::make_shared<Theme>();

        theme->button.colors["text"] = ColorU(200, 200, 200);

        theme->button.colors["text_disabled"] = ColorU(200, 200, 200, 100);

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(32, 32, 32);
            style_box.border_color = ColorU(67, 67, 67);
            style_box.border_width = 2;
            theme->button.styles["normal"] = style_box;
        }

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(41, 41, 41);
            style_box.border_color = ColorU(67, 67, 67);
            style_box.border_width = 2;
            theme->button.styles["hovered"] = style_box;
        }

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(45, 45, 45, 255);
            style_box.border_color = ColorU(67, 67, 67, 255);
            style_box.border_width = 2;
            theme->button.styles["pressed"] = style_box;
        }

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(32, 32, 32, 255);
            style_box.border_color = ColorU(67, 67, 67, 255);
            style_box.border_width = 2;
            theme->button.styles["disabled"] = style_box;
        }

        theme->label.colors["text"] = ColorU(200, 200, 200, 255);
        theme->label.styles["background"] = StyleBox::from_empty();

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(32, 32, 32, 255);
            style_box.border_color = {75, 75, 75, 100};
            style_box.border_width = 2;
            style_box.corner_radius = 8;
            theme->panel.styles["background"] = style_box;
        }

        {
            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(86, 170, 114, 255);
                style_box.border_width = 0;
                style_box.corner_radius = 8;
                theme->collapse_container.styles["title_bar"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(32, 32, 32, 255);
                style_box.border_color = {86, 170, 114, 255};
                style_box.border_width = 2;
                style_box.corner_radius = 8;
                theme->collapse_container.styles["background"] = style_box;
            }
        }

        // Tab container
        {
            {
                auto style_box = StyleBox();
                style_box.corner_radius = 0;
                style_box.border_width = 0;
                style_box.bg_color = ColorU(32, 32, 32);
                theme->tab_container.styles["background"] = style_box;
            }
            {
                auto style_box = StyleBox();
                style_box.corner_radius = 0;
                style_box.border_width = 0;
                style_box.bg_color = ColorU(40, 40, 40);
                theme->tab_container.styles["tab_background"] = style_box;
            }
            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(40, 40, 40);
                theme->tab_container.styles["tab_button_normal"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(32, 32, 32);
                theme->tab_container.styles["tab_button_hovered"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(32, 32, 32);
                theme->tab_container.styles["tab_button_pressed"] = style_box;
            }
        }

        // Popup menu
        {
            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(32, 32, 32, 255);
                style_box.border_color = {75, 75, 75, 100};
                style_box.border_width = 2;
                style_box.corner_radius = 8;
                theme->popup_menu.styles["background"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(255, 255, 255, 0);
                style_box.border_width = 0;
                theme->popup_menu.styles["item_normal"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(100, 100, 100);
                style_box.border_width = 0;
                theme->popup_menu.styles["item_hovered"] = style_box;
            }
        }

        // Text edit
        {
            {
                auto style_box = StyleBox();
                style_box.corner_radius = 8;
                style_box.border_width = 0;
                style_box.bg_color = ColorU(10, 10, 10);
                theme->text_edit.styles["normal"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.corner_radius = 8;
                style_box.border_width = 2;
                style_box.bg_color = ColorU(10, 10, 10);
                theme->text_edit.styles["focused"] = style_box;
            }

            theme->text_edit.caret.color = ColorU(200, 200, 200);
            theme->text_edit.caret.width = 2;

            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(35, 92, 207);
                theme->text_edit.styles["selection"] = style_box;
            }
        }

        theme->load_system_font();

        return theme;
    }

    static std::shared_ptr<Theme> light_new() {
        auto theme = std::make_shared<Theme>();

        theme->button.colors["text"] = ColorU(55, 55, 55);

        theme->button.colors["text_disabled"] = ColorU(55, 55, 55, 100);

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(223, 223, 223);
            style_box.border_color = ColorU(188, 188, 188);
            style_box.border_width = 2;
            theme->button.styles["normal"] = style_box;
        }

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(200, 200, 200);
            style_box.border_color = ColorU(188, 188, 188);
            style_box.border_width = 2;
            theme->button.styles["hovered"] = style_box;
        }

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(210, 210, 210, 255);
            style_box.border_color = ColorU(188, 188, 188, 255);
            style_box.border_width = 2;
            theme->button.styles["pressed"] = style_box;
        }

        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(223, 223, 223, 255);
            style_box.border_color = ColorU(188, 188, 188, 255);
            style_box.border_width = 2;
            theme->button.styles["disabled"] = style_box;
        }

        theme->label.colors["text"] = ColorU(55, 55, 55);
        theme->label.styles["background"] = StyleBox::from_empty();

        // Panel
        {
            auto style_box = StyleBox();
            style_box.bg_color = ColorU(223, 223, 223);
            style_box.border_color = {180, 180, 180, 100};
            style_box.border_width = 2;
            style_box.corner_radius = 8;
            theme->panel.styles["background"] = style_box;
        }

        {
            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(86, 170, 114, 255);
                style_box.border_width = 0;
                style_box.corner_radius = 8;
                theme->collapse_container.styles["title_bar"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(223, 223, 223, 255);
                style_box.border_color = {86, 170, 114, 255};
                style_box.border_width = 2;
                style_box.corner_radius = 8;
                theme->collapse_container.styles["background"] = style_box;
            }
        }

        // Tab container
        {
            {
                auto style_box = StyleBox();
                style_box.corner_radius = 0;
                style_box.border_width = 0;
                style_box.bg_color = ColorU(200, 200, 200);
                theme->tab_container.styles["background"] = style_box;
            }
            {
                auto style_box = StyleBox();
                style_box.corner_radius = 0;
                style_box.border_width = 0;
                style_box.bg_color = ColorU(230, 230, 230);
                theme->tab_container.styles["tab_background"] = style_box;
            }
            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(230, 230, 230);
                theme->tab_container.styles["tab_button_normal"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(200, 200, 200);
                theme->tab_container.styles["tab_button_hovered"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(200, 200, 200);
                theme->tab_container.styles["tab_button_pressed"] = style_box;
            }
        }

        // Text edit
        {
            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.bg_color = ColorU(232, 232, 232);
                theme->text_edit.styles["normal"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.border_width = 2;
                style_box.bg_color = ColorU(232, 232, 232);
                style_box.border_color = ColorU(188, 188, 188);
                theme->text_edit.styles["focused"] = style_box;
            }

            theme->text_edit.caret.color = ColorU(55, 55, 55);
            theme->text_edit.caret.width = 2;

            {
                auto style_box = StyleBox();
                style_box.border_width = 0;
                style_box.corner_radius = 0;
                style_box.bg_color = ColorU(35, 92, 207);
                theme->text_edit.styles["selection"] = style_box;
            }
        }

        // Popup menu
        {
            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(223, 223, 223, 255);
                style_box.border_color = {180, 180, 180, 100};
                style_box.border_width = 2;
                style_box.corner_radius = 8;
                theme->popup_menu.styles["background"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(255, 255, 255, 0);
                style_box.border_width = 0;
                theme->popup_menu.styles["item_normal"] = style_box;
            }

            {
                auto style_box = StyleBox();
                style_box.bg_color = ColorU(188, 188, 188);
                style_box.border_width = 0;
                theme->popup_menu.styles["item_hovered"] = style_box;
            }
        }

        theme->load_system_font();

        return theme;
    }

    void load_system_font();

    std::shared_ptr<Font> font;

    uint32_t font_size = 24;

    ColorU accent_color = ColorU(214, 93, 51);

    struct {
        std::map<std::string, float> constants;
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
    } button;

    struct {
        std::map<std::string, float> constants;
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
    } label;

    struct {
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
    } panel;

    struct {
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
    } tab_container;

    struct {
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
    } collapse_container;

    struct {
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
    } popup_menu;

    struct {
        std::map<std::string, ColorU> colors;
        std::map<std::string, StyleBox> styles;
        StyleLine caret;
    } text_edit;
};

} // namespace revector
