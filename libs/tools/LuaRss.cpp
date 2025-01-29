#include "LuaRss.h"
#include <fmt/printf.h>
#include <optional>
#include <functional>
extern "C" {
#include <lua/lua.h>

#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace ndn::tools::rss_loader
{

static std::string s_luaHelpers = R"(

SpriteSheet = ""

function Sprite(input)
    local sprite = {}
    sprite.resource_type = "sprite"
    sprite.x = input.x or 0
    sprite.y = input.y or 0
    sprite.w = input.w or 0
    sprite.h = input.h or 0
    sprite.scale = input.scale or 0.0
    sprite.rotation = input.rotation or 0.0
    sprite.file = input.fileName or SpriteSheet

    local pivot_offsets = {
            center = { 0.5, 0.5 },
            up = { 0.5, 0.0 },
            up_left = { 0.0, 0.0 },
            up_right = { 1.0, 0.0 },
            down = { 0.5, 1.0 },
            down_left = { 0.0, 1.0 },
            down_right = { 1.0, 1.0 },
            left = { 0.0, 0.5 },
            right = { 1.0, 0.5 }
        }

    if input.pivot and pivot_offsets[input.pivot] then
        local offset = pivot_offsets[input.pivot]
        sprite.pivotX = math.floor(sprite.w * offset[1])
        sprite.pivotY = math.floor(sprite.h * offset[2])
    else
        sprite.pivotX = input.pivotX or 0
        sprite.pivotY = input.pivotY or 0
    end

    return sprite
end

function Animation(input)
    local animation = {}
    animation.resource_type = "animation"
    animation.pivotX = input.pivotX or 0
    animation.pivotY = input.pivotY or 0
    animation.scale = input.scale or 0.0
    animation.rotation = input.rotation or 0.0
    animation.fps = input.fps or 0.0

    animation.frames = {}
    if input.frames then
        for i, sprite in ipairs(input.frames) do
            animation.frames[i] = Sprite(sprite)
        end
    end

    function animation:Frame(sprite)
        table.insert(self.frames, Sprite(sprite))
        return self
    end

    return animation
end

)";

template<typename T>
using Opt = std::optional<T>;

void IterateArray(lua_State* luaState, const std::function<void(int idx)>& f)
{
    int len = luaL_len(luaState, -1);
    for (int i = 1; i <= len; i++)
    {
        lua_rawgeti(luaState, -1, i);
        f(i - 1);
        lua_pop(luaState, 1);
    }
}

void IterateTableArray(lua_State* luaState, const char* arrayName, const std::function<void(int idx)>& f)
{
    lua_getfield(luaState, -1, arrayName);

    if (!lua_istable(luaState, -1))
    {
        lua_pop(luaState, 1);  // Махаме ненужното
        return;
    }
    IterateArray(luaState, f);
    lua_pop(luaState, 1);
}

template<typename T>
Opt<T> ReadTableNum(lua_State* luaState, const char* fieldName)
{
    Opt<T> res{};
    lua_getfield(luaState, -1, fieldName);
    if (lua_isnumber(luaState, -1))
    {
        res = T(lua_tonumber(luaState, -1));
    }
    lua_pop(luaState, 1);
    return res;
}

Opt<std::string> ReadTableString(lua_State* luaState, const char* fieldName)
{
    Opt<std::string> res{};
    lua_getfield(luaState, -1, fieldName);
    if (lua_isstring(luaState, -1))
    {
        res = std::string(lua_tostring(luaState, -1));
    }
    lua_pop(luaState, 1);
    return res;
}

struct ParserCtx
{
    Resources allResources;
};

Opt<Sprite> ReadSprite(ParserCtx& ctx, lua_State* luaState)
{
    try
    {
        Sprite s;
        s.x = ReadTableNum<int>(luaState,              "x").value();
        s.y = ReadTableNum<int>(luaState,              "y").value();
        s.w = ReadTableNum<int>(luaState,              "w").value();
        s.h = ReadTableNum<int>(luaState,              "h").value();
        s.pivotX = ReadTableNum<int>(luaState,         "pivotX").value();
        s.pivotY = ReadTableNum<int>(luaState,         "pivotY").value();
        s.scale = ReadTableNum<float>(luaState,        "scale").value();
        s.rotationDeg = ReadTableNum<float>(luaState,  "rotation").value();
        s.fileName = ReadTableString(luaState,         "file").value();
        return s;
    }
    catch (...)
    {
        fmt::println(stderr, "Sprite not complete");
        return {};
    }
}

Opt<Animation> ReadAnimation(ParserCtx& ctx, lua_State* luaState)
{
    try
    {
        Animation a;
        a.pivotX = *ReadTableNum<int>(luaState,         "pivotX");
        a.pivotY = *ReadTableNum<int>(luaState,         "pivotY");
        a.scale = *ReadTableNum<float>(luaState,        "scale");
        a.rotationDeg = *ReadTableNum<float>(luaState,  "rotation");
        a.fps = *ReadTableNum<float>(luaState,          "fps");
        IterateTableArray(luaState, "frames",
        [&a, &ctx, luaState](int idx){
            auto s = ReadSprite(ctx, luaState);
            if (s.has_value())
            {
                a.frames.push_back(s.value());
            }
        });
        return a;
    }
    catch (...)
    {
        return {};
    }
}

Opt<Sound> ReadSound(ParserCtx& ctx, lua_State* luaState)
{
    try
    {
        Sound s;
        s.bps = *ReadTableNum<int>(luaState,        "bps");
        s.fileName = *ReadTableString(luaState,     "fileName");
        return s;
    }
    catch (...)
    {
        return {};
    }}

void HandleTable(ParserCtx& ctx, const std::string& tableName, lua_State* luaState)
{
    lua_getfield(luaState, -1, "resource_type");
    if (lua_isstring(luaState, -1))
    {
        std::string rssType = lua_tostring(luaState, -1);
        lua_pop(luaState, 1);
        if (rssType == "sprite")
        {
            auto spriteOpt = ReadSprite(ctx, luaState);
            if (spriteOpt.has_value())
            {
                ctx.allResources.sprites[tableName] = spriteOpt.value();
            }
        }
        else if(rssType == "animation")
        {
            auto animationOpt = ReadAnimation(ctx, luaState);
            if (animationOpt.has_value())
            {
                ctx.allResources.animations[tableName] = animationOpt.value();
            }
        }
        else if(rssType == "sound")
        {
            auto soundOpt = ReadSound(ctx, luaState);
            if (soundOpt.has_value())
            {
                ctx.allResources.sounds[tableName] = soundOpt.value();
            }
        }
        else
        {
            //fmt::println("{} is not supported", rssType);
        }
    }
    else
    {
        lua_pop(luaState, 1);
    }
}

void ReadAllGlobals(ParserCtx& ctx, lua_State* luaState)
{
    lua_pushglobaltable(luaState);

    // Iterate _G (Global table)
    lua_pushnil(luaState); // nil for table beginning
    while (lua_next(luaState, -2) != 0)
    {
        // key idx: -2, value idx: -1
        const char *key = lua_tostring(luaState, -2); // Key
        if (lua_isstring(luaState, -1))
        {
            //fmt::println("string = {}", lua_tostring(luaState, -1));
        }
        else if (lua_isnumber(luaState, -1))
        {
            //fmt::println("number = {}", lua_tonumber(luaState, -1));
        }
        else if (lua_istable(luaState, -1))
        {
            HandleTable(ctx, key, luaState);
        }
        else
        {
            //fmt::println("{} is not supported", key);
        }
        // pop current object
        lua_pop(luaState, 1);
    }
    // remove _G table (global table)
    lua_pop(luaState, 1);
}

lua_State * ExecuteLuaScript(const std::string& script)
{
    lua_State *luaState = luaL_newstate();
    if (luaState == nullptr)
    {
        fmt::println(stderr, "Cannot create Lua state.");
        return nullptr;
    }

    // Load Lua standard libs
    luaL_openlibs(luaState);
    if (luaL_dostring(luaState, s_luaHelpers.c_str()))
    {
        fmt::println(stderr, "Error loading Lua helpers: {}", lua_tostring(luaState, -1));
        lua_close(luaState);
        return nullptr;
    }
    if (luaL_dostring(luaState, script.c_str()))
    {
        fmt::println(stderr, "Error loading Lua script: {}", lua_tostring(luaState, -1));
        lua_close(luaState);
        return nullptr;
    }
    return luaState;
}


Resources LoadFromLuaGenerator(const std::string &luaGeneratorScript)
{
    auto luaState = ExecuteLuaScript(luaGeneratorScript);
    if (luaState == nullptr)
    {
        fmt::println("Error executing lua script");
        return {};
    }

    ParserCtx ctx;
    ReadAllGlobals(ctx, luaState);

    return ctx.allResources;
}

void Resources::Add(const Resources &rss)
{
    for(const auto& p : rss.sprites)
    {
        sprites[p.first] = p.second;
    }
    for(const auto& p : rss.animations)
    {
        animations[p.first] = p.second;
    }
    for(const auto& p : rss.sounds)
    {
        sounds[p.first] = p.second;
    }
}

std::string format_as(const Sprite &s)
{
    return fmt::format("x: {}\ny: {}\nw: {}\nh: {}\npivotX: {}\npivotY: {}\nscale: {}\nrotationDeg: {}\nfileName: {}\n",
                        s.x,s.y,s.w,s.h,s.pivotX,s.pivotY,s.scale,s.rotationDeg,s.fileName);
}

std::string format_as(const Animation &a)
{
    auto str = fmt::format("pivotX: {}\npivotY: {}\nscale: {}\nrotationDeg: {}\nfps: {}\nFrames:[\n",
                            a.pivotX,a.pivotY,a.scale,a.rotationDeg,a.fps);
    for(size_t i = 0; i < a.frames.size(); i++)
    {
        str += fmt::format("Frame {}:\n{}\n", i, a.frames[i]);
    }
    return str += "]\n";
}

std::string format_as(const Sound &s)
{
    return fmt::format("bps: {}, fileName: {}", s.bps, s.fileName);
}


std::string format_as(const Resources &r)
{
    std::string str = "Sprites:\n";
    for(const auto& p : r.sprites)
    {
        str += fmt::format("key: {}, sprite:\n{}\n", p.first, p.second);
    }
    str += "\nAnimations:\n";
    for(const auto& p : r.animations)
    {
        str += fmt::format("key: {}, animation:\n{}", p.first, p.second);
    }
    str += "Sounds:\n";
    for(const auto& p : r.sounds)
    {
        str += fmt::format("key: {}, sound: {}\n", p.first, p.second);
    }
    return str + "\n";
}

}

