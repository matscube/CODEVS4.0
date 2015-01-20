//
//  Field.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__Field__
#define __CODEVS4_0__Field__


#include "Library.h"
#include "Player.h"
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>

#define MAX_FIELD_WIDTH 100
#define MAX_FIELD_HEIGHT 100
#define MAX_FIELD_CELL_COUNT 10000
#define MAX_GETTING_RESOURCE 5


using namespace std;
typedef pair<int, int> Position;


bool isValidIndex(Position p);

class ResourceUnit {
public:
    Position position;
    int hashID;
    int occupancy;
    ResourceUnit();
    ResourceUnit(Position position);
    static int getHashID(Position position);
};

class Field {
    ofstream ofs;
public:

    Field();
    void resetWithStage();

    // TODO: status unuesd?
    // MARK: Visited Status
    bool isViewed[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool willBeViewed[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool isVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool willBeVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    map<int, Position> enemyCastlePositions(PlayerType pType); // <hashID, position>

    // MARK: Resource status
    map<int, ResourceUnit> resources;
    int allyWorkers[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    int reservedWorkers[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    

    void resetStatusWithTurn();
    void updateStatusWithAllyUnit(PlayerUnit playerUnit);
    void updateStatusWithResourceUnit(ResourceUnit resourceUnit);
    void updateWithPlayerUnit(PlayerUnit *playerUnit);
    
    // MARK: count
    int calcVisited();
};

#endif /* defined(__CODEVS4_0__Field__) */
