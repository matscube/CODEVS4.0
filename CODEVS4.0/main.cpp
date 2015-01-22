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
        field.debugStatusInfo();

//        Position defendBasePos(30, 30);
        Position castleBasePos = player.castle.position;
        Position rightBasePos = Position(99, 30);
        if (ai.calcResourceGetting() < 40) {
            ai.createVillageOnResourceCommand(INF, 100);
            ai.searchUnkownFieldSmallCommand(INF);
            ai.getResourceCommand(INF);
            
            ai.searchUnkownFieldMediumCommand(INF);
            ai.searchUnkownFieldAllCommand(INF);
            ai.defendResourceCommand(INF);
        } else if (ai.defenderBaseCount(castleBasePos) == 0) {
            int castleBaseCount = ai.createDefenderBaseCommand(castleBasePos, 100);
            if (castleBaseCount == 0) {
                ai.setCostLimit(500);
            }
            ai.createVillageOnResourceCommand(INF, 100);
            ai.searchUnkownFieldMediumCommand(INF);
            ai.getResourceCommand(INF);

            ai.searchUnkownFieldAllCommand(INF);
            ai.defendResourceCommand(INF);
        } else if (ai.defenderBaseCount(rightBasePos) == 0) {
            ai.createCastleDefenderCommand(castleBasePos, 1, 50);

            ai.createVillageOnResourceCommand(INF, 50);
            ai.searchUnkownFieldAllCommand(INF);
            ai.getResourceCommand(INF);
            
            ai.defendCastleCommand(30);
            ai.defendResourceCommand(INF);
            
            /*
            ai.createCastleDefenderCommand(defendBasePos, 1, 50);
            ai.defendCastleCommand(30);

            ai.createDefenderBaseCommand(rightBasePos, 50);
            ai.searchUnkownFieldMediumCommand(INF);

            ai.searchUnkownFieldAllCommand(INF);
            ai.defendResourceCommand(INF);*/
        } else {
            /*
            ai.createCastleDefenderCommand(defendBasePos, 1, 50);
            ai.defendCastleCommand(30);

            ai.createCastleDefenderCommand(rightBasePos, 1, 20);
            
            ai.defendResourceCommand(INF);
            
            ai.createVillageOnResourceCommand(INF, 30);
            ai.searchUnkownFieldAllCommand(INF);
            ai.getResourceCommand(INF);
             */
        }
        
        

        // Output AI Commands
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
