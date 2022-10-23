#ifndef SHARED_H
#define SHARED_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/IO/WKT.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Point_2<Kernel> Point_2;
typedef Point_2* PointList;

enum Algorithm {incremental, convex_hull, onion};
enum EdgeSelection {randomSelection, min, max};
enum Initialization {a1, a2, b1, b2};

struct ArgFlags{
    std::string inputFile;
    std::string outputFile;
    Algorithm algorithm;
    EdgeSelection edgeSelection;
    Initialization initialization;
    int onionInitialization;

    bool error;
    std::string errorMessage;
};

#endif