#include "ImageViewer.h"
#include <tools/tools.h>
#include "GuiTools.h"
namespace ndn::rssedit
{

void ImageViewer::SetImage(const std::string &imageFile)
{
    m_originalTexture = GuiTools::ImageAsTexture(imageFile);
    m_decoratedTexture = LoadRenderTexture(int(m_viewAreaSize.x), int(m_viewAreaSize.y));
}

void ImageViewer::EnableZoom(bool isEnabled)
{

}

void ImageViewer::SetDecorator(std::function<void (Vector2, Vector2, Vector2)> decorator)
{

}

void ImageViewer::SetScale(float scale)
{
    m_scale = {scale, scale};
}

void ImageViewer::FitToViewArea()
{
    m_scale.x = m_viewAreaSize.x / float(m_originalTexture.width);
    m_scale.y = m_viewAreaSize.y / float(m_originalTexture.height);
    if (m_scale.x < m_scale.y)
    {
        m_scale.y = m_scale.x;
    }
    else
    {
        m_scale.x = m_scale.y;
    }
}

void ImageViewer::ResizeViewArea(Vector2 newSize)
{
    m_viewAreaSize = newSize;
    m_decoratedTexture = LoadRenderTexture(int(m_viewAreaSize.x), int(m_viewAreaSize.y));
}

void ImageViewer::Draw()
{
    BeginTextureMode(m_decoratedTexture);
    ClearBackground(BLACK);

    Rectangle source = { 0.0f, 0.0f,
                         (float)m_originalTexture.width, (float)m_originalTexture.height };

    Rectangle dest = { m_offset.x, m_offset.y,
                       (float)m_originalTexture.width*m_scale.x, (float)m_originalTexture.height*m_scale.y };

    DrawTexturePro(m_originalTexture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);

    if (m_decorator)
    {
        m_decorator({float(dest.width), float(dest.height)},
                    {float(m_originalTexture.width), float(m_originalTexture.height)},
                    {1.0f, 1.0f});
    }
    EndTextureMode();

    //Rectangle srcRect = {0, 0, float(target.texture.width), -float(target.texture.height)};
    //rlImGuiImageRect(&target.texture, target.texture.width * 0.5f, target.texture.height * 0.5f, srcRect);

}

}
