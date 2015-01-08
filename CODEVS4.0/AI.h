//
//  AI.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__AI__
#define __CODEVS4_0__AI__

#include "Game.h"
#include "Field.h"
#include "Player.h"
#include "Library.h"
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;

typedef pair<int, PlayerUnitActionType> Command;

class AI {
    Game *game;
    Field *field;
    Player *player;
    vector<Command> commands;
    ofstream ofs;
    ofstream fieldOfs;
public:
    AI(Game &game, Field &field, Player &player);
    void resetWithTurn();
    
    vector<Command> getCommands();
    
    void addCommands(vector<Command> newCommands);
    
    // TODO: cost limit system
    void setResourceLimit(int n);
    void releaseResourceLimit();
    

    vector<Command> createWorkerCommand(int assign);
    vector<Command> createVillageCommand(int assign);
    vector<Command> createBaseCommand(int assign);

    vector<Command> createAttakerCommand(int assign);

    // *********** get resource *************
    // get resource system
    vector<Command> getMinimumResourceCommand(int assign);
    vector<Command> createVillageOnResource(int assign);
    vector<Command> createWorkerOnResource(int assign);
    
    // get resource utility
    vector<Command> getResourceCommand(int assign);
    vector<Command> searchResourceNearestCommand(int assign);
    vector<Command> searchResourceWithRangeCommand(int assign, int depth);

    // ************ create base **************
    // create base
    Position basePointNearestToEnemy();
    int calcDistanceToEnemy(Position p);
    vector<Command> createBaseOnNearestEnemy();

    // ************ create village **************
    // create village
    Position villagePointNearestToEnemy();
    vector<Command> createVillageOnNearestEnemy();
    vector<Command> createBaseEnemyArea(int assign);
    vector<Command> createWorkerOnVillage();

    vector<Command> createAttackerOnBase();

    // ************ defend castle ****************
    vector<Command> setWorkerOnCastle();
    vector<Command> createBaseOnCastle();
    vector<Position> defendingArea();
    bool isBaseReady();
    vector<Command> createDefenderOnCastle();
    vector<Command> setDefenderOnCastle();
    
    // ************ defend resource ***************
    vector<Command> setWorkerFieldCenter();
    vector<Command> createVillageOnFieldCenter();
    vector<Command> createBaseOnFieldCenter();
    vector<Position> fieldCenterArea();
    bool isFieldCenterVillageReady();
    bool isFieldCenterBaseReady();
    vector<Command> createDefenderOnFieldCenter();
    vector<Command> setDefenderOnResource();
    
    // ************ attack castle ****************
    // attack castle
    vector<Command> searchEnemyCastle(int assign);
    vector<Command> attackCastleCommand(int assign);

    // utility
    vector<Command> randomWalkCommand();
    
    bool isSearchable();
    
    void debug();
};

#endif /* defined(__CODEVS4_0__AI__) */
