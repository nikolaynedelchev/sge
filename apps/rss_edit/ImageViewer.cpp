#include "ImageViewer.h"
#include <tools/tools.h>
#include "GuiTools.h"
#include <raymath.h>
namespace ndn::rssedit
{

void ImageViewer::SetImage(const std::string &imageFile)
{
    m_originalTexture = GuiTools::ImageAsTexture(imageFile);
    m_decoratedTexture = LoadRenderTexture(int(m_viewAreaSize.x), int(m_viewAreaSize.y));
    FitToViewArea();
    m_scale = {1.0f, 1.0f};
    m_scaleStep = {0.0f, 0.0f};
    m_scaleTarget = m_scale;
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

    ImVec2 max = ImGui::GetContentRegionAvail(); //ImGui::GetWindowContentRegionMax();
    if (std::abs(max.x - m_viewAreaSize.x) > 50 || std::abs(max.y - m_viewAreaSize.y) > 50)
    {
        ResizeViewArea({max.x, max.y});
        FitToViewArea();
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && io.MouseWheel != 0.0f)
    {
        Vector2 elementPos = {ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y};
        Vector2 mousePos = {io.MousePos.x, io.MousePos.y};
        m_zoomAnchorRelativeToWindow = mousePos - elementPos;
        Vector2 relativeImagePointScaled = m_zoomAnchorRelativeToWindow - m_offset;
        m_zoomAnchorRelativeToImage = relativeImagePointScaled / m_scale;

        //m_scaleTarget = m_scale;

        m_scaleTarget.x += m_scaleTarget.x * (io.MouseWheel / 10.0f);
        m_scaleTarget.y += m_scaleTarget.y * (io.MouseWheel / 10.0f);
        if (m_scaleTarget.x < 0.01f) m_scaleTarget.x = 0.01f;
        if (m_scaleTarget.y < 0.01f) m_scaleTarget.y = 0.01f;
        m_scaleStep = (m_scaleTarget - m_scale) / 6.f;


        // Vector2 newRelativeImagePointScaled = m_zoomAnchorRelativeToImage * m_scaleTarget;
        // m_offset = m_zoomAnchorRelativeToWindow - newRelativeImagePointScaled;


        // m_scale = m_scaleTarget;
        //ImGui::Text("Scroll detected with Ctrl!");
        //ImGui::Text("Relative Mouse Position: (%.2f, %.2f)", relative_mouse_pos.x, relative_mouse_pos.y);
    }

    if (io.MouseDown[ImGuiMouseButton_Middle])
    {
        ImVec2 mouseDelta = io.MouseDelta;
        m_offset.x += mouseDelta.x;
        m_offset.y += mouseDelta.y;
    }

    UpdateScale();

    BeginTextureMode(m_decoratedTexture);
    ClearBackground(BLACK);

    Rectangle source = { 0.0f, 0.0f,
                         (float)m_originalTexture.width, (float)m_originalTexture.height };

    Rectangle dest = { m_offset.x, m_offset.y,
                       (float)m_originalTexture.width*m_scale.x, (float)m_originalTexture.height*m_scale.y };

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
