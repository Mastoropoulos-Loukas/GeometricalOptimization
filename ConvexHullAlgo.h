#ifndef CONVEX_HULL_ALGO
#define CONVEX_HULL_ALGO

#include "shared.h"
#include "PolygonGenerator.h"


class ConvexHullAlgo : public PolygonGenerator{

public:
    ConvexHullAlgo(PointList&);
    virtual Polygon_2 generatePolygon();
};

#endif