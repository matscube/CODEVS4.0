//
//  QuickAI.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/08.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__QuickAI__
#define __CODEVS4_0__QuickAI__

#include "Game.h"
#include "Field.h"
#include "Player.h"
#include "Library.h"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

typedef pair<int, PlayerUnitActionType> Command;

class QuickAI {
    Game *game;
    Field *field;
    Player *player;
    Player *enemy;
    vector<Command> commands;
    void addCommand(Command newCommand);
    void addCommands(vector<Command> newCommands);
public:
    QuickAI(Game &game, Field &field, Player &player, Player &enemy);
    void resetWithTurn();
    void resetWithStage();
    vector<Command> getCommands();
    
    // MARK: Cost Manager
    void setNecessaryResource(int n);
    void releaseNecessaryResource();
    
    // MARK: Search----------------------
    void searchNoVisitedAreaCommand(vector<Position> area, int assign, map<PlayerUnitType, bool> types);
    map<PlayerUnitType, bool> allTypes();
    map<PlayerUnitType, bool> workerTypes();
    map<PlayerUnitType, bool> attackerTypes();

    // MARK: searchLine
    vector<Position> searchLineToRight1();
    vector<Position> searchLineToRight2();
    vector<Position> searchLineToRight3();
    vector<Position> searchLineToRight4();
    vector<Position> searchLineToRight5();
    vector<Position> searchLineToDown1();
    vector<Position> searchLineToDown2();
    vector<Position> searchLineToDown3();
    vector<Position> searchLineToDown4();
    vector<Position> searchLineToDown5();
    vector<Position> searchLineAlly();
    void searchUnkownFieldSmallCommand();
    void searchUnkownFieldMediumCommand();

    // MARK: Back Attack
    void assignRightLineCommand();
    void assignDownLineCommand();
    int baseCountOnEnemyArea();
    void createOneBaseOnEnemyAreaCommand();
    void createOneMoreBaseOnEnemyAreaCommand();
    void createAttackerOnBaseCommand();
    bool firstCannonReleased;
    void firstCannonCommand();
    void primaryCannonCommand();
    bool poolAttackerOnBaseCommand(int need);
    
    // MARK: Search Castle
    vector<Position> searchEnemyCastleLine1();
    vector<Position> searchEnemyCastleLine2();
    vector<Position> searchEnemyCastleLine3();
    vector<Position> searchEnemyCastleLine4();
    vector<Position> searchEnemyCastleLine5();
    void searchEnemyCastleCommand();
    
    // MARK: Defend Castle
    int baseCountOnDefendingArea();
    bool isDefendingBase(PlayerUnit *pUnit);
    void createBaseOnDefendingAreaCommand();
    void createDefenderOnBaseCommand();
    
    int castleDefenderCount();
    void denendCastleCommand(int assign);
    
    // MARK: Attack Castle
    void attackCastleCommand();

    // MARK: Resource
    void createVillageOnResourceCommand();
    void fixResourceCommand();
    
    // MARK: Worker
    void supplyMovableWorkerWithCastle(int need);
    void supplyWorkerForSearchCommand(int need);

    // MARK: Attacker
    vector<PlayerUnitType> attackerTypesPack(int knight, int fighter, int assasin);
    
    
    void debug();
    
};

#endif /* defined(__CODEVS4_0__QuickAI__) */
