//
//  IOManager.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__IOManager__
#define __CODEVS4_0__IOManager__

#include "Field.h"
#include "Game.h"
#include "Player.h"
#include "Library.h"
#include "AI.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


class IOManager {
    Game *game;
    Field *field;
    Player *player;
    void finishInput();
    ofstream ofs;
public:
    IOManager(Game &game, Field &field, Player &player);
    int nextInt();
    void testInput();
    void input();
    void firstOutput();
    void output(vector<Command> commands);
    void testOutput();
};

#endif /* defined(__CODEVS4_0__IOManager__) */
