#include "ConvexHullAlgo.h"
#include <boost/optional/optional_io.hpp>

ConvexHullAlgo::ConvexHullAlgo(PointList& list) : PolygonGenerator(list){};

using CGAL::squared_distance, CGAL::IO::write_multi_point_WKT, CGAL::IO::write_polygon_WKT;
using std::string;

template <typename T>
static void printList(std::vector<T>, std::string);
bool isReplaceable(Point_2, Segment_2, Polygon_2&);
Point_2 closestReplaceable(Segment_2, Polygon_2&, PointList&);
void allClosestReplaceable(Polygon_2&, PointList&, PointPairList&);

using std::cout;  using std::endl; using std::string;

Polygon_2 ConvexHullAlgo::generatePolygon(){
    Polygon_2 p;

    //polygon is convex hull at the start
    PointList temp;
    CGAL::convex_hull_2(list.begin(), list.end(), std::back_inserter(temp));
    for(auto it = temp.begin(); it != temp.end(); ++it)p.push_back(*it);

    //get uniserted points
    PointList uninserted;
    std::sort(list.begin(), list.end());
    std::sort(temp.begin(), temp.end());
    std::set_difference(list.begin(), list.end(), temp.begin(), temp.end(), std::inserter(uninserted, uninserted.end()));

    // int step = 1;
    PointPairList record;
    int step = 1;
    PointList writing;
    allClosestReplaceable(p, uninserted, record);

    //log out result
    for(
        auto it = record.begin();
        it != record.end();
        ++it
    )
    {
        writing.clear();
        writing.push_back(
            (*it).first
        );
        writing.push_back(
            (*it).second
        );
        string filename = "pointpair_" + std::to_string(step) + ".wkt";
        std::ofstream dump(filename);
        write_multi_point_WKT(dump, writing);
        step++;
    }

    return p;
}

template <typename T>
static void printList(std::vector<T> list, std::string msg)
{
    cout << endl << msg << endl;
    for(auto it = list.begin(); it != list.end(); ++it)
        cout << *it << endl;
}

/*
    allClosestReplaceable finds the closest replaceable point from <list> for every edge of <polygon> and stores the result in <record>
*/
void allClosestReplaceable(Polygon_2& polygon, PointList& list, PointPairList& record)
{
    record.clear();
    for(
            auto edgeIter = polygon.edges_begin();
            edgeIter != polygon.edges_end();
            ++edgeIter
        )
        {
            Segment_2 seg = *edgeIter;
            Point_2 p1 = seg[0];
            Point_2 p2 = closestReplaceable(seg, polygon, list);

            record.push_back( PointPair(p1, p2) );
        }
}

/*
    closestReplaceable returns the closest to <segment> point from <list> that has true value for isReplaceable(point, segment, <polygon>)
*/
Point_2 closestReplaceable(Segment_2 segment, Polygon_2& polygon, PointList& list)
{
    double minDistance;
    Point_2 minPoint;

    PointListIterator iter;

    //find first replaceable point
    for(
        iter = list.begin();
        iter != list.end();
        ++iter
    )
    {
        if(isReplaceable(*iter, segment, polygon))
        {
            minPoint = *iter;
            minDistance = squared_distance(segment, minPoint);
            break;
        }
    }

    //check for replaceable point with smaller distance
    for(
        iter++;
        iter != list.end();
        ++iter
    )
    {
        if(isReplaceable(*iter, segment, polygon))
        {
            double temp = squared_distance(segment, *iter);
            if(temp < minDistance)
            {
                minPoint = *iter;
                minDistance = temp;
            }
        }
    }

    return minPoint;
}

/*
    Assume a polygon <poly> with an edge <initialEdge> and a point <p>
    If we can break <initialEdge> (from point A to point B) and connect p (point C) with edges AC and BC so that p is added to the polygon, isReplaceable return true, else false. 
*/
bool isReplaceable(Point_2 p, Segment_2 initialEdge, Polygon_2& poly)
{

    Point_2 v1 = initialEdge[0];
    Point_2 v2 = initialEdge[1];
    Segment_2 edge1(v1, p);
    Segment_2 edge2(p, v2);

    for(
        auto it = poly.edges_begin();
        it != poly.edges_end();
        ++it
    )
    {
        Segment_2 curr = *it;
        boost::variant<Point_2, Segment_2> var1(curr[0]);
        boost::variant<Point_2, Segment_2> var2(curr[1]);

        if(do_intersect(edge1, curr))
        {
            auto val = intersection(edge1, curr).value();
            if(val != var1 && val != var2)
            {
                return false;
            }
        }
        if(do_intersect(edge2, curr))
        {

            auto val = intersection(edge2, curr).value();
            if(val != var1 && val != var2)
            {
                return false;
            }
        }
    }
    return true;
}

