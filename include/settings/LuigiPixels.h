#pragma once

#include "al/Library/LiveActor/LiveActor.h"

namespace btt {

class LuigiPixels {
public:
    static void update(al::LiveActor* player);
    static void draw();
    static void resetDiscoveredSpots();
};

} // namespace btt
