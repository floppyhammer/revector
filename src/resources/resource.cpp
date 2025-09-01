#include <cstdint>
#include <memory>
#include <string>

#include "src/common/utils.h"

#ifndef __APPLE__

namespace revector {

std::string get_asset_dir(const std::string &relative_path) {
    // AppImage specific.
    const auto app_dir = getenv("APPDIR");

    std::string asset_dir;
    if (app_dir) {
        Logger::info("Got the app directory: " + std::string(app_dir), "revector");
        asset_dir = std::string(app_dir) + "/assets/";
    } else {
        Logger::error("Failed to get the app directory, will use relative path", "revector");
        asset_dir = "assets/";
    }

    return asset_dir + relative_path;
}

} // namespace revector

#endif
