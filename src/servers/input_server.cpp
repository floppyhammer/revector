#include "input_server.h"

#include <pathfinder/prelude.h>

#include <codecvt>
#include <locale>

#include "../nodes/sub_window.h"
#include "render_server.h"

namespace revector {

std::string cpp11_codepoint_to_utf8(char32_t codepoint) {
    char utf8[4];
    char *end_of_utf8;

    char32_t const *from = &codepoint;

    std::mbstate_t mbs;
    std::codecvt_utf8<char32_t> ccv;

    if (ccv.out(mbs, from, from + 1, from, utf8, utf8 + 4, end_of_utf8)) {
        throw std::runtime_error("Bad codepoint-to-utf8 conversion!");
    }

    return {utf8, end_of_utf8};
}

InputServer::InputServer() {
    // All remaining cursors are destroyed when glfwTerminate is called.
    arrow_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    ibeam_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    crosshair_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    resize_cursor_h = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    resize_cursor_v = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    resize_tlbr_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    resize_trbl_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
}

void InputServer::initialize_window_callbacks(uint8_t window_index) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    // A lambda function that doesn't capture anything can be implicitly converted to a regular function pointer.
    auto cursor_position_callback = [](GLFWwindow *window, double x_pos, double y_pos) {
        // Mouse position is under the logical coordinates instead of the physical ones.

        auto pf_window = reinterpret_cast<Pathfinder::Window *>(glfwGetWindowUserPointer(window));

#if defined(__linux__) || defined(_WIN32)
        // Get DPI scale.
        float dpi_scale_x, dpi_scale_y;
        glfwGetWindowContentScale(window, &dpi_scale_x, &dpi_scale_y);
        assert(dpi_scale_x == dpi_scale_y);
        x_pos /= dpi_scale_x;
        y_pos /= dpi_scale_x;
#endif

        InputEvent input_event{};
        input_event.type = InputEventType::MouseMotion;
        input_event.window_index = pf_window->window_index;
        input_event.args.mouse_motion.position = {(float)x_pos, (float)y_pos};
        auto input_server = get_singleton();
        input_server->last_cursor_position = input_server->cursor_position;
        input_server->cursor_position = {(float)x_pos, (float)y_pos};
        input_event.args.mouse_motion.relative = input_server->cursor_position - input_server->last_cursor_position;
        input_server->input_queue.push_back(input_event);
    };
    glfwSetCursorPosCallback(window, cursor_position_callback);

    auto cursor_button_callback = [](GLFWwindow *window, int button, int action, int mods) {
        auto pf_window = reinterpret_cast<Pathfinder::Window *>(glfwGetWindowUserPointer(window));

        InputEvent input_event{};
        input_event.type = InputEventType::MouseButton;
        input_event.window_index = pf_window->window_index;
        input_event.args.mouse_button.button = button;
        input_event.args.mouse_button.pressed = action == GLFW_PRESS;
        auto input_server = get_singleton();
        input_event.args.mouse_button.position = input_server->cursor_position;
        input_server->input_queue.push_back(input_event);
    };
    glfwSetMouseButtonCallback(window, cursor_button_callback);

    auto cursor_scroll_callback = [](GLFWwindow *window, double x_offset, double y_offset) {
        auto pf_window = reinterpret_cast<Pathfinder::Window *>(glfwGetWindowUserPointer(window));

        InputEvent input_event{};
        input_event.type = InputEventType::MouseScroll;
        input_event.window_index = pf_window->window_index;
        input_event.args.mouse_scroll.x_delta = x_offset;
        input_event.args.mouse_scroll.y_delta = y_offset;

        auto input_server = get_singleton();
        input_server->input_queue.push_back(input_event);
    };
    glfwSetScrollCallback(window, cursor_scroll_callback);

    auto key_callback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto input_server = get_singleton();

        auto pf_window = reinterpret_cast<Pathfinder::Window *>(glfwGetWindowUserPointer(window));

        InputEvent input_event{};
        input_event.type = InputEventType::Key;
        input_event.window_index = pf_window->window_index;

        auto &key_args = input_event.args.key;
        key_args.pressed = action == GLFW_PRESS;
        key_args.repeated = action == GLFW_REPEAT;

