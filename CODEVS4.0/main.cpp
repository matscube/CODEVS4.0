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
    
//    PlayerUnit enemyCastle = PlayerUnit(INF, Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT), PlayerUnitType::Castle, &enemy);
//    enemy.castle = enemyCastle;
    
    IOManager iOManager(game, field, player, enemy);

    ExtraAI ai = ExtraAI(game, field, player, enemy);

    while (true) {
        if (game.isNextStage(iOManager.inputStage())) {
            field.resetWithStage();
            player.resetWithStage();
            enemy.resetWithStage();
            iOManager.resetWithStage(game, field, player, enemy);
            ai.resetWithStage();
        }
        iOManager.inputBody();

        // debug output
        ai.debug();
        
        // reset AI
        ai.resetWithTurn();
        
        // Mark: AI Commands **********************************************************
        
        // Debug
        cerr << "T: " << game.currentTurn << " ResGet" << ai.calcResourceGetting() << " Res" << player.resourceCount << endl;
//        cerr << "T: " << game.currentTurn << " EnemyCastle: " << castleModeString(enemy.castleMode) << endl;
//        field.debugStatusInfo();
//        cerr << enemy.castle.position.first << " " << enemy.castle.position.second << endl;

        // Ready
        ai.updateNearestEnemy();
        
        // Defending Mode
        /*
        if (ai.nearestEnemyDistance < 30) {
            Position castleBasePos = player.castle.position;
            
            if (ai.defenderBaseCount(castleBasePos) == 0) {
                int castleBaseCount = ai.createCastleBaseCommand(100);
                if (castleBaseCount == 0) {
                    ai.setCostLimit(500);
                }
            } else {
                ai.createDefenderCommand(castleBasePos, 1, 100);
                ai.defendCastleCommand(30);
            }
        }*/
        
        // Attack
        // create base
        int attackBaseCount = ai.calcAttackBase();
        if (attackBaseCount == 0) {
            ai.backAttackMoveRightCommand();
            ai.backAttackMoveDownCommand();
            ai.createOneAttackerBase();
        } else if (attackBaseCount == 1) {
            ai.createOneMoreAttackerBase();
        }
        ai.createAttackerCommand();
        if (!enemy.isViewdCastle) {
            ai.searchEnemyCastleCommand();
        }
        
        if (enemy.castleMode == CastleMode::Alone) {
            // Quick Attack
            ai.attackCastleCommand();
        } else if (enemy.castleMode == CastleMode::Defending) {
            // Pool Attack
            ai.poolAttackerCommand(60);
            ai.attackCastleCommand();
        } else {
            ai.poolAttackerCommand(30);
            ai.attackCastleCommand();
        }
        
        // Resource Getting
        ai.createVillageOnResourceCommand(INF, 100);
        ai.searchUnkownFieldSmallCommand(INF);
        ai.getResourceCommand(INF);

        ai.searchUnkownFieldMediumCommand(INF);
        ai.getResourceCommand(INF);
        
        ai.searchUnkownFieldAllCommand(INF);
        ai.getResourceCommand(INF);

        // End AI Commands **********************************************************

        // Output AI Commands
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
