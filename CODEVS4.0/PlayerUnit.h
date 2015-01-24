//
//  PlayerUnit.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/24.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__PlayerUnit__
#define __CODEVS4_0__PlayerUnit__

#include "Library.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <iostream>


using namespace std;
typedef pair<int, int> Position;

string PlayerUnitTypeName(PlayerUnitType t);
PlayerUnitType UnitTypeCreated(PlayerUnitActionType at);
PlayerUnitActionType CreateAttackerAction(PlayerUnitType t);


string PlayerUnitStatusName(PlayerUnitStatus s);

class Player;
class PlayerUnit {
public:
    int ID;
    //    int x, y;
    Position position;
    int hitPoint;
    PlayerUnitType type;
    PlayerUnitStatus status;
    Player *player;
    
    PlayerUnit();
    PlayerUnit(int ID, Position position, PlayerUnitType type, Player *player);
    void setHitPoint(int hitPoint);
    // TODO: use Position type;
    PlayerUnitActionType moveToTargetAction(Position target, bool synchro = false);
    
    int getHashID();
    
    static int cost(PlayerUnitActionType at);
    static string action(PlayerUnitActionType type);
    static int viewRange(PlayerUnitType type);
    static int attackRange(PlayerUnitType type);
    bool isMovable();
    bool isCreatableWorker();
    bool isCreatableAttacker(PlayerUnitType t);
    bool isCreatableVillage();
    bool isCreatableBase();
    
    bool isAttacker();
    
    void fix(PlayerUnitActionType at);
    void fixOnlyPosition();
};

#endif /* defined(__CODEVS4_0__PlayerUnit__) */
