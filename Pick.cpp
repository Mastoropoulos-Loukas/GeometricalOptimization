#include "shared.h"
#include "PolygonGenerator.h"
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K>                                  Polygon_2;
 
#include <CGAL/IO/WKT.h>
struct points{


  double a=0;
  double b=0;
};
points check_inside(Point pt, Point *pgn_begin, Point *pgn_end, K traits)
{ points temp;
  switch(CGAL::bounded_side_2(pgn_begin, pgn_end,pt, traits)) {
    case CGAL::ON_BOUNDED_SIDE :
      temp.a=1;

      break;
    case CGAL::ON_BOUNDARY:
      temp.b=1;
      break;
    case CGAL::ON_UNBOUNDED_SIDE:
      break;
  }

  return temp;
}
double pick(double a,double b){
std::cout<< a << b <<std::endl;
  return a+ (b/2) -1;
}

int pick(Polygon_2 poly){
  double a=0;
  double b=0;
    points add;

int j=0;
  Point points[poly.size()] ;
for (auto vi = poly.vertices_begin(); vi != poly.vertices_end(); ++vi,++j)
    {
     points[j]=vi[0];
     }
int xmin=CGAL::to_double(poly.left_vertex()[0][0]);
int xmax=CGAL::to_double(poly.right_vertex()[0][0]);
int ymin=CGAL::to_double(poly.bottom_vertex()[0][1]);
int ymax=CGAL::to_double(poly.top_vertex()[0][1]);
  for(int i=xmin;i<=xmax;++i){
 for(int k=ymin;k<=ymax;++k)
  {add=check_inside(Point(i,k), points, points+poly.size(), K());
   a+=add.a;
   b+=add.b;
  // std::cout<<i<< " "<<k<<std::endl;
  }


   double temp=pick(a,b);
   return temp;
}

}
int temp()
{
  Point p(0,1);
  Point q(1,2);
  Point q1(3,3);
  Point q2(4,1);
  Point q3(4,-1);
  Point q4(7,-3);
  Point q5(3,-11);
  Point q6(2,10);
  Point q7(10,10);
  Point q8(8.315,1.901);
  Point q9(-4,2);
  Point q10(6,-6);
  Point q11(-6,-5);
  Point q12(-11,7);
  Point q13(12,6);
  Point q14(-10,-1);
  //Polygon_2 hull;
  Polygon_2 poly;
  poly.push_back(p);
  poly.push_back(q1); 
  poly.push_back(q2);     



  points add;
  double a=0;
  double b=0;
  //CGAL::convex_hull_2( poly.begin(), poly.end() ,std::back_inserter(hull));
 std::cout<< "From the cgal function "<<poly.area()<<std::endl;


 int j=0;
  Point points[poly.size()] ;
for (auto vi = poly.vertices_begin(); vi != poly.vertices_end(); ++vi,++j)
    {
     points[j]=vi[0];
     }
  // check if the polygon is simple.

int xmin=CGAL::to_double(poly.left_vertex()[0][0]);
int xmax=CGAL::to_double(poly.right_vertex()[0][0]);
int ymin=CGAL::to_double(poly.bottom_vertex()[0][1]);
int ymax=CGAL::to_double(poly.top_vertex()[0][1]);
for(int i=xmin;i<=xmax;++i){
 for(int k=ymin;k<=ymax;++k)
  {add=check_inside(Point(i,k), points, points+poly.size(), K());
   a+=add.a;
   b+=add.b;
  // std::cout<<i<< " "<<k<<std::endl;
  }
}

  double temp=pick(a,b);
   std::cout<<"Using Picks theorem " <<temp<<std::endl; 
   std::ofstream os("test.wkt");
   CGAL::IO::write_polygon_WKT(os,poly);

  return 0;
}
