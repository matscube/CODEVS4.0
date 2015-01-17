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
#include "ExtraAI.h"
#include "Library.h"

using namespace std;

int main(int argc, const char * argv[]) {
    cout << "matscube" << endl;
    
    Game game = Game(0);
    Field field = Field();
    Player player = Player();
    Player enemy = Player();
    
    PlayerUnit enemyCastle = PlayerUnit(INF, Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT), PlayerUnitType::Castle, &enemy);
    enemy.castle = enemyCastle;
    
    IOManager iOManager(game, field, player, enemy);

    ExtraAI ai = ExtraAI(game, field, player, enemy);

    while (true) {
        if (game.isNextStage(iOManager.inputStage())) {
            field.resetWithStage();
            player.resetWithStage();
            iOManager.resetWithStage(game, field, player, enemy);
            ai.resetWithStage();
        }
        iOManager.inputBody();

        // debug output
        ai.debug();
        
        // reset AI
        ai.resetWithTurn();
        
        // Mark: AI Commands **********************************************************
        
        cerr << "T: " << game.currentTurn << " ResGet" << ai.calcResourceGetting() << " Res" << player.resourceCount << endl;

        Position rPos(30, 20);
        Position dPos(20, 30);
        if (ai.defenderVillageCount(rPos) == 0 || ai.defenderVillageCount(dPos) == 0) {
//            cerr << "Step 1: Defender Village" << endl;
            ai.createDefenderVillageCommand(rPos, 1, 100);
            ai.createDefenderVillageCommand(dPos, 1, 100);
//            ai.supplyFreeWorkerWithCastleCommand(30, 100);
            ai.searchUnkownFieldSmallCommand(INF);
            ai.getResourceCommand(INF);

//            ai.searchUnkownFieldSmallCommand(4);
        } else if (ai.calcResourceGetting() < 30) {
//            cerr << "Step 2: Resource Getting" << endl;
            ai.supplyFreeWorkerWithVillageCommand(30, 100);
            ai.searchUnkownFieldSmallCommand(INF);
            ai.getResourceCommand(INF);
        }
        
//        ai.searchUnkownFieldMediumCommand(INF);
        

        // Output AI Commands
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
