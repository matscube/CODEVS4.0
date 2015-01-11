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

enum class FieldUnitType {
    Resource,
};

class FieldUnit {
public:
//    int x, y;
    Position position;
    int hashID;
    int occupancy;
    FieldUnitType type;
    FieldUnit();
    FieldUnit(Position position, FieldUnitType type);
    static int getHashID(Position position);
};

enum class FieldStatus {
    Unknown,
    Visited,
    Resource,
    AllyCastle,
    EnemyCastle,
};
string FieldStatusName(FieldStatus s);

class Field {
    ofstream ofs;
public:
//    bool isViewdEnemyCastle;
//    Position allyCastlePosition;
//    Position enemyCastlePosition;

    Field();
    void resetWithStage();

    // TODO: status unuesd?
    FieldStatus status[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool isViewed[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool willBeViewed[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool isVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    bool willBeVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    int calcVisited();
    map<int, Position> enemyCastlePositions(PlayerType pType); // <hashID, position>
//    vector<Position> enemyCastlePositions(PlayerType pType); // <hashID, position>

    map<int, FieldUnit> resources;
    int allyWorkers[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    int reservedWorkers[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];

    void resetStatusWithTurn();
    void updateStatusWithAllyUnit(PlayerUnit playerUnit);
    void updateStatusWithFieldUnit(FieldUnit fieldUnit);
    void updateWithPlayerUnit(PlayerUnit *playerUnit);
    
};

#endif /* defined(__CODEVS4_0__Field__) */
