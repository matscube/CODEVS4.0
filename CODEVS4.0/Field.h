//
//  Field.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__Field__
#define __CODEVS4_0__Field__

#include "Player.h"
#include <stdio.h>
#include <vector>
#include <map>

#define MAX_FIELD_WIDTH 100
#define MAX_FIELD_HEIGHT 100


using namespace std;


typedef pair<int, int> Position;

enum class FieldUnitType {
    Resource,
};

class FieldUnit {
    static int getHashID(int x, int y);
public:
    int x, y;
    int hashID;
    FieldUnitType type;
    FieldUnit();
    FieldUnit(int x, int y, FieldUnitType type);
};

enum class FieldStatus {
    Unknown,
    Visited,
    Resource,
    AllyCastle,
    EnemyCastle,
};

class Field {
public:
    Field();
    FieldStatus status[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    Position allyCastle;
    Position EnemyCastle;

    map<int, FieldUnit> resources;
    int allyWorkers[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    int reservedWorkers[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];

    
//    vector<FieldUnit> units;
    void initStatus();
    void resetStatusWithTurn();
    void updateStatusWithAllyUnit(PlayerUnit playerUnit);
    void updateStatusWithFieldUnit(FieldUnit fieldUnit);
    
};




#endif /* defined(__CODEVS4_0__Field__) */
