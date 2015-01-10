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
    Unknown,
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

PlayerUnitType UnitTypeCreated(PlayerUnitActionType at);

PlayerUnitActionType CreateAttackerAction(PlayerUnitType t);

enum class PlayerUnitStatus {
    Idle,
    FixPosition,
    Reserved,
};

string PlayerUnitStatusName(PlayerUnitStatus s);

class Player;
class PlayerUnit {
public:
    int ID;
    int x, y;
    int hitPoint;
    PlayerUnitType type;
    PlayerUnitStatus status;
    Player *player;

    PlayerUnit();
    PlayerUnit(int ID, int x, int y, PlayerUnitType type, Player *player);
    void setHitPoint(int hitPoint);
    PlayerUnitActionType moveToTargetAction(int targetX, int targetY);
    
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

enum class PlayerType {
    Ally,
    Enemy,
};

class Player {
public:
    Player();
    int resourceCount;
    int necessaryResourceCount;

    PlayerType type;
    void updateType(PlayerUnit p);

    // MARK: Unit
    void clearUnits();
    void updateUnit(PlayerUnit pUnit);
    map<int, PlayerUnit *> units;
    PlayerUnit castle;
    map<int, PlayerUnit> villages;
    map<int, PlayerUnit> bases;
    map<int, PlayerUnit> workers;
    map<int, PlayerUnit> knights;
    map<int, PlayerUnit> fighters;
    map<int, PlayerUnit> assassins;
    
    // MARK: Reset
    void resetWithStage();
    void resetWithTurn();
    
    // MARK: Calc count
    int calcWorkerCount();
    int calcVillageCount();
    int calcBaseCount();
    int calcAssassinCount();
};


#endif /* defined(__CODEVS4_0__Player__) */
