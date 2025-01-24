#include "components.h"

namespace ndn::pong::comp
{

void Sprite::SetAnchor(Anchors a)
{
    anchor = CalculateAnchor(a, src.width, src.height);
}

}
