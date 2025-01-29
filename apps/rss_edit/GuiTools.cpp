#include "GuiTools.h"

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
}
