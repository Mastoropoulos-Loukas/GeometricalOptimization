#include <vector>
#include <numeric>

#include <CGAL/property_map.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/IO/WKT.h>


#define ENDL std::endl 
#define COUT std::cout

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Convex_hull_traits_adapter_2<K,
          CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;

// Just the peeling for now. NOTHING ELSE!

int main(){
  // std::vector<Point_2> points = { Point_2(10,0),
  //                                 Point_2(0,10),
  //                                 Point_2(1,1),
  //                                 Point_2(3,4),
  //                                 Point_2(0,0),
  //                                 Point_2(10,10),
  //                                 Point_2(2,6) };

  std::vector<Point_2> points = { Point_2(2102,5260),
                                  Point_2(2420,354),
                                  Point_2(3906,4366),
                                  Point_2(5184,2922),
                                  Point_2(5346,4176),
                                  Point_2(5550,4142),
                                  Point_2(5796,2822),
                                  Point_2(6384,2302),
                                  Point_2(7084,2732),
                                  Point_2(7426,394),
                                  Point_2(7672,6734),
                                  Point_2(7904,920),
                                  Point_2(8124,4912),
                                  Point_2(8396,3542),
                                  Point_2(8846,1814),
                                  Point_2(9216,2630),
                                  Point_2(9792,4640),
                                  Point_2(10188,4142),
                                  Point_2(11108,3318),
                                  Point_2(12110,496) };

  std::vector<Polygon_2> allPolys;

  std::cout<<"Points BEFORE remove"<<ENDL;
  for(int i =0; i<points.size();i++){
    COUT<<"("<<points[i]<<")"<<" ";
  }
  COUT<<ENDL;
  
  while(points.size()>2){
    std::vector<std::size_t> indices(points.size()), out;
    std::iota(indices.begin(), indices.end(),0);
    CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out),
                      Convex_hull_traits_2(CGAL::make_property_map(points)));
  
  // std::vector<Point_2> pointsLeft = points;
  // int sizeBefore = pointsLeft.size();
  // int dumAdded = 0;
  
  // std::vector<Point_2>::iterator it;
  // it=pointsLeft.begin();
    Polygon_2 poly;
    std::vector<Point_2> pointsLeftAct;

  // loop that indicates the convexHull points
    for( std::size_t i : out){

      poly.push_back(points[i]); // push the point to a polygon
      std::cout << "points[" << i << "] = " <<"("<<points[i]<<") " << std::endl;

    // if the vector of points  contains duplicates, we find them
    // for(int j=0;j<pointsLeft.size();j++){
    //   if(points[i]==pointsLeft[j]){
    //     pointsLeft[j]=Point_2(-99,-99);   // mark the convex hull points
    //   }
    // }
      points[i]=Point_2(-99,-99);   // mark the convex hull points
 
  }
    allPolys.push_back(poly);

  // we find the points that do not belong to the convexHull
    for(int i =0; i<points.size();i++){
      if(points[i]!=Point_2(-99,-99)){ // if a point is left unmarked this means that it does not belong to the convexHull
        pointsLeftAct.push_back(points[i]); // push it to the pointsLeftAct
      }
    }

    std::cout<<"Points AFTER remove"<<ENDL;
    for(int i =0; i<points.size();i++){
      COUT<<"("<<points[i]<<")"<<" ";
    }
    COUT<<ENDL;

    COUT<<"PRINTING actual points LEFT"<<ENDL;
    for(int i =0; i<pointsLeftAct.size();i++){
      COUT<<"("<<pointsLeftAct[i]<<")"<<" ";
    }
    COUT<<ENDL;
  
    points=pointsLeftAct;
  
    std::cout<<"Points AFTER REPLACE"<<ENDL;
    for(int i =0; i<points.size();i++){
      COUT<<"("<<points[i]<<")"<<" ";
    }
    COUT<<ENDL;
  }
  
  std::cout<<"FINAL Points LEFT"<<ENDL;
  for(int i =0; i<points.size();i++){
    COUT<<"("<<points[i]<<")"<<" ";
  }
  COUT<<ENDL;

  COUT<<ENDL<<"PRINTING ALLPOLYS"<<ENDL;
  for(int i =0; i<allPolys.size();i++){
    COUT<<allPolys[i]<<ENDL;

  }

  return 0;
}