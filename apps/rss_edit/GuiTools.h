#include <imgui/imgui.h>
#include <raylib.h>
#include <string>
#include <raymath.h>

namespace ndn::rssedit
{
    static inline bool IsEqual(float f1, float f2)
    {
        return FloatEquals(f1, f2);
    }

    static inline bool IsEqual(Vector2 v1, Vector2 v2)
    {
        return IsEqual(v1.x, v2.x) && IsEqual(v1.y, v2.y);
    }

    static inline bool IsLess(float f1, float f2)
    {
        return (!IsEqual(f1, f2) && f1 < f2);
    }

    static inline bool IsBigger(float f1, float f2)
    {
        return (!IsEqual(f1, f2) && f1 > f2);
    }

    static inline bool IsInTheMiddle(float middleNumber, float bound1, float bound2)
    {
        return (IsLess(bound1, middleNumber) && IsBigger(bound2, middleNumber)) ||
               (IsLess(bound2, middleNumber) && IsBigger(bound1, middleNumber));
    }

    static inline bool IsInTheMiddle(Vector2 middleVector, Vector2 bound1, Vector2 bound2)
    {
        return IsInTheMiddle(middleVector.x, bound1.x, bound2.x) && IsInTheMiddle(middleVector.y, bound1.y, bound2.y);
    }

    template<typename T1, typename T2>
    static inline Vector2 Vec(T1 x, T2 y)
    {
        return Vector2{float(x), float(y)};
    }

    static inline Vector2 Vec(const ImVec2& v)
    {
        return Vector2{float(v.x), float(v.y)};
    }

    template<typename T1, typename T2, typename T3, typename T4>
    static inline Rectangle Rect(T1 x, T2 y, T3 w, T4 h)
    {
        return Rectangle{float(x), float(y), float(w), float(h)};
    }

    static inline Rectangle Rect(const Vector2& pos, const Vector2& size)
    {
        return Rectangle{float(pos.x), float(pos.y), float(size.x), float(size.y)};
    }
}

namespace ndn::rssedit::GuiTools
{
bool Splitter(bool vertical, float thickness, float* size1, float* size2, float minSize1, float minSize2);
void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);
Texture ImageAsTexture(const std::string& file);

void DrawBorders(Rectangle rect, float thick, Color color);

}
