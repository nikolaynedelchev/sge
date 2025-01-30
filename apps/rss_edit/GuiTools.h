#include <imgui/imgui.h>
#include <raylib.h>
#include <string>

namespace ndn::rssedit::GuiTools
{
bool Splitter(bool vertical, float thickness, float* size1, float* size2, float minSize1, float minSize2);
void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);
Texture ImageAsTexture(const std::string& file);

}
