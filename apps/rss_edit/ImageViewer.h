#pragma once
#include <imgui/imgui.h>
#include <raylib.h>
#include <string>
#include <functional>
#include <sge/image_mgr.h>
#include <memory>

namespace ndn::rssedit
{

class ImageViewer
{
public:
    void SetImageMgr(std::shared_ptr<sge::ImageMgr>);
    void SetImage(const std::string& imageFile);
    void SetBackground(const std::string& imageFile);
    void EnableZoom(bool isEnabled);
    void SetDecorator(std::function<void(Vector2 zoom, Vector2 offset)> decorator);
    void FitToViewArea();

    void ResizeViewArea(Vector2 newSize);
    void Draw();
    void UpdateScale();

private:
    void SetScale(Vector2 scale);
    bool m_isZoomEnabled;

    Vector2 m_offset = {0.0f, 0.0f};
    Vector2 m_scale = {1.0f, 1.0f};
    Vector2 m_viewAreaSize = {256.0f, 256.0f};
    Texture2D m_originalTexture;
    RenderTexture2D m_decoratedTexture;
    std::function<void(Vector2 zoom, Vector2 offset)> m_decorator;
    Vector2 m_scaleStep;
    Vector2 m_scaleTarget;
    Vector2 m_zoomAnchorRelativeToImage;
    Vector2 m_zoomAnchorRelativeToWindow;
    std::shared_ptr<sge::ImageMgr> m_imgMgr;
    Texture2D m_backgroundTexture;
};

}
