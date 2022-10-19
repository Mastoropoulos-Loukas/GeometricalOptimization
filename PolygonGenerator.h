#ifndef POLYGON_GENERATOR_H
#define POLYGON_GENERATOR_H

#include "shared.h"

class PolygonGenerator
{
private:
    const PointList *list;  //constant pointer to the list of points.

public:
    PolygonGenerator(PointList *input): list(input){};
    virtual Polygon_2 generatePolygon() = 0;    //sub classes must implent this class
};

#endif