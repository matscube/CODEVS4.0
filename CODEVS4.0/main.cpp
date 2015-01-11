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
    Player enemy = Player();
    IOManager iOManager(game, field, player, enemy);

    QuickAI ai = QuickAI(game, field, player, enemy);

    while (true) {
//        cerr << game.currentTurn << endl;
        
        if (game.isNextStage(iOManager.inputStage())) {
            field.resetWithStage();
            player.resetWithStage();
            iOManager.resetWithStage(game, field, player, enemy);
        }
        iOManager.inputBody();

        // debug output
        ai.debug();
        
        // reset AI
        ai.resetWithTurn();
        
        // Mark: AI Commands **********************************************************
        
        ai.createBaseOnLineCommand();
        ai.createAttackerOnLineCommand();

        ai.createVillageOnResourceCommand();
        ai.searchUnkownFieldCommand();
        ai.fixResourceCommand();
        ai.searchEnemyCastleCommand();
        
        ai.attackCastleCommand();

        // Output AI Commands
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
