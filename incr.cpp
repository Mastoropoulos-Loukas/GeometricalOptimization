#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/IO/WKT.h>
#include <boost/optional/optional_io.hpp>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>
#include <list>
#include <cassert>
 #include <CGAL/Boolean_set_operations_2.h> 
 #include <CGAL/Line_2.h>
 #include <CGAL/Circular_kernel_intersections.h> 
 #include <string>
 #include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include "PolygonGenerator.h"
#include "shared.h"
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2                                          Point;
typedef Kernel::Line_2                                          Line_2;

typedef CGAL::Polygon_2<Kernel>                                  Polygon_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;


//intersection between a segment of a polygon and the line 
int foo(CGAL::Segment_2<Kernel> seg, CGAL::Segment_2<Kernel> line, CGAL::Point_2<Kernel> p)
{
  int count=0;
  Point point;
  CGAL::Object result = CGAL::intersection(seg, line);
  if(seg[0]==p || seg[1]==p)
    return 0;
    
  if (const CGAL::Point_2<Kernel> *ipoint = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result)) {
    
    if(*ipoint!=p)
      count=1;

  }

  return count;
}


struct {
      bool operator()(Point a, Point b) const { return a.y() < b.y(); }
    } customLess;

struct {
      bool operator()(Point a, Point b) const { return a.y() > b.y(); }
    } customMore;



std::vector<Point> SortPoints(std::string mode,std::vector<Point> v){

if(mode=="2a")
  std::sort(v.begin(),v.end());
else if(mode=="1a")
  std::sort(v.begin(), v.end(), std::greater<Point>());
else if(mode=="2b")
  std::sort(v.begin(), v.end(), customLess);
else if (mode =="1b")
  std::sort(v.begin(), v.end(), customMore);

return v;
}

struct PurpleEdges{

  Point x;
  Point y;

};

struct ListofSegments{

   int pos;
   Segment_2 seg;  

};
PurpleEdges CheckHull(Polygon_2 hull,Point p,int pos){


Segment_2 lright;
Segment_2 midpoint;
Segment_2 lleft;
PurpleEdges res;
int x;
int y;
int count=0;
for (auto vi = hull.edges_begin()+pos+1; vi != hull.edges_begin(); --vi){
lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
    midpoint={p,Point(x,y)};
for (auto v2 = hull.edges_end()-1; v2 != hull.edges_begin(); --v2){
count=0;   
count+=foo(v2[0],lright,p);
count+=foo(v2[0],lleft,p);
count+=foo(v2[0],midpoint,Point(x,y));
if(count>1){

  res.y=vi[0][0];
break;

}
}

}

for (auto vi = hull.edges_begin()+pos; vi != hull.edges_end(); ++vi){

lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
midpoint={p,Point(x,y)};

for (auto v2 = hull.edges_end()-1; v2 != hull.edges_begin(); --v2){
count=0;

count+=foo(v2[0],lright,p);
count+=foo(v2[0],lleft,p);
count+=foo(v2[0],midpoint,Point(x,y));
if(count>1){

  res.x=vi[0][0];
break;

}
}

}
std::cout<<res.x << " "<<res.y <<std::endl;
return res;
}


std::vector<ListofSegments> ChecPol(Polygon_2 poly,Point p,int pos,PurpleEdges edges){


Segment_2 lright;
Segment_2 midpoint;
Segment_2 lleft;
std::vector<ListofSegments> res;
int x;
int y;
int count=0;
ListofSegments temp;
int i=0;
int j=0;
for (auto vi = poly.edges_begin()+pos+1; vi != poly.edges_begin(); --vi,++i){

if(vi[0][0]==edges.y)
break;
lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
    midpoint={p,Point(x,y)};
for (auto v2 = poly.edges_end()-1; v2 != poly.edges_begin(); --v2){
count=0;   
count+=foo(v2[0],lright,p);
count+=foo(v2[0],lleft,p);
count+=foo(v2[0],midpoint,Point(x,y));

}
if(count>0){
 temp.pos=i;
 temp.seg=vi[0];
 res.push_back(temp);

}
}

for (auto vi = poly.edges_begin()+pos; vi != poly.edges_end(); ++vi,++j){
if(vi[0][0]==edges.x)
break;
lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
midpoint={p,Point(x,y)};

for (auto v2 = poly.edges_end()-1; v2 != poly.edges_begin(); --v2){
count=0;

count+=foo(v2[0],lright,p);
count+=foo(v2[0],lleft,p);
count+=foo(v2[0],midpoint,Point(x,y));

}
if(count>0){
 temp.pos=j;
 temp.seg=vi[0];
 res.push_back(temp);

}
}
return res;
}
PurpleEdges CheckTheHull(Polygon_2 poly,Point p,int count,Segment_2 l1){

  int j=0;
  PurpleEdges res;
  //res.x=p;
  //res.y=p;
  Segment_2 midpoint;
  Segment_2 temp;
  int count1;
  for (auto ver = poly.edges_begin(); ver != poly.edges_end(); ++ver){
    l1={p,ver[0][0]};
    temp={p,ver[0][1]};
    count=0;
    count1=0;
    int x=(ver[0][0][0]+ver[0][1][0])/2;
    int y=(ver[0][1][1]+ver[0][0][1])/2;
    midpoint={p,Point(x,y)};
    count+=foo(poly.edges_begin()[0],l1,poly.edges_begin()[0][0]);
    count+=foo(poly.edges_begin()[0],midpoint,Point(x,y));
    count+=foo(poly.edges_begin()[0],temp,poly.edges_begin()[0][1]);

    if(count==1){
      res.x=ver[0][1];

    }

    count=0;
    for (auto vi = poly.edges_end()-1; vi != poly.edges_begin(); --vi,++j)
      { 
      count=0;
      // buffer[j] = vi[0];
      count+=foo(vi[0],l1,ver[0][0]);
      count+=foo(vi[0],midpoint,Point(x,y));
      count+=foo(vi[0],temp,ver[0][1]);
      
      if(count==1){
        break;
      }
    }

    if(count==1){
    
      res.x=ver[0][1];

    }
    for (auto vi = poly.edges_begin(); vi != poly.edges_end(); ++vi,++j)
      {
        
      count=0;
      count+=foo(vi[0],l1,ver[0][0]);
      count+=foo(vi[0],midpoint,Point(x,y));
      count+=foo(vi[0],temp,ver[0][1]);
    if(count==1){
        break;
      }
    }
    if(count==1){
      res.y=ver[0][0];
    }
  }

  return res;

}

