#pragma once
#include <imgui/imgui.h>
#include <raylib.h>
#include <string>
#include <functional>

namespace ndn::rssedit
{

class ImageViewer
{
public:
    void SetImage(const std::string& imageFile);
    void EnableZoom(bool isEnabled);
    void SetDecorator(std::function<void(Vector2 currentSize, Vector2 srcTopLeft, Vector2 srcDnRight)> decorator);
    void SetScale(float scale);
    void FitToViewArea();

    void ResizeViewArea(Vector2 newSize);
    void Draw();

private:
    bool m_isZoomEnabled;

    Vector2 m_offset = {0.0f, 0.0f};
    Vector2 m_scale = {1.0f, 1.0f};
    Vector2 m_viewAreaSize = {256.0f, 256.0f};
    Texture2D m_originalTexture;
    RenderTexture2D m_decoratedTexture;
    std::function<void(Vector2 currentSize, Vector2 srcTopLeft, Vector2 srcDnRight)> m_decorator;

};

}
