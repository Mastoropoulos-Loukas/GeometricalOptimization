#ifndef CONVEX_HULL_ALGO
#define CONVEX_HULL_ALGO

#include "shared.h"
#include "PolygonGenerator.h"


class ConvexHullAlgo : private PolygonGenerator{

public:
    ConvexHullAlgo(PointList&);
    Polygon_2 generatePolygon();

};

#endif