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
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>


class PlayerUnit;

using namespace std;
typedef pair<int, int> Position;


bool isValidIndex(Position p);

enum class ResourceUnitStatus {
    Default,
    Ally,
    Enemy,
};
class ResourceUnit {
public:
    Position position;
    int hashID;
    ResourceUnitStatus status;
    ResourceUnit();
    ResourceUnit(Position position);
};

class Field {
//    ofstream ofs;
public:

    Field();
    void resetWithStage();
    void resetWithTurn();

    // MARK: Visited and Viewd Status
    bool isViewed[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool willBeViewed[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool isVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool willBeVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    
    // MARK: ally info
    int allyWorkerCount[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    int unitCount[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];

    // MARK: enemy info
    int enemyWorkerCount[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    map<int, Position> enemyCastlePositions(PlayerType pType); // <hashID, position>

    // MARK: Resource status
    map<int, ResourceUnit> resources;
    void updateWithResourceUnit(ResourceUnit resourceUnit);
    void updateResourceStatus();

    void updateWithPlayerUnit(PlayerUnit *playerUnit);

    // MARK: count
    int calcVisited();

    // MARK: debug
    void debugStatusInfo();

};

#endif /* defined(__CODEVS4_0__Field__) */
