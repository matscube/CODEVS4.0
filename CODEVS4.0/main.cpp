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

//        Position defendBasePos(30, 30);
        Position defendBasePos = player.castle.position;
        if (ai.calcResourceGetting() < 30) {
            ai.createVillageOnResourceCommand(INF, 100);
            ai.searchUnkownFieldSmallCommand(INF);
            ai.getResourceCommand(INF);
        } else if (ai.defenderBaseCount(defendBasePos) == 0) {
            ai.createDefenderBaseCommand(defendBasePos, 1, 100);
            ai.searchUnkownFieldMediumCommand(INF);
        } else {
            ai.createCastleDefenderCommand(defendBasePos, 1, 70);
            ai.defendCastleCommand(30);
            
            ai.createVillageOnResourceCommand(INF, 30);
            ai.searchUnkownFieldAllCommand(INF);
            ai.getResourceCommand(INF);
        }
        
//        ai.searchUnkownFieldMediumCommand(INF);
        

        // Output AI Commands
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
