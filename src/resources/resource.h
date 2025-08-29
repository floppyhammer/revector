#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace revector {

static std::string get_asset_dir(const std::string &relative_path) {
    // AppImage specific.
    auto app_dir = getenv("APPDIR");

    std::string asset_dir;
    if (app_dir) {
        asset_dir = std::string(app_dir) + "/assets/";
    } else {
        asset_dir = "assets/";
    }

    return asset_dir + relative_path;
}

} // namespace revector
