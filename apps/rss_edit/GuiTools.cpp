#include "GuiTools.h"
#include <cmath>
#include <fmt/printf.h>

namespace ndn::rssedit::GuiTools
{
bool Splitter(bool vertical, float thickness, float* size1, float* size2, float minSize1, float minSize2)
{
    ImVec2 cursorPos = ImGui::GetCursorPos();
    bool hovered, held;
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushID(size1);
    ImGui::Button("", vertical ? ImVec2(thickness, -1) : ImVec2(-1, thickness));
    ImGui::PopID();
    ImGui::SetItemAllowOverlap();

    hovered = ImGui::IsItemHovered();
    held = ImGui::IsItemActive();

    if (hovered || held)
        ImGui::SetMouseCursor(vertical ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS);

    if (held)
    {
        float delta = vertical ? ImGui::GetIO().MouseDelta.x : ImGui::GetIO().MouseDelta.y;
        if (delta < 0.0f && *size1 + delta < minSize1) delta = minSize1 - *size1;
        if (delta > 0.0f && *size2 - delta < minSize2) delta = *size2 - minSize2;
        *size1 += delta;
        *size2 -= delta;
    }

    return held;
}


void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect)
{
    if (!image)
        return;

    ImVec2 uv0;
    ImVec2 uv1;

    if (sourceRect.width < 0)
    {
        uv0.x = -((float)sourceRect.x / image->width);
        uv1.x = (uv0.x - (float)(std::fabs(sourceRect.width) / image->width));
    }
    else
    {
        uv0.x = (float)sourceRect.x / image->width;
        uv1.x = uv0.x + (float)(sourceRect.width / image->width);
    }

    if (sourceRect.height < 0)
    {
        uv0.y = -((float)sourceRect.y / image->height);
        uv1.y = (uv0.y - (float)(std::fabs(sourceRect.height) / image->height));
    }
    else
    {
        uv0.y = (float)sourceRect.y / image->height;
        uv1.y = uv0.y + (float)(sourceRect.height / image->height);
    }

    ImGui::Image((ImTextureID)image, ImVec2(float(destWidth), float(destHeight)), uv0, uv1);
}

Texture ImageAsTexture(const std::string &file)
{
    std::string fullFileName = file;
    Image image = LoadImage(fullFileName.c_str());
    if (image.data == nullptr)
    {
        fmt::println("Error: Empty image: key: {} , full: {}", file, fullFileName);
        return Texture();
    }
    Texture2D texture = LoadTextureFromImage(image);
    if (texture.id == 0)
    {
        UnloadImage(image);
        fmt::println("Error: Texture construct failed: key: {} , full: {}", file, fullFileName);
        return Texture();
    }
    UnloadImage(image);
    return texture;
}

}
