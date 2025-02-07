#pragma once
#include <raylib.h>
#include <raymath.h>
#include <map>
#include <vector>
#include <string>

namespace ndn::sge
{
    class ImageMgr
    {
    public:
        const Image& Load(const std::string& file);
        std::vector<std::vector<uint32_t>> Pixels(const std::string& file);
        const Texture2D& Texture(const std::string& file);

        void UnloadAll();
        void UnloadAllImages();
        void UnloadAllTextures();
        void UnloadImage(const std::string& file);
        void UnloadTexture(const std::string& file);
    private:
        Image& LoadMutable(const std::string& file);

        std::map<std::string, Image> m_images;
        std::map<std::string, Texture2D> m_textures;
    };
}
