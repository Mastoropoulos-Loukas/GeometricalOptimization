#include"incr.h"
#include <climits>
IncAlgo::IncAlgo(PointList& list, ArgFlags argFlags) : PolygonGenerator(list){this->argFlags = argFlags;};

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2                                          Point;
int inter(CGAL::Segment_2<Kernel>, CGAL::Segment_2<Kernel> , CGAL::Point_2<Kernel> );
std::vector<Point> SortPoints(std::string ,std::vector<Point>);

struct {
      bool operator()(Point a, Point b) const { return a.y() < b.y(); }
    } customLess;

struct {
      bool operator()(Point a, Point b) const { return a.y() > b.y(); }
    } customMore;
struct PurpleEdges{

  Point x;
  Point y;

};

struct ListofSegments{

   int pos;
   Segment_2 seg;  

};
std::vector<ListofSegments> ChecPol(Polygon_2 ,Point ,int ,PurpleEdges );
PurpleEdges CheckHull(Polygon_2 ,Point ,int );


int Edgeselection(Polygon_2 poly,Point p,std::vector<ListofSegments> segs,int mode){
int i=0;
int pos=0;
Polygon_2 triangle;
Segment_2 seg;
srand ( time(NULL) );
int temp;
if(segs.size()>2)
temp=rand()%segs.size();
else
temp=0;
if(mode==0)
for(auto v2=poly.edges_begin();v2!=poly.edges_end();++v2,++i){

  seg=segs[temp].seg;


   if (seg==v2[0])
  { pos=i+1;
  return pos;
  }


}
if(mode==2){
int max=-1;
for(auto v2=segs.begin();v2!=segs.end();++v2){

triangle.push_back(p);
triangle.push_back(v2[0].seg[0]);
triangle.push_back(v2[0].seg[1]);
if(max<=abs(triangle.area())){
max=abs(triangle.area());
seg=v2[0].seg;
}
}
for(auto v2=poly.edges_begin();v2!=poly.edges_end();++v2,++i){
   if (seg==v2[0])
  { pos=i+1;
  return pos;
  }


}

}

if(mode==1){
int min=INT_MAX;
for(auto v2=segs.begin();v2!=segs.end();++v2){

triangle.push_back(p);
triangle.push_back(v2[0].seg[0]);
triangle.push_back(v2[0].seg[1]);
if(min>=abs(triangle.area())){
min=abs(triangle.area());
seg=v2[0].seg;
}
}

for(auto v2=poly.edges_begin();v2!=poly.edges_end();++v2,++i){
   if (seg==v2[0])
  { pos=i+1;
  return pos;
  }


}


}


}
Polygon_2 IncAlgo::generatePolygon(){

  std::vector <Point> vec;
std::ostream_iterator< Point>  out( std::cout, "\n" );
  std::ofstream os("test.wkt");
  std::ofstream os1("test1.wkt");
  std::ofstream os2("test12.wkt");
  PurpleEdges edges;
  std::string mode;
  if(argFlags.initialization==0)
  mode="1a";
  else if(argFlags.initialization==1)
  mode="2a";
else   if(argFlags.initialization==2)
  mode="1b";
else   if(argFlags.initialization==3)
  mode="2b";
  vec=SortPoints(mode,list);

  Polygon_2 poly;
  Polygon_2 hull;

  poly.push_back(vec[0]);
  poly.push_back(vec[1]);
  poly.push_back(vec[2]);
  CGAL::IO::write_polygon_WKT(os2,poly);


 if(!poly.is_simple()){
  
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

  }
  int i=0;
  int j=0;
  int count=0;
  int select=0;
  std::vector<ListofSegments> points;
   int pos=1;
  for(auto v1=vec.begin()+3;v1!=vec.end();++v1,++i){

CGAL::convex_hull_2( poly.begin(), poly.end() ,std::back_inserter(hull));

edges=CheckHull(hull,v1[0],pos);

points=ChecPol(poly,v1[0],pos,edges);

i=0;

    j++;
 for(auto v2=poly.edges_begin();v2!=poly.edges_end();++v2,++i){
   if (points[0].seg==v2[0])
  { pos=i+1;
  break;
  }


  }
poly.insert(poly.vertices_begin()+Edgeselection(poly,v1[0],points,argFlags.edgeSelection),v1[0]);
//else if(select==1)

//else if(select==2)
hull.clear();
points.clear();

pos=pos-1;
  if(!poly.is_simple()){
  
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

  }

  }
      CGAL::IO::write_polygon_WKT(os2,poly);

  return poly;



}
//intersection between a segment of a polygon and the line 
int inter(CGAL::Segment_2<Kernel> seg, CGAL::Segment_2<Kernel> line, CGAL::Point_2<Kernel> p)
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
count=0;   

for (auto v2 = hull.edges_end()-1; v2 != hull.edges_begin(); --v2){
count+=inter(v2[0],lright,p);
count+=inter(v2[0],lleft,p);
count+=inter(v2[0],midpoint,Point(x,y));
if(count>1){

  res.y=vi[0][0];
break;

}
}
if(count>1){
break;

}

}

