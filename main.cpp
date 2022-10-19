#include <iostream>
#include "shared.h"

using std::cout, std::endl, std::string;

PointList getPointsFromFile(string filepath);

int main(int argc, char const *argv[])
{
    //let first arg to be file name, ignore other parameters for now

    if(argc != 3)
    {
        cout << "Usage: main -f <path-to-file>" << endl;
        return -1;
    }    

    string filepath(argv[2]);
    cout << "path to file: " << filepath << endl;

    PointList list = getPointsFromFile(filepath);
    cout << list[0] << endl;

    return 0;
}

PointList getPointsFromFile(string filepath)
{
    PointList list = (PointList) malloc(sizeof(Point_2)*1);
    list[0] = Point_2(1, 2);
    return list;
    
    //TODO: Actually get points from file
}