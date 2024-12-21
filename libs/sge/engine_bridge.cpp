#include "engine_bridge.h"
#include "casts.h"
// #include <map>
// #include "libs.h"

#include <raylib.h>
// #define RAYGUI_IMPLEMENTATION
// #include <raygui.h>
// #undef RAYGUI_IMPLEMENTATION

#include "operators.h"
//#include "formatters.hpp"
#include <cmath>
#include <thread>
#include <chrono>

namespace ndn::sge
{
    static std::string rss_folder_;
    void rss_folder(std::string folder)
    {
        rss_folder_ = std::move(folder);
    }
    const std::string& rss_folder()
    {
        return rss_folder_;
    }

    void point::draw(color c, bool bold) const
    {
        if (bold)
        {
            DrawCircle(int(this->x), int(this->y), 3.0f, cast(c));
        }
        else
        {
            DrawPixel(int(this->x), int(this->y), cast(c));
        }
    }

    float point::distance(const point& p) const
    {
        return std::hypot(this->x - p.x, this->y - p.y);
    }

    point rect::position() const
    {
        return {x, y};
    }

    vec rect::size() const
    {
        return {width, height};
    }

    void rect::position(ndn::sge::point p)
    {
        this->x = p.x;
        this->y = p.y;
    }

    void rect::size(ndn::sge::vec s)
    {
        this->width = s.x;
        this->height = s.y;
    }

    ndn::sge::rect rect::bounding_rect(int spacing) const
    {
        ndn::sge::rect b = *this;
        b.x -= spacing;
        b.y -= spacing;
        b.width += 2*spacing;
        b.height += 2*spacing;
        return b;
    }

    void rect::draw(color c, bool filled) const
    {
        if (filled)
        {
            DrawRectangle(int(this->x), int(this->y), int(this->width), int(this->height), cast(c));
        }
        else
        {
            DrawRectangleLines(int(this->x), int(this->y), int(this->width), int(this->height), cast(c));
        }
    }

    ndn::sge::point rect::anchor_pos(anchors a) const
    {
        ndn::sge::point res;
        switch (a)
        {
        case ndn::sge::anchors::up_left:      return this->position();
        case ndn::sge::anchors::up_mid:       return this->position() + ndn::sge::vec(this->width / 2.0f, 0.0f);
        case ndn::sge::anchors::up_right:     return this->position() + ndn::sge::vec(this->width, 0.0f);
        case ndn::sge::anchors::mid_left:     return this->position() + ndn::sge::vec(0.0f, this->height / 2.0f);
        case ndn::sge::anchors::centered:     return this->position() + ndn::sge::vec(this->width / 2.0f, this->height / 2.0f);
        case ndn::sge::anchors::mid_right:    return this->position() + ndn::sge::vec(this->width, this->height / 2.0f);
        case ndn::sge::anchors::down_left:    return this->position() + ndn::sge::vec(0.0f, this->height);
        case ndn::sge::anchors::down_mid:     return this->position() + ndn::sge::vec(this->width / 2.0f, this->height);
        case ndn::sge::anchors::down_right:   return this->position() + ndn::sge::vec(this->width, this->height);
        default: break;
        }
        return ndn::sge::point();
    }

    ndn::sge::rect rect::anchor_rect(anchors a) const
    {
        return { position() - ndn::sge::rect(0, 0, width, height).anchor_pos(a), {width, height}};
    }

    bool rect::collision(const rect &r1, const rect &r2)
    {
        return CheckCollisionRecs(cast(r1), cast(r2));
    }



    // namespace window
    // {
        static unsigned s_window_config_flags = 0;
        vec engine::init(std::string game_folder,
                         int width, int height,
                         const std::string& window_title,
                         bool fullscreen)
        {
            rss_folder(game_folder);
            s_window_config_flags |= FLAG_MSAA_4X_HINT;
            s_window_config_flags |= FLAG_VSYNC_HINT;
            if (fullscreen)
            {
                s_window_config_flags |= FLAG_FULLSCREEN_MODE;
            }
            else
            {
                s_window_config_flags |= FLAG_WINDOW_RESIZABLE;
            }
            SetConfigFlags(s_window_config_flags);
            auto title = window_title;
            if (title.empty())
            {
                title = game_folder;
            }
            InitWindow(width, height, window_title.c_str());

            InitAudioDevice();
            return {float(width), float(height)};
        }

        vec engine::init_fs(std::string game_folder, const std::string &window_title)
        {
            init(game_folder, 0, 0, window_title, false);
            int w = GetScreenWidth();
            int h = GetScreenHeight();
            ToggleFullscreen();
            return vec(float(w), float(h));
        }

