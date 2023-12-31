#pragma once
#include "Geometry/AABB.h"
#include "Geometry/OBB.h"
#include <vector>

class OctreeNode
{
public:

    OctreeNode(AABB& box) { box = bounding_box; meshesInside = 0; }



private:
    AABB bounding_box;
    std::vector<OctreeNode*> children;
    int meshesInside;
};