std::vector<ListofSegments> CheckThePolygon(Polygon_2 poly,Point p,int count,Segment_2 l1,Point right,Point left){
  int j=0;
  int k=0;
  Segment_2 midpoint;
  Segment_2 temp;
  ListofSegments Temp1;
  Temp1.pos=0;
  Temp1.seg=l1;
  int flag=0;
  std::vector<ListofSegments> list;
  bool flagRight=false;
  bool flagLeft=false;
  for (auto ver = poly.edges_begin(); ver != poly.edges_end(); ++ver,++j){

    std::cout<<ver[0]<<std::endl;
      l1={p,ver[0][0]};
      temp={p,ver[0][1]};
      count=0;
      int x=(ver[0][0][0]+ver[0][1][0])/2;
      int y=(ver[0][1][1]+ver[0][0][1])/2;
      midpoint={p,Point(x,y)};
       

     // if(count==0)
      //    std::cout<<(poly.edges_end()-1)[0]<<" is a visible segment from the right"<<std::endl;



      

      if(flagRight==false)
        for (auto vi = poly.edges_end()-1; vi != poly.edges_begin(); --vi)
        { 
        // buffer[j] = vi[0];
          count+=foo(vi[0],l1,ver[0][0]);
          count+=foo(vi[0],midpoint,Point(x,y));
          count+=foo(vi[0],temp,ver[0][1]);
         if(count==1)
          break;
        }

     
 

  if(flagLeft==false)
    for (auto vi = poly.edges_begin(); vi != poly.edges_end(); ++vi)
        {

          count+=foo(vi[0],l1,ver[0][0]);
          count+=foo(vi[0],midpoint,Point(x,y));
          count+=foo(vi[0],temp,ver[0][1]);
      if(count==1)
          break;
        }

 
    if(count==0){
      
 
      Temp1.pos=j;
      Temp1.seg=ver[0];
      std::cout<<"a"<<std::endl;
      list.push_back(Temp1);
    }

if(ver[1][0]==left)
        flagLeft=true;
      if(ver[1][1]==right)
        flagRight=true;
    if(flagLeft&&flagRight==true){
    
    return list;
    break;

    }


}


}



/*
int main ()
{
  Polygon_2 v;
  Polygon_2 hull;

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
  Point t;
  
  std::vector <Point> lis;
  std::vector <Point> vec;

  lis.push_back(p);
  lis.push_back(q);
  lis.push_back(q1);
  lis.push_back(q2);
   lis.push_back(q3);
  lis.push_back(q4);
  lis.push_back(q5);
  lis.push_back(q6);
  lis.push_back(q7);

 


  std::ostream_iterator< Point>  out( std::cout, "\n" );
  std::ofstream os("test.wkt");
  std::ofstream os1("test1.wkt");
  std::ofstream os2("test12.wkt");
  PurpleEdges edges;


  vec=SortPoints("1a",lis);

  Polygon_2 poly;
  poly.push_back(vec[0]);
  poly.push_back(vec[1]);
  poly.push_back(vec[2]);
 if(!poly.is_simple()){
  
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

  }
  int i=0;
  int j=0;
  //Segment_2 buffer[sizeof(v)/8+4];
  int count=0;
  Polygon_2 temp;
  Segment_2 l1;
  std::vector<ListofSegments> points;
   int pos=1;
  for(auto v1=vec.begin()+4;v1!=vec.end();++v1,++i){

  CGAL::convex_hull_2( poly.begin(), poly.end() ,std::back_inserter(hull));
  //edges=CheckTheHull(hull,v1[0],count,l1);
 // std::cout<<"For "<<v1[0]<< " " << edges.x<< " "<<edges.y<<std::endl;
  //points=CheckThePolygon(poly,v1[0],count,l1,edges.x,edges.y);
  //showlist(points);
edges=CheckHull(hull,v1[0],pos);
points=ChecPol(poly,v1[0],pos,edges);
pos=points[0].pos;
poly.insert(poly.vertices_begin()+points[0].pos,v1[0]);
hull.clear();
//std::list<ListofSegments>::iterator it = li.begin();
//std::cout<<v1[0]<<points[0].pos<<std::endl;
//poly.insert(poly.vertices_begin()+points[0].pos+1,v1[0]);

  if(!poly.is_simple()){
  
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

  }

  
  }
  CGAL::IO::write_polygon_WKT(os1,hull);
//check edges_end()-1,which is the seg to the right of the last point or change the second loop from -2 to -1
  


  CGAL::IO::write_polygon_WKT(os,poly);


  
  return 0;
}

*/