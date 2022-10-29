#ifndef ONION
#define ONION

#include "shared.h"
#include "PolygonGenerator.h"

#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>

#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_2_algorithms.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <vector>
#include <numeric>
#include <cstdlib>


#define ENDL std::endl 
#define COUT std::cout


typedef CGAL::Polygon_set_2<Kernel, std::vector<Kernel::Point_2>> Polygon_set_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;

typedef CGAL::Convex_hull_traits_adapter_2<Kernel,
          CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;

class OnionAlgo : private PolygonGenerator{

public:
    OnionAlgo(PointList&);
    Polygon_2 generatePolygon();

};

#endif