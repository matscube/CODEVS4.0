//
//  Player.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__Player__
#define __CODEVS4_0__Player__

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <iostream>


using namespace std;

enum class PlayerUnitType {
    Worker,
    Knight,
    Fighter,
    Assassin,
    Castle,
    Village,
    Base,
};

string PlayerUnitTypeName(PlayerUnitType t);

enum class PlayerUnitActionType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    CreateWorker,
    CreateKnight,
    CreateFighter,
    CreateAssassin,
    CreateVillage,
    CreateBase,
    None,
};

enum class PlayerUnitStatus {
    Idle,
    FixPosition,
    Reserved,
};

string PlayerUnitStatusName(PlayerUnitStatus s);

class PlayerUnit {
public:
    int ID;
    int x, y;
    int hitPoint;
    PlayerUnitType type;
    PlayerUnitStatus status;

    PlayerUnit();
    PlayerUnit(int ID, int x, int y, PlayerUnitType type);
    void setHitPoint(int hitPoint);
    PlayerUnitActionType moveToTargetAction(int targetX, int targetY);
    
    static int cost(PlayerUnitType type);
    static string action(PlayerUnitActionType type);
    static int viewRange(PlayerUnitType type);
    static int attackRange(PlayerUnitType type);
    // TODO : including calcing cost
    bool isMovable();
    bool isCreatableWorker();
    bool isCreatableAttacker();
    bool isCreatableVillage();
    bool isCreatableBase();

    void fix();
    void fixOnlyPosition();
};



class Player {
public:
    Player();
    int resourceCount;
  
    // TODO : units by type
//    map<int, PlayerUnit> villageUnits;
    map<int, PlayerUnit> units;
    
    int calcWorkerCount();
    int calcVillageCount();
    int calcBaseCount();
    int calcAssassinCount();
    bool hasResource(PlayerUnitType t);
    void resetWithTurn();
};


#endif /* defined(__CODEVS4_0__Player__) */
