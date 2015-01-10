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

using namespace std;

typedef pair<int, PlayerUnitActionType> Command;

class QuickAI {
    Game *game;
    Field *field;
    Player *player;
    vector<Command> commands;
    void addCommands(vector<Command> newCommands);
public:
    QuickAI(Game &game, Field &field, Player &player);
    void resetWithTurn();
    vector<Command> getCommands();
    
    // MARK: Search
    vector<Position> searchLine1();
    vector<Position> searchLine2();
    vector<Position> searchLine3();
    vector<Position> searchLine4();
    vector<Position> searchLine5();
    vector<Position> searchArea();
    void searchUnkownAreaCommand(vector<Position> area, int assign);
    void searchUnkownFieldCommand();

    // MARK: Resource
    void createVillageOnResourceCommand();
    void fixResourceCommand();

    
    void debug();
    
};

#endif /* defined(__CODEVS4_0__QuickAI__) */