        switch (key) {
            case GLFW_KEY_BACKSPACE: {
                key_args.key = KeyCode::Backspace;
            } break;
            case GLFW_KEY_DELETE: {
                key_args.key = KeyCode::Delete;
            } break;
            case GLFW_KEY_LEFT: {
                key_args.key = KeyCode::Left;
            } break;
            case GLFW_KEY_RIGHT: {
                key_args.key = KeyCode::Right;
            } break;
            case GLFW_KEY_UP: {
                key_args.key = KeyCode::Up;
            } break;
            case GLFW_KEY_DOWN: {
                key_args.key = KeyCode::Down;
            } break;
            case GLFW_KEY_LEFT_CONTROL: {
                key_args.key = KeyCode::LeftControl;
            } break;
            case GLFW_KEY_LEFT_SHIFT: {
                key_args.key = KeyCode::LeftShift;
            } break;
            case GLFW_KEY_C: {
                key_args.key = KeyCode::C;
            } break;
            case GLFW_KEY_V: {
                key_args.key = KeyCode::V;
            } break;
            case GLFW_KEY_X: {
                key_args.key = KeyCode::X;
            } break;
            case GLFW_KEY_R: {
                key_args.key = KeyCode::R;
            } break;
            case GLFW_KEY_F5: {
                key_args.key = KeyCode::F5;
            } break;
            case GLFW_KEY_F10: {
                key_args.key = KeyCode::F10;
            } break;
            case GLFW_KEY_F11: {
                key_args.key = KeyCode::F11;
            } break;
            default: {
                key_args.key = KeyCode::Unknown;
            }
        }

        if (key_args.pressed) {
            input_server->keys_pressed.insert(key_args.key);
        } else if (key_args.repeated) {
        } else {
            input_server->keys_pressed.erase(key_args.key);
        }

        input_server->input_queue.push_back(input_event);
    };
    glfwSetKeyCallback(window, key_callback);

    auto character_callback = [](GLFWwindow *window, unsigned int codepoint) {
        auto pf_window = reinterpret_cast<Pathfinder::Window *>(glfwGetWindowUserPointer(window));

        InputEvent input_event{};
        input_event.type = InputEventType::Text;
        input_event.window_index = pf_window->window_index;
        input_event.args.text.codepoint = codepoint;
        auto input_server = get_singleton();
        input_server->input_queue.push_back(input_event);
    };

    glfwSetCharCallback(window, character_callback);
}

void InputServer::clear_events() {
    input_queue.clear();
}

std::string InputServer::get_clipboard(uint8_t window_index) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    auto chars = glfwGetClipboardString(window);
    return std::string(chars);
}

void InputServer::set_clipboard(uint8_t window_index, std::string text) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    glfwSetClipboardString(window, text.c_str());
}

void InputServer::set_cursor(uint8_t window_index, CursorShape shape) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    GLFWcursor *current_cursor{};

    switch (shape) {
        case CursorShape::Arrow: {
            current_cursor = arrow_cursor;
        } break;
        case CursorShape::IBeam: {
            current_cursor = ibeam_cursor;
        } break;
        case CursorShape::Crosshair: {
            current_cursor = crosshair_cursor;
        } break;
        case CursorShape::Hand: {
            current_cursor = hand_cursor;
        } break;
        case CursorShape::ResizeH: {
            current_cursor = resize_cursor_h;
        } break;
        case CursorShape::ResizeV: {
            current_cursor = resize_cursor_v;
        } break;
        case CursorShape::ResizeTlbr: {
            current_cursor = resize_tlbr_cursor;
        } break;
        case CursorShape::ResizeTrbl: {
            current_cursor = resize_trbl_cursor;
        } break;
    }

    glfwSetCursor(window, current_cursor);
}

bool InputServer::is_key_pressed(KeyCode code) const {
    return keys_pressed.find(code) != keys_pressed.end();
}

void InputServer::set_cursor_captured(uint8_t window_index, bool captured) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    glfwSetInputMode(window, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void InputServer::hide_cursor(uint8_t window_index) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void InputServer::restore_cursor(uint8_t window_index) {
    auto render_server = RenderServer::get_singleton();
    auto window = (GLFWwindow *)render_server->window_builder_->get_window(window_index).lock()->get_glfw_handle();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

} // namespace revector
