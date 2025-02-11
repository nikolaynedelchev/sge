#pragma once
#include <string>
#include <map>
#include <vector>
#include <fmt/core.h>
#include <functional>
#include <variant>

namespace ndn::tools::rss_loader
{

struct Sprite
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int pivotX = 0;
    int pivotY = 0;
    float scale = 0.0f;
    float rotationDeg = 0.0f;
    std::string fileName;
};
std::string format_as(const Sprite& s);

using SpriteArray = std::vector<Sprite>;
std::string format_as(const SpriteArray& a);

struct Animation
{
    int pivotX = 0;
    int pivotY = 0;
    float scale = 0.0f;
    float rotationDeg = 0.0f;
    float fps = 0.0f;
    std::string spriteArray;
};
std::string format_as(const Animation& a);

struct Sound
{
    int bps = 0;
    std::string fileName;
};
std::string format_as(const Sound& s);

struct DrawCommand
{
    int x = 0;
    int y = 0;
    std::string spriteKey;
    std::pair<std::string, int> spriteArray;
};
std::string format_as(const DrawCommand& comm);

struct PlayCommand
{
    int x = 0;
    int y = 0;
    std::string animationKey;
};
std::string format_as(const PlayCommand& comm);

using Command = std::variant<DrawCommand, PlayCommand>;
std::string format_as(const Command& comm);

using Script = std::vector<Command>;
std::string format_as(const Script& s);

struct Resources
{
    std::map<std::string, Sprite> sprites;
    std::map<std::string, SpriteArray> spriteArrays;
    std::map<std::string, Animation> animations;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Script> scripts;
    std::map<std::string, Command> commands;

    void Add(const Resources&);
};
std::string format_as(const Resources& r);

Resources LoadFromLuaGenerator(const std::string& luaGeneratorScript);
SpriteArray AutoSprites(const std::vector<std::vector<uint32_t>>& imagePixels);

}


