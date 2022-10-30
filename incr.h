
#ifndef Incremental
#define Incremental
#include "PolygonGenerator.h"
#include "shared.h"
#include"Pick.h"

#include "PolygonGenerator.h"




class IncAlgo : public PolygonGenerator{
private:
ArgFlags argFlags;
public:
    IncAlgo(PointList&);
    virtual Polygon_2 generatePolygon(ArgFlags);
};


#endif