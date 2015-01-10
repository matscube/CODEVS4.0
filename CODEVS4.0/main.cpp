//
//  main.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include "IOManager.h"
#include "Field.h"
#include "Player.h"
#include "Game.h"
#include "AI.h"
#include "QuickAI.h"
#include "Library.h"

using namespace std;

int main(int argc, const char * argv[]) {
    cout << "matscube" << endl;
    
    Game game = Game(0);
    Field field = Field();
    Player player = Player();
    IOManager iOManager(game, field, player);

    QuickAI ai = QuickAI(game, field, player);

    while (true) {
        if (game.isNextStage(iOManager.inputStage())) {
            field.resetWithStage();
            player.resetWithStage();
            iOManager.resetWithStage(game, field, player);
        }
        iOManager.inputBody();

        // debug output
        ai.debug();
        
        // reset AI
        ai.resetWithTurn();
        
        // Mark: AI Commands **********************************************************
        
        ai.resourceAssignCommand(INF);

        ai.searchCommand(ai.searchLine1(), 1);
        ai.searchCommand(ai.searchLine2(), 1);
        ai.searchCommand(ai.searchLine3(), 1);
        ai.searchCommand(ai.searchLine4(), 1);
        ai.searchCommand(ai.searchLine5(), 1);

        // Output AI Commands
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
