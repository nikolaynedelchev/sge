#include "image_mgr.h"

namespace ndn::sge
{

const Image &ImageMgr::Load(const std::string &file)
{
    return LoadMutable(file);
}

Image &ImageMgr::Create(int width, int height, const std::string& key)
{
    auto it = m_images.find(key);
    if (it != m_images.end())
    {
        auto& img = it->second;
        if (img.width == width && img.height == height)
        {
            return img;
        }
        UnloadImage(key);
    }
    Image image = GenImageColor(width, height, BLACK);
    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    m_images[key] = image;
    return m_images.at(key);
}

std::vector<std::vector<uint32_t>> ImageMgr::Pixels(const std::string &file)
{
    std::vector<std::vector<uint32_t>> result;
    auto& img = LoadMutable(file);
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    result.resize(img.height);
    Color *pixels = (Color *)img.data;
    for (int y = 0; y < img.height; y++)
    {
        result[y].resize(img.width);
        for (int x = 0; x < img.width; x++)
        {
            int index = y * img.width + x;
            const auto& col = pixels[index];
            uint32_t code = (col.r << 0) | (col.g << 8) | (col.b << 16) | (col.a << 24);
            result[y][x] = code;
        }
    }
    return result;
}

const Texture2D &ImageMgr::Texture(const std::string& file)
{
    auto it = m_textures.find(file);
    if (it != m_textures.end())
    {
        return it->second;
    }
    auto img = Load(file);
    Texture2D texture = LoadTextureFromImage(img);
    m_textures[file] = texture;
    return m_textures.at(file);
}

void ImageMgr::UnloadImage(const std::string &file)
{
    auto it = m_images.find(file);
    if(it != m_images.end() && it->second.data != nullptr)
    {
        ::UnloadImage(it->second);
        m_images.erase(it);
    }
}

void ImageMgr::UnloadTexture(const std::string &file)
{
    auto it = m_textures.find(file);
    if (it != m_textures.end() && it->second.id != 0)
    {
        ::UnloadTexture(it->second);
        m_textures.erase(it);
    }
}

void ImageMgr::UnloadAll()
{
    UnloadAllTextures();
    UnloadAllImages();
}

void ImageMgr::UnloadAllImages()
{
    for(auto& p : m_images)
    {
        if (p.second.data != nullptr)
        {
            ::UnloadImage(p.second);
        }
    }
    m_images.clear();
}

void ImageMgr::UnloadAllTextures()
{
    for(auto& p : m_textures)
    {
        if (p.second.id != 0)
        {
            ::UnloadTexture(p.second);
        }
    }
    m_textures.clear();
}

Image &ImageMgr::LoadMutable(const std::string &file)
{
    auto it = m_images.find(file);
    if (it != m_images.end())
    {
        return it->second;
    }
    std::string fullFileName = file;
    m_images[file] = LoadImage(fullFileName.c_str());
    return m_images.at(file);
}

}
