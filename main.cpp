#include <iostream>
#include <fstream>
#include <string.h>
#include "shared.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;

PointList getPointsFromFile(string filepath, int& size);
void handleArgs(ArgFlags& argFlags, int& argc, char**& argv);

int main(int argc, char **argv)
{
    ArgFlags argFlags;
    handleArgs(argFlags, argc, argv);
    
    //check for input error
    if(argFlags.error)
    {
        cout << argFlags.errorMessage << endl;
        cout << "Usage: /to_polygon -i <point set input file> -o <output file> -algorithm <incremental or convex_hull or onion> -edge_selection <1 or 2 or 3 | not for onion> -initialization <1a or 1b or 2a or 2b | only for incremental> -onion_initialization <1 to 5>" << endl;
        return -1;
    }

    int size;
    PointList list = getPointsFromFile(argFlags.inputFile, size);
    //for(int i = 0; i < size; i++) cout << i << ": " << list[i] << endl;

    return 0;
}

void handleArgs(ArgFlags& argFlags, int& argc, char**& argv)
{
    int waitingForArg = 0;
    bool waitingInput = true;
    bool waitingOutput = true;
    bool waitingAlgorithm = true;
    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        switch (waitingForArg)
        {
            case 0:
                if (!strcmp(arg, "-i"))
                    waitingForArg = 1;
                else if (!strcmp(arg, "-o"))
                    waitingForArg = 2;
                else if (!strcmp(arg, "-algorithm"))
                    waitingForArg = 3;
                else if (!strcmp(arg, "-edge_selection"))
                    waitingForArg = 4;
                else if (!strcmp(arg, "-initialization"))
                    waitingForArg = 5;
                else if (!strcmp(arg, "-onion_initialization"))
                    waitingForArg = 6;
                break;
            case 1:
                argFlags.inputFile = string(arg);
                waitingInput = false;
                waitingForArg = 0;
                break;
            case 2:
                argFlags.outputFile = string(arg);
                waitingOutput = false;
                waitingForArg = 0;
                break;
            case 3:
                if(!strcmp(arg, "incremental"))
                    argFlags.algorithm = incremental;
                else if(!strcmp(arg, "convex_hull"))
                    argFlags.algorithm = convex_hull;
                else if(!strcmp(arg, "onion"))
                    argFlags.algorithm = onion;
                waitingAlgorithm = false;
                waitingForArg = 0;
                break;
            case 4:
                argFlags.edgeSelection = static_cast<EdgeSelection>(atoi(arg)-1);
                waitingForArg = 0;
                break;
            case 5:
                if(!strcmp(arg, "1a"))
                    argFlags.initialization = a1;
                else if(!strcmp(arg, "1b"))
                    argFlags.initialization = b1;
                else if(!strcmp(arg, "2a"))
                    argFlags.initialization = a2;
                else if(!strcmp(arg, "2b"))
                    argFlags.initialization = b2;
                waitingForArg = 0;
                break;
            case 6:
                argFlags.onionInitialization = atoi(arg);
                waitingForArg = 0;
                break;
        }
    }

    if(waitingAlgorithm){
        argFlags.error = true;
        argFlags.errorMessage = string("Must select algorithm");
        return;
    }
    if(waitingInput){
        argFlags.error = true;
        argFlags.errorMessage = string("Must select input file");
        return;
    }
    if(waitingOutput){
        argFlags.error = true;
        argFlags.errorMessage = string("Must select output file");
        return;
    }

    argFlags.error = false;
}

PointList getPointsFromFile(string filepath, int& size)
{
    //get number of lines
    ifstream infile(filepath);
    int linesCount = 0;
    string line;

    while(getline(infile, line))linesCount++;
    int pointsCount = linesCount - 2;   //files always have 2 comments and then just points
    size = pointsCount;

    //allocate space
    PointList list = (PointList) malloc(sizeof(Point_2)*pointsCount);
    
    //reset reading cursor
    infile.clear();
    infile.seekg(0);

    //ignore first 2 lines
    getline(infile, line);
    getline(infile,line);
    
    //read points
    int x,y;
    for(int i = 0; getline(infile, line); i++)
    {
        int ignore;
        std::istringstream iss(line);
        iss >> ignore >> x >> y;
        list[i] = Point_2(x, y);
    }

    return list;
}

void printArguments(ArgFlags& argFlags){
    cout << "Input file: " << argFlags.inputFile << endl;
    cout << "Output file: " << argFlags.outputFile << endl;
    cout << "Algorithm: " << argFlags.algorithm << endl;
    cout << "Edge selection: " << argFlags.edgeSelection << endl;
    cout << "Initialization: " << argFlags.initialization << endl;
    cout << "Onion initialization: " << argFlags.onionInitialization << endl;
}