for (auto vi = hull.edges_begin()+pos; vi != hull.edges_end(); ++vi){

lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
midpoint={p,Point(x,y)};
count=0;

for (auto v2 = hull.edges_end()-1; v2 != hull.edges_begin(); --v2){

count+=inter(v2[0],lright,p);
count+=inter(v2[0],lleft,p);
count+=inter(v2[0],midpoint,Point(x,y));
if(count>1){
  res.x=vi[0][0];
break;

}
}
if(count>1){
break;

}
}

return res;
}


std::vector<ListofSegments> ChecPol(Polygon_2 poly,Point p,int pos,PurpleEdges edges){


Segment_2 lright;
Segment_2 midpoint;
Segment_2 lleft;
std::vector<ListofSegments> res;
double x;
double y;
int count=0;
ListofSegments temp;
int i=0;
int j=0;
for (auto vi = poly.edges_begin()+pos; vi != poly.edges_begin(); --vi,++i){


lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
    midpoint={p,Point(x,y)};
count=0;

for (auto v2 = poly.edges_end()-1; v2 != poly.edges_begin(); --v2){
count+=inter(v2[0],lright,vi[0][0]);
count+=inter(v2[0],lleft,vi[0][1]);

count+=inter(v2[0],midpoint,Point(x,y));

}
if(count==0){
 temp.pos=i+pos+1;
 temp.seg=vi[0];
 res.push_back(temp);

}
if(vi[0][0]==edges.x)
break;
}

for (auto vi = poly.edges_begin()+pos+1; vi != poly.edges_end(); ++vi,++j){


lright={p,vi[0][0]};
lleft={p,vi[0][1]};
x=(vi[0][0][0]+vi[0][1][0])/2;
y=(vi[0][1][1]+vi[0][0][1])/2;
midpoint={p,Point(x,y)};
count=0;

for (auto v2 = poly.edges_end()-1; v2 != poly.edges_begin(); --v2){

count+=inter(v2[0],lright,vi[0][0]);

count+=inter(v2[0],lleft,vi[0][1]);

count+=inter(v2[0],midpoint,Point(x,y));

}
if(count==0){

 temp.pos=pos+j-1;
 temp.seg=vi[0];
 res.push_back(temp);

}
if(vi[0][0]==edges.y)
break;
}

return res;
}

/*

int ing ()
{
  Polygon_2 v;
  Polygon_2 hull;

  Point p(0,1);
  Point q(1,2);
  Point q1(3,3);
  Point q2(4,1);
  Point q3(4,-1);
  Point q4(7,-3);
  Point q5(-2,-11);
  Point q7(10,10);
  Point q9(-4,2);
  Point q10(6,-6);
  Point q11(-6,-5);
  Point q12(-11,7);
  Point q13(12,6);
  Point q14(-10,-1);
  Point t;
  
  std::vector <Point> lis;
  std::vector <Point> vec;




 lis.push_back(q5); 

 lis.push_back(q10); 
 lis.push_back(q7); 

 lis.push_back(q11); 
 lis.push_back(q14); 
 lis.push_back(q2); 
 lis.push_back(q13); 

lis.push_back(q12); 
lis.push_back(q3); 
lis.push_back(q4); 



  std::ostream_iterator< Point>  out( std::cout, "\n" );
  std::ofstream os("test.wkt");
  std::ofstream os1("test1.wkt");
  std::ofstream os2("test12.wkt");
  PurpleEdges edges;


  vec=SortPoints("1a",lis);
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

  Polygon_2 poly;
  poly.push_back(vec[0]);
  poly.push_back(vec[1]);
  poly.push_back(vec[2]);
      std::cout<<"Not simple anymore error occured!!!"<<std::endl;

    CGAL::IO::write_polygon_WKT(os2,poly);
 std::cout<<Pick(poly)<<std::endl;
 std::cout<<(poly.area())<<std::endl;
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

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
  for(auto v1=vec.begin()+3;v1!=vec.end();++v1,++i){

CGAL::convex_hull_2( poly.begin(), poly.end() ,std::back_inserter(hull));


edges=CheckHull(hull,v1[0],pos);
points=ChecPol(poly,v1[0],pos,edges);

//pos=points[0].pos;
i=0;

 for(auto v2=poly.edges_begin();v2!=poly.edges_end();++v2,++i){

   if (points[0].seg==v2[0])
  { pos=i+1;
  break;
  }


  }

poly.insert(poly.vertices_begin()+pos,v1[0]);
 


hull.clear();
std::cout<<v1[0]<< " point is getting added with the line " <<points[0].seg<<" and pos " <<pos<<std::endl;
pos=pos-1;
  if(!poly.is_simple()){
  
    std::cout<<"Not simple anymore error occured!!!"<<std::endl;

  }

  
  }
  CGAL::IO::write_polygon_WKT(os1,hull);
  CGAL::IO::write_polygon_WKT(os,poly);
  
  return 0;
}
*/