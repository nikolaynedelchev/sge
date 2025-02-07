#include "ImageViewer.h"
#include <tools/tools.h>
#include "GuiTools.h"
#include <raymath.h>
namespace ndn::rssedit
{

void ImageViewer::SetImageMgr(std::shared_ptr<sge::ImageMgr> imgMgr)
{
    m_imgMgr = imgMgr;
}

void ImageViewer::SetImage(const std::string &imageFile)
{
    m_originalTexture = m_imgMgr->Texture(imageFile);
    m_decoratedTexture = LoadRenderTexture(int(m_viewAreaSize.x), int(m_viewAreaSize.y));
    FitToViewArea();
    m_scale = {1.0f, 1.0f};
    m_scaleStep = {0.0f, 0.0f};
    m_scaleTarget = m_scale;
    GenerateBackground();
}

void ImageViewer::EnableZoom(bool isEnabled)
{

}

void ImageViewer::SetDecorator(std::function<void (Vector2 zoom, Vector2 offset)> decorator)
{
    m_decorator = std::move(decorator);
}

void ImageViewer::SetScale(Vector2 scale)
{
    if (scale.x < 0.01f) scale.x = 0.01f;
    if (scale.y < 0.01f) scale.y = 0.01f;

    Vector2 newRelativeImagePointScaled = m_zoomAnchorRelativeToImage * scale;
    m_offset = m_zoomAnchorRelativeToWindow - newRelativeImagePointScaled;
    m_scale = scale;
}

void ImageViewer::GenerateBackground()
{
    static constexpr int width = 64;
    static constexpr int height = 64;
    static constexpr int blockSz = 32;

    auto image = m_imgMgr->Create(64, 64, "img_viewer_background");
    Color *pixels = (Color *)image.data;
    for (int y = 0; y < height; y++)
    {
        bool yOdd = (y / blockSz) % 2;
        bool y2Odd = (y % blockSz) % 2;
        for (int x = 0; x < width; x++)
        {
            bool xOdd = (x / blockSz) % 2;
            bool x2Odd = (x % blockSz) % 2;

            int index = y * width + x;
            Color c;
            c.a = 200;
            if (xOdd == yOdd)
            {
                c.r = (x2Odd == y2Odd) ? 50 : 60;
                c.g = (x2Odd == y2Odd) ? 30 : 40;
                c.b = (x2Odd == y2Odd) ? 10 : 20;
            }
            else
            {
                c.r = (x2Odd == y2Odd) ? 10 : 20;
                c.g = (x2Odd == y2Odd) ? 30 : 40;
                c.b = (x2Odd == y2Odd) ? 50 : 60;
            }
            pixels[index] = c;
        }
    }
}

void ImageViewer::FitToViewArea()
{
    Vector2 scale;
    scale.x = m_viewAreaSize.x / float(m_originalTexture.width);
    scale.y = m_viewAreaSize.y / float(m_originalTexture.height);
    if (scale.x < scale.y)
    {
        scale.y = scale.x;
    }
    else
    {
        scale.x = scale.y;
    }

    m_offset.x = (m_viewAreaSize.x - m_originalTexture.width * scale.x) / 2.0f;
    m_offset.y = (m_viewAreaSize.y - m_originalTexture.height * scale.y) / 2.0f;

    m_scaleStep = {0.0f, 0.0f};
    m_scaleTarget = scale;
    m_scale = scale;
}

void ImageViewer::ResizeViewArea(Vector2 newSize)
{
    m_viewAreaSize = newSize;
    m_decoratedTexture = LoadRenderTexture(int(m_viewAreaSize.x), int(m_viewAreaSize.y));
}

void ImageViewer::Draw()
{
    Rectangle max = Rect(ImGui::GetContentRegionAvail()); //ImGui::GetWindowContentRegionMax();
    if (std::abs(max.width - m_viewAreaSize.x) > 50 || std::abs(max.height - m_viewAreaSize.y) > 50)
    {
        ResizeViewArea({max.width, max.height});
        FitToViewArea();
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && io.MouseWheel != 0.0f)
    {
        Vector2 elementPos = Vec(ImGui::GetCursorScreenPos());
        Vector2 mousePos = Vec(io.MousePos);
        m_zoomAnchorRelativeToWindow = mousePos - elementPos;
        Vector2 relativeImagePointScaled = m_zoomAnchorRelativeToWindow - m_offset;
        m_zoomAnchorRelativeToImage = relativeImagePointScaled / m_scale;

        m_scaleTarget.x += m_scaleTarget.x * (io.MouseWheel / 10.0f);
        m_scaleTarget.y += m_scaleTarget.y * (io.MouseWheel / 10.0f);
        if (m_scaleTarget.x < 0.01f) m_scaleTarget.x = 0.01f;
        if (m_scaleTarget.y < 0.01f) m_scaleTarget.y = 0.01f;
        m_scaleStep = (m_scaleTarget - m_scale) / 6.f;
    }

    if (io.MouseDown[ImGuiMouseButton_Middle])
    {
        m_offset += Vec(io.MouseDelta);
    }

    UpdateScale();

    BeginTextureMode(m_decoratedTexture);
    ClearBackground(BLACK);

    Rectangle source = { 0.0f, 0.0f,
                         (float)m_originalTexture.width, (float)m_originalTexture.height };

    Rectangle dest = Rect(m_offset, Vec(m_originalTexture.width, m_originalTexture.height) * m_scale);

    DrawTexturePro(m_imgMgr->Texture("img_viewer_background"), max, max, {0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(m_originalTexture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);

    if (m_decorator)
    {
        m_decorator(m_scale, m_offset);
    }
    EndTextureMode();

    Rectangle srcRect = {0, 0, float(m_decoratedTexture.texture.width), -float(m_decoratedTexture.texture.height)};
    GuiTools::rlImGuiImageRect(&m_decoratedTexture.texture, m_decoratedTexture.texture.width, m_decoratedTexture.texture.height, srcRect);

}

void ImageViewer::UpdateScale()
{
    if (IsEqual(m_scale, m_scaleTarget))
    {
        return;
    }

    auto newScale = m_scale + m_scaleStep;
    if (false == IsInTheMiddle(newScale, m_scale, m_scaleTarget))
    {
        SetScale(m_scaleTarget);
        return;
    }

    SetScale(newScale);
}

}
