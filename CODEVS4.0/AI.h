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

    vector<Command> createWorkerCommand();
    vector<Command> randomWalkCommand();
    vector<Command> getResourceCommand();
    vector<Command> searchResourceCommand();
    
    
    bool isSearchable();
    
    void debug();
};

#endif /* defined(__CODEVS4_0__AI__) */
