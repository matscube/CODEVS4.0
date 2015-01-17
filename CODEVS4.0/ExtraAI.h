//
//  ExtraAI.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/17.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__ExtraAI__
#define __CODEVS4_0__ExtraAI__

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

class ExtraAI {
    Game *game;
    Field *field;
    Player *player;
    Player *enemy;
    vector<Command> commands;
    void addCommand(Command newCommand);
public:
    ExtraAI(Game &game, Field &field, Player &player, Player &enemy);
    void resetWithTurn();
    void resetWithStage();
    vector<Command> getCommands();
    
    // MARK: Basic Command
    void addCommandMove(PlayerUnit *pUnit, Position target);
    void addCommandCreateVillage(PlayerUnit *pUnit);
    
    // MARK: Defend
    Position defenderVillagePosition = Position(40, 40);
    int defenderVillageCount();
    int createDefenderVillageCommand(int assign, int prob);
    
    
    // MARK: Search----------------------
    void searchNoVisitedAreaCommand(vector<Position> area, int assign, map<PlayerUnitType, bool> types);
    map<PlayerUnitType, bool> allTypes();
    map<PlayerUnitType, bool> workerTypes();
    map<PlayerUnitType, bool> attackerTypes();
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
    void searchUnkownFieldSmallCommand(int assign);
    void searchUnkownFieldMediumCommand(int assign);
    
    
    // MARK: Attacker
//    vector<PlayerUnitType> attackerTypesPack(int knight, int fighter, int assasin);
    
    
    // MARK: Debug
    void debug();
    
};


#endif /* defined(__CODEVS4_0__ExtraAI__) */


