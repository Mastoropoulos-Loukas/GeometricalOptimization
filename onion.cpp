
#include "onion.h"
#include <time.h>

int nextIndex(int,Polygon_2);
int previousIndex(int,Polygon_2);

// Constructor
OnionAlgo::OnionAlgo(PointList& list, int option) : PolygonGenerator(list){this->option=option;};

// Function that actually find the polygon
Polygon_2 OnionAlgo::generatePolygon(){

  srand(time(0));

  std::vector<Point_2> points=list;
  std::vector<Polygon_2> allPolys;

  std::vector<std::vector<Segment_2>> vecAllPolys;

  std::ofstream os("yelp.wkt");

  for(int i=0;i<points.size();i++){
    CGAL::IO::write_point_WKT(os,points[i]);
  }

  int initPointsSize=points.size();
  
  while(points.size()>2){
    if(points.size()==3 && CGAL::collinear(points[0],points[1],points[2])){
      std::vector<Point_2>::iterator it = points.begin();
      points.erase(it+1); //Check for potential trouble
      initPointsSize;
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

  // COUT<<ENDL<<"PRINTING ALLPOLYS"<<ENDL;
  for(int i =0; i<allPolys.size();i++){

    // std::vector<Segment_2> segVec;
    // // for(const Segment_2& e  : allPolys[i].edges()){
    // //   segVec.push_back(e);
    // // }
    // vecAllPolys.push_back(segVec);

    COUT<<"Polygon at depth: "<< i <<" " << allPolys[i]<<ENDL;
  }

  // std::cout<<"ALL POLYGONS AS VECTOR"<<ENDL;
  // for(int i =0; i<vecAllPolys.size();i++){
  //   for(int j=0;j<vecAllPolys[i].size();j++){
  //     COUT<<"("<<vecAllPolys[i][j][0]<<")"<<" ";
  //   }
    
  //   COUT<<ENDL;
  // }


  COUT<<ENDL;
  // for(const Point_2& p : allPolys[0].vertices()){
  //   std::cout << "(" << p << ") ";
  // }
  COUT<<ENDL;

  
  // Print out all the Convex Hulls
  // for(int i=0;i<allPolys.size();i++){
  //     CGAL::IO::write_polygon_WKT(os,allPolys[i]);
  // }



  int criterion=this->option;
  int m=0;

  if(criterion==1){
    m= rand()% allPolys[0].size();  
  }else if(criterion==2){
    while(allPolys[0].vertex(m)!= *allPolys[0].left_vertex()){
      m++;
    }  
  }else if(criterion==3){
    while(allPolys[0].vertex(m)!= *allPolys[0].right_vertex()){
      m++;
    }
  }else if(criterion==4){
    while(allPolys[0].vertex(m)!= *allPolys[0].bottom_vertex()){
      m++;
    }
  }else if(criterion==5){
    while(allPolys[0].vertex(m)!= *allPolys[0].top_vertex()){
      m++;
    }
  }else{
    m=allPolys[0].size()/2;
  }

  int initialM=m;
  COUT<<"m is "<<initialM<<ENDL;

  int mPlus=nextIndex(m,allPolys[0]);
  int mMinus=previousIndex(m,allPolys[0]);

  Polygon_2 finalPoly=allPolys[0];

  for(int i =0;i<allPolys.size();i++){

    COUT<<"DEPTH IS "<<i<<ENDL;

    Point_2 mVertex;
    Point_2 mVertexPlus;


    mVertex = finalPoly.vertex(m);
    mVertexPlus=finalPoly.vertex(mPlus);

  
    COUT<<"m is "<<m<<ENDL;
    COUT<<"Mplus is "<<mPlus<<ENDL;
    COUT<<"Vertex i(m) is "<<" "<<mVertex<<ENDL;
    COUT<<"Vertex i(m+1) is "<<" "<<mVertexPlus<<ENDL;

    if(i+1!=allPolys.size()){
      Point_2 closestK=Point_2(-1111,-1111);
      int indexClosestK=-1;
      double dist=INFINITY;

      // Find closest Point K
      for(int j=0;j<allPolys[i+1].size();j++){
        Point_2 vert=allPolys[i+1].vertex(j);
        double vertDist=sqrt(((vert[0]-mVertex[0])*(vert[0]-mVertex[0]))+((vert[1]-mVertex[1])*(vert[1]-mVertex[1])));
            
        if(vertDist<dist){
          dist=vertDist;
          closestK=vert;
          indexClosestK=j;
        }
      }

      int indexLamda=nextIndex(indexClosestK,allPolys[i+1]);
      
      Point_2 lamda=allPolys[i+1].vertex(indexLamda);

      Segment_2 edgeInPoly(mVertexPlus,lamda);

      COUT<<"CLOSEST POINT K TO m IS "<<closestK<< " with index " << indexClosestK<<ENDL;
      COUT<<"LAMDA IS "<< lamda << " with index " << indexLamda<<ENDL;
      
      if(CGAL::collinear(mVertex,allPolys[i+1].vertex(nextIndex(indexLamda,allPolys[i+1])),lamda)){
        COUT<<"COLLINEAR Lamda,LamdaPlus,mPlus"<<ENDL;
      }

      if(isVisible(edgeInPoly,allPolys[i+1])){
        COUT<<"Lamda "<< lamda<<" with index "<< indexLamda<<" is visible from mPlus "<<mVertexPlus<<" with index "<<mPlus<<ENDL;
      }else{
        COUT<<"Lamda "<< lamda<<" with index "<< indexLamda<<" is NOT visible from mPlus "<<mVertexPlus<<" with index "<<mPlus<<ENDL;        
        
        if((m<mPlus && m!=finalPoly.size()-1) ||(m>mPlus && m==finalPoly.size()-1) ){
          mPlus=previousIndex(m,finalPoly);

        }else{
          mPlus=nextIndex(m,finalPoly);     
        }
        
        indexLamda=previousIndex(indexClosestK,allPolys[i+1]);
        mVertexPlus=finalPoly.vertex(mPlus);
        lamda=allPolys[i+1].vertex(indexLamda);


        COUT<<"NEW Mplus is "<<mPlus<<ENDL;
        COUT<<"NEW Vertex i(m+1) is "<<" "<<mVertexPlus<<ENDL;

        COUT<<" NEW LAMDA IS "<< lamda << " with index " << indexLamda<<ENDL;    
      }

      Segment_2 mToClosestKEdge(mVertex,closestK);
      Segment_2 mPlusToLamdaEdge(mVertexPlus,lamda);

      if(CGAL::do_intersect(mToClosestKEdge,mPlusToLamdaEdge)){
        std::swap(m,mPlus);
        mVertex = finalPoly.vertex(m);
        mVertexPlus=finalPoly.vertex(mPlus);
        COUT<<"SWAPPING m with mPlus"<<ENDL;
        COUT<<"m is "<<m<<ENDL;
        COUT<<"Mplus is "<<mPlus<<ENDL;
        
      }

      if(finalPoly.size()==allPolys[0].size()){
        
        auto veit=finalPoly.vertices_begin();
        
        if(mPlus>m || mPlus==0){
          while(*veit!=mVertexPlus){
            if(veit+1!=finalPoly.vertices_end()){
              veit++;
            }else{
              veit=finalPoly.vertices_begin();
            }
          }
        }else{
          while(*veit!=mVertex){
            if(veit+1!=finalPoly.vertices_end()){
              veit++;
            }else{
              veit=finalPoly.vertices_begin();
            }
          }
        }
        
        std::vector<Point_2> toBeAdded;

        if(m<mPlus && mPlus!=0 && m!=0){
          
          if(indexClosestK==allPolys[i+1].size()-1){
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }  
          }else{
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
          }


        
        }else if(m<mPlus && mPlus!=0 && m==0){
          if(indexClosestK==allPolys[i+1].size()-1){
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }            
          }else{
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }            
          }

        }else if(m>mPlus && mPlus!=0 && m!=0){
          if(indexClosestK==0){
            for(int ind=indexLamda;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }            
          }
        }else if(m>mPlus && mPlus==0){
          COUT<<"GOT HERE MOFOS"<<ENDL;
          if(indexLamda==0){
            
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }
          }else{

            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }

            for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
              COUT<<"ind is "<<ind<<ENDL;
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }

          }
        }
        else{
          COUT<<"HELLO BITCHES!"<<ENDL;
          if(indexLamda<indexClosestK){
            for(int ind=indexLamda;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
            for(int ind=allPolys[i+1].size()-1;ind>=indexClosestK;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }            
          
          }else{
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
          }

        }

        if(mPlus==0){
          COUT<<"HERE WE GO..."<<ENDL;
          finalPoly.insert(veit,toBeAdded.begin(),toBeAdded.end());
        }else{
          finalPoly.insert(veit,toBeAdded.begin(),toBeAdded.end());
        }


      }else{
        Polygon_2 newPoly;

        auto veit=finalPoly.vertices_begin();

        std::vector<Point_2> toBeAdded;

        if(mPlus>m){
          while(*veit!=mVertexPlus){
            if(veit+1!=finalPoly.vertices_end()){
              veit++;
            }else{
              veit=finalPoly.vertices_begin();
            }
          }
        }else{
          while(*veit!=mVertex){
            if(veit+1!=finalPoly.vertices_end()){
              veit++;
            }else{
              veit=finalPoly.vertices_begin();
            }
          }
        }

        if(m<mPlus){
          if(indexClosestK>indexLamda && indexLamda!=0){
            for(int ind=indexClosestK;ind<allPolys[i+1].size();ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=0;ind<=indexLamda;ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            } 
          }else if(indexClosestK>indexLamda && indexLamda==0){
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
          }else if(indexClosestK<indexLamda && indexLamda==allPolys[i+1].size()-1 && indexClosestK==0){
            for(int ind=0;ind<=indexLamda;ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
          }else if(indexClosestK<indexLamda && indexLamda==allPolys[i+1].size()-1 && indexClosestK!=0){
            
            
            // for(int ind=indexLamda;ind>=0;ind--){ // if everything else fails
            //   toBeAdded.push_back(allPolys[i+1].vertex(ind));
            // }
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
            for(int ind=indexLamda;ind<allPolys[i+1].size();ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }             
          }else if(indexClosestK<indexLamda && indexLamda!=allPolys[i+1].size()-1){
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }           
          }else{          
            for(int ind=indexClosestK;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=allPolys[i+1].size()-1;ind>=indexLamda;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
          }

        }else{

          if(indexClosestK>indexLamda && indexLamda!=0){
            COUT<<"STUPID CONDITION 1"<<ENDL;
            for(int ind=indexClosestK;ind<allPolys[i+1].size();ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }

            for(int ind=0;ind<=indexLamda;ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            } 
          }else if(indexLamda==0){
                        COUT<<"STUPID CONDITION 2"<<ENDL;
            for(int ind=indexLamda;ind<allPolys[i+1].size();ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }

          }else if(indexClosestK<indexLamda && indexClosestK==0 && indexLamda==allPolys[i+1].size()-1){
                        COUT<<"STUPID CONDITION 3"<<ENDL;
            for(int ind=indexLamda;ind>=0;ind--){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }
          }else if(indexClosestK<indexLamda && indexClosestK==0 && indexLamda!=allPolys[i+1].size()-1){
                        COUT<<"STUPID CONDITION 4"<<ENDL;
            for(int ind=indexLamda;ind<allPolys[i+1].size();ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }
            for(int ind=0;ind<=indexClosestK;ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));              
            }                        
          }
          else{
                        COUT<<"STUPID CONDITION 5"<<ENDL;
            for(int ind=indexLamda;ind<allPolys[i+1].size();ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            }
            for(int ind=0;ind<=indexClosestK;ind++){
              toBeAdded.push_back(allPolys[i+1].vertex(ind));
            } 
          }  
        }
        
        
        finalPoly.insert(veit,toBeAdded.begin(),toBeAdded.end());

        // COUT<<finalPoly<<ENDL;
      }

      int initK=indexClosestK;
      int initLam=indexLamda;

      do{

        indexLamda=nextIndex(indexLamda,allPolys[i+1]);
        indexClosestK=previousIndex(indexClosestK,allPolys[i+1]);
 
        if((allPolys[i+1].size()) %2 && indexLamda==indexClosestK){
          indexLamda=nextIndex(indexLamda,allPolys[i+1]);
        }

      }while((indexLamda-indexClosestK!=1 && indexLamda-indexClosestK!=-1 && indexLamda-indexClosestK!=allPolys[i+1].size()-1 
        && indexLamda-indexClosestK!=-(allPolys[i+1].size()-1)) || (indexClosestK==initLam && indexClosestK==initK ) ||
        (indexLamda==initLam && indexLamda==initK) );


      if( indexLamda-indexClosestK==-1 ||indexLamda-indexClosestK==allPolys[i+1].size()-1){
        std::swap(indexClosestK,indexLamda);
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
      
        // Find closest Point K
        for(int k=0;k<finalPoly.size();k++){
          Point_2 vert=finalPoly.vertex(k);
          double vertDist=sqrt(((vert[0]-points[j][0])*(vert[0]-points[j][0]))+((vert[1]-points[j][1])*(vert[1]-points[j][1])));
            
          if(vertDist<dist){
            dist=vertDist;
            closePoint=vert;
            indexClosePoint=k;
          }        
        }
        COUT<<"CLOSEST POINT TO POINT_LEFT: "<<points[j]<<" IS "<< closePoint << " with INDEX "<< indexClosePoint<<ENDL;

        auto veit=finalPoly.vertices_begin();
        while(*veit!=closePoint){
          if(veit+1!=finalPoly.vertices_end()){
            veit++;
          }else{
            veit=finalPoly.vertices_begin();
          }

        }

        Segment_2 pointLine(*(veit+1),points[j]);
        
        if(isVisible(pointLine,finalPoly)){
          COUT<<"Point is Visible "<<ENDL;
          finalPoly.insert(veit+1,points[j]);
        }else{
          COUT<<"SUCKS to be me "<<ENDL;
          finalPoly.insert(veit,points[j]);
        }
     
      }
    }
      // if(i==check){
      //   CGAL::IO::write_polygon_WKT(os,finalPoly);
      // }
      if(finalPoly.is_simple()){
        COUT<<"polygon at DEPTH: "<< i << " IS OK"<<ENDL;
      }else{
        COUT<<"polygon at DEPTH: "<< i << " IS NOT OK"<<ENDL; 
      }
  }
  CGAL::IO::write_polygon_WKT(os,finalPoly);

  if( finalPoly.size()==initPointsSize && finalPoly.is_simple() ){
    COUT<<"final polygon of "<< initPointsSize<<" points" <<" IS OK"<<ENDL;
  }else{
    COUT<<"final polygon of "<< initPointsSize<<" points" <<" IS NOT OK"<<ENDL;
    if(!finalPoly.is_simple()){
      COUT<<"IT IS NOT SIMPLE"<<ENDL;      
    }
    if(finalPoly.size()!=initPointsSize){
      COUT<<"IT IS MISSING POINTS"<<ENDL;      
    }    
  }

  int area=finalPoly.area();

  COUT<<"AREA IS "<<area<<ENDL;

  COUT<<"INITIAL m was "<<initialM<<ENDL;

  return finalPoly;
}

// Practically checks whether <initialEdge> intersects with <poly> in more than 2 spots since initialEdge[1] is a vertex of <poly>
bool isVisible(Segment_2 initialEdge, Polygon_2 poly)
{
    int timesInter=0;
    for(auto eit=poly.edges_begin();eit!=poly.edges_end();eit++){
      const auto res=CGAL::intersection(initialEdge,*eit);
      if(res){
        timesInter++;
        if(timesInter>2){
          return false;
        }
      }      
    }
    if(timesInter==2){
      return true;
    }else{
      return false;
    }

}


// returns the index after <index> in Polygon_2 <poly>
int nextIndex(int index, Polygon_2 poly){
  int next=0;
  
  if(index!=poly.size()-1){
    next = index+1;
  }else{
    next=0;
  }

  return next;
}

// returns the index before <index> in Polygon_2 <poly>
int previousIndex(int index, Polygon_2 poly){
  int previous=0;
  
  if(index!=0){
    previous = index-1;
  }else{
    previous=poly.size()-1;
  }

  return previous;
}