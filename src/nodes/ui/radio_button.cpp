#include "radio_button.h"

#include "../../resources/default_resource.h"

namespace revector {

RadioButton::RadioButton() {
    type = NodeType::RadioButton;

    toggle_mode = true;

    label->set_text("Radio Button");

    theme_pressed = theme_normal;

    icon_normal_ = ResourceManager::get_singleton()->load<VectorImage>(get_asset_dir("icons/GuiRadioUnchecked.svg"));
    icon_pressed_ = ResourceManager::get_singleton()->load<VectorImage>(get_asset_dir("icons/GuiRadioChecked.svg"));
}

} // namespace revector
