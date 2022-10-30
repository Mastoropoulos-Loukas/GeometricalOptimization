
#ifndef Incrimental
#define Incrimental
#include "PolygonGenerator.h"
#include "shared.h"
#include"Pick.h"

#include "PolygonGenerator.h"


class IncAlgo : private PolygonGenerator{

public:
    IncAlgo(PointList&);
    Polygon_2 generatePolygon();

};

#endif