#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/IO/WKT.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Simple_cartesian.h>
#include <vector>
#include <numeric>
#include <cstdlib>


#define ENDL std::endl 
#define COUT std::cout

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

typedef K::Line_2 Line_2;
typedef K::Intersect_2 Intersect_2;


typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Segment_2    Segment_2;

typedef CGAL::Polygon_set_2<K, std::vector<K::Point_2>> Polygon_set_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;

typedef CGAL::Convex_hull_traits_adapter_2<K,
          CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;
int onion(){

  std::vector<Point_2> points = { Point_2(498,5392), //0
                                  Point_2(1192,226),  //1
                                  Point_2(3906,624),  //2
                                  Point_2(3460,48),  //3
                                  Point_2(4090,4492),  //4
                                  Point_2(4238,94), // 5
                                  Point_2(4292,4606),  //6
                                  Point_2(4488,3376),  //7
                                  Point_2(4512,4632),  //8
                                  Point_2(4660,2216),  //9
                                  Point_2(4822,2972),  //10
                                  Point_2(6056,3838),  //11
                                  Point_2(6290,2626),  //12
                                  Point_2(6350,4018), //13
                                  Point_2(6496,5406), //14 
                                  Point_2(6696,4980), //15
                                  Point_2(6728,4454), //16
                                  Point_2(6918,452), //17
                                  Point_2(7070,2192), //18
                                  Point_2(7126,1594), //19
                                  Point_2(7886,5418), //20
                                  Point_2(8070,3972), //21
                                  Point_2(8116,3658), // 22
                                  Point_2(8164,3236), //23
                                  Point_2(8470,3100), //24
                                  Point_2(9678,3574), //25
                                  Point_2(10028,560), //26
                                  Point_2(10858,3852), //27
                                  Point_2(11424,682), // 28
                                  Point_2(12044,5748), //29
                                  }; 

  
  // std::vector<Point_2> points = { Point_2(2102,5260),
  //                                 Point_2(2420,354),
  //                                 Point_2(3906,4366),
  //                                 Point_2(5184,2922),
  //                                 Point_2(5346,4176),
  //                                 Point_2(5550,4142),
  //                                 Point_2(5796,2822),
  //                                 Point_2(6384,2302),
  //                                 Point_2(7084,2732),
  //                                 Point_2(7426,394),
  //                                 Point_2(7672,6734),
  //                                 Point_2(7904,920),
  //                                 Point_2(8124,4912),
  //                                 Point_2(8396,3542),
  //                                 Point_2(8846,1814),
  //                                 Point_2(9216,2630),
  //                                 Point_2(9792,4640),
  //                                 Point_2(10188,4142),
  //                                 Point_2(11108,3318),
  //                                 Point_2(12110,496) };

  std::vector<Polygon_2> allPolys;

  std::vector<std::vector<Segment_2>> vecAllPolys;

  std::ofstream os("yelp.wkt");

  for(int i=0;i<points.size();i++){
    CGAL::IO::write_point_WKT(os,points[i]);
  }

  COUT<<ENDL;
  
  while(points.size()>2){
    if(points.size()==3 && CGAL::collinear(points[0],points[1],points[2])){
      std::vector<Point_2>::iterator it;
      points.erase(it+1); //Check for potential trouble
    }else{
      std::vector<std::size_t> indices(points.size()), out;
      std::iota(indices.begin(), indices.end(),0);
      CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out),
                        Convex_hull_traits_2(CGAL::make_property_map(points)));
    

      Polygon_2 poly;
      std::vector<Point_2> pointsLeftAct;

    // loop that indicates the convexHull points
      for( std::size_t i : out){

        poly.push_back(points[i]); // push the point to a polygon
        
        
        std::cout << "points[" << i << "] = " <<"("<<points[i]<<") " << std::endl;

        points[i]=Point_2(-99,-99);   // mark the convex hull points
  
    }
      allPolys.push_back(poly);

    // we find the points that do not belong to the convexHull
      for(int i =0; i<points.size();i++){
        if(points[i]!=Point_2(-99,-99)){ // if a point is left unmarked this means that it does not belong to the convexHull
          pointsLeftAct.push_back(points[i]); // push it to the pointsLeftAct
        }
      }

      COUT<<ENDL;
    
      points=pointsLeftAct;
    }

  
  }
  
  std::cout<<"FINAL Points LEFT"<<ENDL;
  for(int i =0; i<points.size();i++){
    COUT<<"("<<points[i]<<")"<<" ";
  }
  COUT<<ENDL;

  COUT<<ENDL<<"PRINTING ALLPOLYS"<<ENDL;
  for(int i =0; i<allPolys.size();i++){

    // std::vector<Segment_2> segVec;
    // for(const Segment_2& e  : allPolys[i].edges()){
    //   segVec.push_back(e);
    // }
    // vecAllPolys.push_back(segVec);

    COUT<<ENDL;
  }

  // std::cout<<"ALL POLYGONS AS VECTOR"<<ENDL;
  for(int i =0; i<vecAllPolys.size();i++){
    for(int j=0;j<vecAllPolys[i].size();j++){
      COUT<<"("<<vecAllPolys[i][j][0]<<")"<<" ";
    }
    
    COUT<<ENDL;
  }


  COUT<<ENDL;
  // for(const Point_2& p : allPolys[0].vertices()){
  //   std::cout << "(" << p << ") ";
  // }
  COUT<<ENDL;

  for(int i=0;i<allPolys.size();i++){
      CGAL::IO::write_polygon_WKT(os,allPolys[i]);
  }




  // int m=allPolys[0].size()/2;

  // int m=0;

  // while(allPolys[0].vertex(m)!= *allPolys[0].top_vertex()){
  //   m++;
  // }
  int m=allPolys[0].size()/2;

  COUT<<"m is "<<m<<ENDL;

  int mPlus;
  int mMinus;

  if(m!=allPolys[0].size()-1){
    mPlus=m+1;
  }else{
    mPlus=0;
  }


  if(m!=0){
    mMinus=m-1;
  }else{
    mMinus=allPolys[0].size()-1;
  }
  
  COUT<<m<<ENDL;

  Polygon_2 finalPoly=allPolys[0];


  for(int i =0;i<allPolys.size();i++){

    COUT<<"DEPTH IS "<<i<<ENDL;

    Point_2 mVertex;
    Point_2 mVertexPlus;

    if(finalPoly.is_empty()){
      mVertex = allPolys[i].vertex(m);
      mVertexPlus=allPolys[i].vertex(mPlus);
    }else{
      mVertex = finalPoly.vertex(m);
      mVertexPlus=finalPoly.vertex(mPlus);
    }
  
    COUT<<"m is "<<m<<ENDL;
    COUT<<"Mplus is "<<mPlus<<ENDL;
    COUT<<"Vertex i(m) is "<<" "<<mVertex<<ENDL;
    COUT<<"Vertex i(m+1) is "<<" "<<mVertexPlus<<ENDL;

    if(i+1!=allPolys.size()){
      Point_2 closestK=Point_2(-1111,-1111);
      int indexClosestK=-1;
      double dist=INFINITY;


      for(int j=0;j<allPolys[i+1].size();j++){
        Point_2 vert=allPolys[i+1].vertex(j);
        double vertDist=sqrt(((vert[0]-mVertex[0])*(vert[0]-mVertex[0]))+((vert[1]-mVertex[1])*(vert[1]-mVertex[1])));
            
        if(vertDist<dist){
          dist=vertDist;
          closestK=vert;
          indexClosestK=j;
        }
      }

      int indexLamda;
      
      if(indexClosestK!=allPolys[i+1].size()-1){
        indexLamda=indexClosestK+1;
      }else{
        indexLamda=0;
      }

      Point_2 lamda=allPolys[i+1].vertex(indexLamda);


      COUT<<"CLOSEST POINT K TO m IS "<<closestK<< " with index " << indexClosestK<<ENDL;
      COUT<<"LAMDA IS "<< lamda << " with index " << indexClosestK+1<<ENDL;
          
      

      if(finalPoly.is_empty()){
        
        auto veit=finalPoly.vertices_begin();
        
        while(*veit!=mVertexPlus){
          if(veit+1!=allPolys[i].vertices_end()){
            veit++;
          }else{
            veit=allPolys[i].vertices_begin();
          }
        }
        
        std::vector<Point_2> toBeAdded;

        for(int ind=indexClosestK;ind>=0;ind--){
          toBeAdded.push_back(allPolys[i+1].vertex(ind));
        }

        for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
          toBeAdded.push_back(allPolys[i+1].vertex(ind));
        }

        finalPoly.insert(veit,toBeAdded.begin(),toBeAdded.end());

        if(mPlus<m && (mPlus) ){
          finalPoly.push_back(allPolys[i].vertex(mPlus));

          for(int ind=indexLamda;ind<allPolys[i+1].size();ind++){
            finalPoly.push_back(allPolys[i+1].vertex(ind));
          }

          for(int ind=0;ind<=indexClosestK; ind++){
            finalPoly.push_back(allPolys[i+1].vertex(ind)); 
          }

          for(int ind=m;ind<allPolys[i].size();ind++){
            finalPoly.push_back(allPolys[i].vertex(ind)); 
          }

          for(int ind=0;ind<mPlus;ind++){
            finalPoly.push_back(allPolys[i].vertex(ind));
          }

        }else if(mPlus==0 && m==allPolys[i].size()-1){
          
          for(int ind=mPlus;ind<allPolys[i].size()-1;ind++){
            finalPoly.push_back(allPolys[i].vertex(ind));
          } 

          for(int ind=0;ind>=m;ind++){
            finalPoly.push_back(allPolys[i].vertex(ind));
          }
          for(int ind=indexClosestK;ind>=0; ind--){
            finalPoly.push_back(allPolys[i+1].vertex(ind)); 
          }
          for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
            finalPoly.push_back(allPolys[i+1].vertex(ind)); 
          }

          for(int ind=0;ind<m;ind++){
            finalPoly.push_back(allPolys[i].vertex(ind)); 
          }

          for(int ind=allPolys[i].size()-1;ind>mPlus;ind--){
            finalPoly.push_back(allPolys[i].vertex(ind));
          }

        }else{
          // finalPoly.push_back(allPolys[i].vertex(m)); // ksekiname ta pushback apo to m 


          // for(int ind=indexClosestK;ind>=0;ind--){
          //   finalPoly.push_back(allPolys[i+1].vertex(ind));    
          // }

          // for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
          //   finalPoly.push_back(allPolys[i+1].vertex(ind)); 
          // }

          // for(int ind=mPlus;ind<allPolys[i].size();ind++){
          //   finalPoly.push_back(allPolys[i].vertex(ind)); 
          // }

          // for(int ind=0;ind<m;ind++){
          //   finalPoly.push_back(allPolys[i].vertex(ind));
          // }
        }





      }else{
        Polygon_2 newPoly;

        if(mPlus<m || (mPlus==0 && m==finalPoly.size()-1)){
          newPoly.push_back(finalPoly.vertex(mPlus));

          for(int ind=indexLamda;ind<allPolys[i+1].size();ind++){
            newPoly.push_back(allPolys[i+1].vertex(ind));
          }

          for(int ind=0;ind<=indexClosestK; ind++){
            newPoly.push_back(allPolys[i+1].vertex(ind)); 
          }

          for(int ind=m;ind<finalPoly.size();ind++){
            newPoly.push_back(finalPoly.vertex(ind)); 
          }

          for(int ind=0;ind<mPlus;ind++){
            newPoly.push_back(finalPoly.vertex(ind));
          }

          finalPoly=newPoly;
          
        }else{
              // Careful with that point.Still fuzzy
              
          newPoly.push_back(finalPoly.vertex(m));
                
          for(int ind=indexClosestK;ind>=0;ind--){
            newPoly.push_back(allPolys[i+1].vertex(ind));
          }

          for(int ind=allPolys[i+1].size()-1;ind>=indexLamda; ind--){
            newPoly.push_back(allPolys[i+1].vertex(ind)); 
          }

          for(int ind=mPlus;ind<finalPoly.size(); ind++){
            newPoly.push_back(finalPoly.vertex(ind)); 
          }              
              
          for(int ind=0;ind<m; ind++){
            newPoly.push_back(finalPoly.vertex(ind)); 
          } 
          finalPoly=newPoly;
        }

      }


      int initK=indexClosestK;
      int initLam=indexLamda;

      // if(indexLamda!=allPolys[i+1].size()-1){
      //   indexLamda++;
      // }else{
      //   indexLamda=0;
      // }
      // if(indexClosestK!=0){
      //   indexClosestK--;
      // }else{
      //   indexClosestK=0;
      // }

      do{
        if(indexLamda!=allPolys[i+1].size()-1){
          indexLamda++;
        }else{
            indexLamda=0;
        }

        if(indexClosestK!=0){
          indexClosestK--;
        }else{
          indexClosestK=allPolys[i+1].size()-1;
        }
        if((allPolys[i+1].size()) %2 && indexLamda==indexClosestK){
          // if(indexClosestK!=0){
          //   indexClosestK--;
          // }else{
          //   indexClosestK=allPolys[i+1].size()-1;
          // }

          if(indexLamda!=allPolys[i+1].size()-1){
            indexLamda++;
          }else{
            indexLamda=0;
          }
        }

      }while((indexLamda-indexClosestK!=1 && indexLamda-indexClosestK!=-1 && indexLamda-indexClosestK!=allPolys[i+1].size()-1 
        && indexLamda-indexClosestK!=-(allPolys[i+1].size()-1)));

      // while((indexLamda-indexClosestK!=1 && indexLamda-indexClosestK!=-1)){
      //   if(indexLamda!=allPolys[i+1].size()-1){
      //     indexLamda++;
      //   }else{
      //       indexLamda=0;
      //   }

      //   if(indexClosestK!=0){
      //     indexClosestK--;
      //   }else{
      //     indexClosestK=allPolys[i+1].size()-1;
      //   }
      // }

      if( indexLamda-indexClosestK==-1 ||indexLamda-indexClosestK==allPolys[i+1].size()-1 ){
        int temp=indexClosestK;
        indexClosestK=indexLamda;
        indexLamda=temp;
      }

      COUT<<"NEW INDEX K IS "<<indexClosestK<<ENDL;
      COUT<<"NEW INDEX L IS "<<indexLamda<<ENDL;



      int kInPoly=0;
      int lamInPoly=0;

      Point_2 findK=allPolys[i+1].vertex(indexClosestK);
      Point_2 findLam=allPolys[i+1].vertex(indexLamda);

      int help=0;
      while(finalPoly.vertex(help)!=findK){
        help++;
      }
      kInPoly=help;
      help=0;

      while(finalPoly.vertex(help)!=findLam){
        help++;
      }
      lamInPoly=help;

      COUT<<"K IN POLY IS IN POSITION "<<kInPoly<<ENDL;
      COUT<<"LAMDA IN POLY IS IN POSITION "<<lamInPoly<<ENDL;


      m=kInPoly;
      mPlus=lamInPoly;

    }else{

      if(finalPoly.is_empty()){
        finalPoly=allPolys[i];
      }

      for(int j=0;j<points.size();j++){
        Point_2 closePoint=Point_2(-1111,-1111);
        int indexClosePoint=-1;
        double dist=INFINITY;
      
        for(int k=0;k<allPolys[i].size();k++){
          Point_2 vert=allPolys[i].vertex(k);
          double vertDist=sqrt(((vert[0]-points[j][0])*(vert[0]-points[j][0]))+((vert[1]-points[j][1])*(vert[1]-points[j][1])));
            
          if(vertDist<dist){
            dist=vertDist;
            closePoint=vert;
            indexClosePoint=j;
          }        
        }
        COUT<<"CLOSEST POINT TO POINT_LEFT: "<<points[j]<<" IS "<< closePoint<<ENDL;

        auto veit=finalPoly.vertices_begin();
        while(*veit!=closePoint){
          if(veit+1!=finalPoly.vertices_end()){
            veit++;
          }else{
            veit=finalPoly.vertices_begin();
          }

        }
        finalPoly.insert(veit+1,points[j]);
      }
    
    }
      // if(i==0){
      //   CGAL::IO::write_polygon_WKT(os,finalPoly);
      // }

  }

  // CGAL::IO::write_polygon_WKT(os,finalPoly);

  int area=finalPoly.area();

  COUT<<"AREA IS "<<area<<ENDL;

  return 0;

}