#include <iostream>
#include <string>
#include "../common/utils.h"
#import <Foundation/Foundation.h>

namespace revector {

std::string getResourcesPath() {
    @autoreleasepool {
        // Get the main application bundle
        NSBundle* mainBundle = [NSBundle mainBundle];

        // Get the path to the Resources directory
        NSString* path = [mainBundle resourcePath];

        if (path) {
            // Convert the Objective-C string to a C++ std::string
            return {[path UTF8String]};
        }
    }
    return "";
}

std::string get_asset_dir(const std::string &relative_path) {
    auto resource_dir = getResourcesPath();

    std::string asset_dir;
    if (resource_dir.empty()) {
        Logger::error("Failed to get the resource directory, will use relative path", "revector");
        asset_dir = "assets/";
    } else {
        Logger::info("Got the resource directory: " + resource_dir, "revector");
        asset_dir = resource_dir + "/assets/";
    }

    return asset_dir + relative_path;
}

} // namespace revector