        bool engine::should_close()
        {
            return WindowShouldClose();
        }

        void engine::close()
        {
            CloseAudioDevice();
            CloseWindow();
        }

        void engine::toggle_fullscreen_win()
        {
            ToggleFullscreen();
        }
    // }

    // namespace time
    // {
        void engine::target_fps(int fps)
        {
            SetTargetFPS(fps);
        }

        int engine::target_fps()
        {
            return GetFPS();
        }

        float engine::frame_time()
        {
            return GetFrameTime();
        }

        double engine::global_time()
        {
            return GetTime();
        }

        int64_t engine::time_since_epoch()
        {
            auto now = std::chrono::system_clock::now();
            auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
            return now_ms.count();
        }
    // }
        static bool is_in_clipping = false;
        static bool is_in_transform = false;

        void engine::begin_frame()
        {
            BeginDrawing();
        }

        void engine::end_frame()
        {
            clipping_off();
            camera_off();
            EndDrawing();
        }

        void engine::clear_frame(color c)
        {
            ClearBackground(cast(c));
        }

        void engine::clipping_on(ndn::sge::rect r)
        {
            clipping_off();
            BeginScissorMode(int(r.x), int(r.y), int(r.width), int(r.height));
            is_in_clipping = true;
        }

        void engine::clipping_off()
        {
            if (is_in_clipping)
            {
                EndScissorMode();
            }
            is_in_clipping = false;
        }

        void engine::camera_on(camera c)
        {
            camera_on(c.offset, c.scale, c.rotation);
        }

        void engine::camera_on(ndn::sge::vec offset, float scale, float rotation)
        {
            camera_off();
            Camera2D camera;
            camera.offset = cast(offset);
            camera.rotation = rotation;
            camera.zoom = scale;
            camera.target = cast(ndn::sge::vec{0, 0});
            BeginMode2D(camera);
            is_in_transform = true;
        }
        void engine::camera_off()
        {
            if (is_in_transform)
            {
                EndMode2D();
            }
            is_in_transform = false;
        }

        bool engine::key_up(keys::kbd_key k)
        {
            return IsKeyUp(cast(k));
        }

        bool engine::key_down(keys::kbd_key k)
        {
            return IsKeyDown(cast(k));
        }

        bool engine::key_pressed(keys::kbd_key k)
        {
            return IsKeyPressed(cast(k));
        }

        bool engine::key_released(keys::kbd_key k)
        {
            return IsKeyReleased(cast(k));
        }

        void engine::set_exitkey(keys::kbd_key k)
        {
            SetExitKey(cast(k));
        }

        keys::kbd_key engine::key_pressed()
        {
            return cast(KeyboardKey(GetKeyPressed()));
        }

        int engine::char_pressed()
        {
            return GetCharPressed();
        }

        bool engine::mouse_btn_pressed(int button)
        {
            return IsMouseButtonPressed(button);
        }

        bool engine::mouse_btn_down(int button)
        {
            return IsMouseButtonDown(button);
        }

        bool engine::mouse_btn_released(int button)
        {
            return IsMouseButtonReleased(button);
        }

        bool engine::mouse_btn_up(int button)
        {
            return IsMouseButtonUp(button);
        }

        int engine::mouse_x(void)
        {
            return GetMouseX();
        }

        int engine::mouse_y(void)
        {
            return GetMouseY();
        }

        point engine::mouse_position(void)
        {
            return cast(GetMousePosition());
        }

        vec engine::mouse_delta(void)
        {
            return cast(GetMouseDelta());
        }

        void engine::mouse_position(int x, int y)
        {
            SetMousePosition(x, y);
        }

        void engine::mouse_offset(int offsetX, int offsetY)
        {
            SetMouseOffset(offsetX, offsetY);
        }

        void engine::mouse_scale(float scaleX, float scaleY)
        {
            SetMouseScale(scaleX, scaleY);
        }

        float engine::mouse_wheel_move(void)
        {
            return GetMouseWheelMove();
        }

        vec engine::mouse_wheel_move_v(void)
        {
            return cast(GetMouseWheelMoveV());
        }

        void engine::mouse_cursor_set(int cursor)
        {
            SetMouseCursor(cursor);
        }

        void engine::mouse_cursor_hide()
        {
            HideCursor();
        }

        void engine::mouse_cursor_show()
        {
            ShowCursor();
        }

        void engine::sleep(int milli)
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(milli) );
        }

        float line::length() const
        {
            return p1.distance(p2);
        }

        void line::draw(color c, float thick) const
        {
            DrawLineEx(cast(p1), cast(p2), thick, cast(c));
        }

}
