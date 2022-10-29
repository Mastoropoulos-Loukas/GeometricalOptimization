#include "ConvexHullAlgo.h"
#include <boost/optional/optional_io.hpp>

ConvexHullAlgo::ConvexHullAlgo(PointList& list) : PolygonGenerator(list){};

using CGAL::squared_distance;

bool isReplaceable(Point_2 p, Segment_2 initialEdge, Polygon_2 poly);
static PointListIterator closestPoint(PointListIterator, PointList&, Polygon_2&);
static void getClosestPointForEachEdge(PointPairList&, Polygon_2&, PointList&);
static PointPairListIterator selectEdge(PointPairList&, EdgeSelection);

template <typename T>
static void printList(std::vector<T>, std::string);

using std::cout;  using std::endl; using std::string;

Polygon_2 ConvexHullAlgo::generatePolygon(){
    Polygon_2 p;

    //polygon is convex hull at the start
    PointList temp;
    CGAL::convex_hull_2(list.begin(), list.end(), std::back_inserter(temp));
    for(auto it = temp.begin(); it != temp.end(); ++it)p.push_back(*it);

    
    printList(temp, std::string("Algo before"));

    //get uniserted points
    PointList uninserted;
    std::sort(list.begin(), list.end());
    std::sort(temp.begin(), temp.end());
    std::set_difference(list.begin(), list.end(), temp.begin(), temp.end(), std::inserter(uninserted, uninserted.end()));
    printList(uninserted, std::string("Algo uninserted before"));

    PointPairList record;
    EdgeSelection method = randomSelection;

    int step = 1;
    while(!uninserted.empty())
    {
        //get closest point for each edge
        // cout << "step " << step << " getting record" << endl;
        getClosestPointForEachEdge(record, p, uninserted);


        // cout << "step " << step << " selecting from record" << endl;
        PointPairListIterator selection = selectEdge(record, method);

        //edge selection - just the first for now
        // cout << "step " << step << " using record" << endl;
        p.insert(
            (*selection).first + 1,
            *(*selection).second
        );
        uninserted.erase((*selection).second);

        
        string a = "polygon_"  + std::to_string(step) + ".wkt";
        std::ofstream dump(a);
        CGAL::IO::write_polygon_WKT(dump, p);
        step++;
    }

    
    printList(uninserted, std::string("Algo uninserted after"));

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
    Selects an edge from <list> based of <method>. Returns iterator to selected edge.
*/
static PointPairListIterator selectEdge(PointPairList& list, EdgeSelection method)
{   
    //just return first option for now
    return list.begin();
}

/*
    Returns the closest point from <uninserted> to <edge> 
*/
static PointListIterator closestPoint(PointListIterator edgeSource, PointList& uninserted, Polygon_2& poly)
{
    PointList list;
    for(
        auto it = uninserted.begin();
        it != uninserted.end();
        ++it
    )
    {
        if(isReplaceable(
            *it, 
            Segment_2(*edgeSource, *(edgeSource+1)), 
            poly))
            list.push_back(*it);
    }

    if(list.empty())
    {
        
        printList(uninserted, "uninserted was: ");
        cout << endl << endl;

    }
    Segment_2 edge(*edgeSource, *(edgeSource+1));
    PointListIterator minIter = list.begin();
    double minDistance = squared_distance(edge, *minIter);

    for(
        PointListIterator pointIter = minIter + 1;
        pointIter != list.end();
        ++pointIter
    )
    {
        double currDistance = squared_distance(edge, *pointIter);
        if(currDistance < minDistance)
        {
            minDistance = currDistance;
            minIter = pointIter;
        }
    }

    return minIter;
}

/*
    Stores the closest point from <list> to every edge of <poly> at <record>
*/
static void getClosestPointForEachEdge(PointPairList& record, Polygon_2& poly, PointList& list)
{
    record.clear();
    for(
        PointListIterator pointIter = poly.vertices_begin();
        (pointIter != poly.vertices_end() - 1);
        ++pointIter
    )
    {
        record.push_back(
            PointPair(
                pointIter,
                closestPoint(
                    pointIter,
                    list,
                    poly
                )
            )
        );
    }
}

/*
    Assume a polygon <poly> with an edge <initialEdge> and a point <p>
    If we can break <initialEdge> (from point A to point B) and connect p (point C) with edges AC and BC so that p is added to the polygon, isReplaceable return true, else false. 
*/
bool isReplaceable(Point_2 p, Segment_2 initialEdge, Polygon_2 poly)
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

