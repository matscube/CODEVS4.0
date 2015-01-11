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
typedef pair<int, int> Position;

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
    PlayerUnitActionType moveToTargetAction(Position target);
    
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

// TODO: rename
enum class PlayerType {
    Ally,
    Enemy,
};

class Player {
public:
    Player();
    int resourceCount;
    int necessaryResourceCount;
    bool isViewdCastle;

    PlayerType type;
    void updateType(PlayerUnit p);

    // MARK: Unit
    void clearUnits();
    void updateUnit(PlayerUnit pUnit);
    map<int, PlayerUnit *> units;
    map<int, PlayerUnit *> attackers;
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
    int calcAttackerCount();
};


#endif /* defined(__CODEVS4_0__Player__) */
