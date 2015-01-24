//
//  IOManager.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__IOManager__
#define __CODEVS4_0__IOManager__

#include "Library.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

class Game;
class Field;
class Player;

using namespace std;

typedef pair<int, PlayerUnitActionType> Command;

class IOManager {
    Game *game;
    Field *field;
    Player *player;
    Player *enemy;
    void finishInput();
    ofstream ofs;
public:
    IOManager(Game &game, Field &field, Player &player, Player &enemy);
    void resetWithStage(Game &game, Field &field, Player &player, Player &enemy);
    int nextInt();
    void testInput();
    int inputStage();
    void inputBody();
    void firstOutput();
    void output(vector<Command> commands);
    void testOutput();
};

#endif /* defined(__CODEVS4_0__IOManager__) */
