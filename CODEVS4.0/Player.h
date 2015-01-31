//
//  Player.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__Player__
#define __CODEVS4_0__Player__


#include "PlayerUnit.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <iostream>

class Field;

// TODO: rename
string castleModeString(CastleMode mode);

class Player {
public:
    Player();
    int resourceCount;
    int necessaryResourceCount;
    bool isViewdCastle;

    PlayerType type;
    void updateType(PlayerUnit p);
    
    // MARK: For enemy
    CastleMode castleMode;
    CastleDefenderType defenderType;
    int maxCastleDefenderCount;
    void updateCastleMode(vector<PlayerUnit> inputEnemyUnits);

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
    void resetCastle();
    
    // MARK: Calc count
    int calcWorkerCount();
    int calcVillageCount();
    int calcBaseCount();
    int calcAssassinCount();
    int calcAttackerCount();
};


#endif /* defined(__CODEVS4_0__Player__) */
