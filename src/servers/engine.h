#pragma once

#include <chrono>
#include <map>

namespace revector {

enum class Signal {
    SwapChainChanged,
    RebuildCommandBuffer,
};

class Engine {
public:
    static Engine *get_singleton() {
        static Engine singleton;

        return &singleton;
    }

    Engine();

    void tick();

    double get_dt() const;

    double get_elapsed() const;

    float get_fps();

    int get_fps_int();

private:
#if defined(_WIN32) || defined(__APPLE__)
    std::chrono::time_point<std::chrono::steady_clock> last_time_updated_fps;
#elif __linux__
    std::chrono::time_point<std::chrono::system_clock> last_time_updated_fps;
#endif

    std::map<int64_t, float> frametimes;
    int64_t fps_average_window = 1000000000; // ns

    double elapsed = 0;
    double dt = 0;
};

} // namespace revector
