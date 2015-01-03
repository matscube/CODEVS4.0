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
#include "Library.h"

using namespace std;

int main(int argc, const char * argv[]) {
    cout << "matscube" << endl;
    
    Game game = Game(0);
    Field field = Field();
    Player player = Player();
    AI ai = AI(game, field, player);
    IOManager iOManager(game, field, player);
    
    while (true) {
        if (game.isNextStage(iOManager.inputStage())) {
            field.resetWithStage();
            player.resetWithStage();
            iOManager.resetWithStage(game, field, player);
        }
        iOManager.inputBody();

        // debug output
        ai.debug();
        
        // AI
        ai.resetWithTurn();
        
        // Mark: AI Commands **********************************************************
        
        // Search Enemy Castle
        if (!isValidIndex(field.castlePosition.first, field.castlePosition.second)) {
            if (player.calcWorkerCount() < 10) {
                ai.addCommands(ai.searchEnemyCastle(1));
            } else {
                ai.addCommands(ai.searchEnemyCastle(3));
            }
        }

        // Get Resource
        ai.addCommands(ai.getMinimumResourceCommand(INF));
        ai.addCommands(ai.createVillageOnResource(INF));
        ai.addCommands(ai.createWorkerOnResource(INF));
        ai.setResourceLimit(PlayerUnit::cost(PlayerUnitActionType::CreateWorker) * 10);

        // Search Resource
        if (player.calcVillageCount() < 20) {
            if (ai.isSearchable()) {
                if (game.currentTurn < 50) {
                    ai.addCommands(ai.searchResourceWithRangeCommand(5, 8));
                } else if (game.currentTurn < 80) {
                    ai.addCommands(ai.searchResourceWithRangeCommand(10, 8));
                } else {
                    ai.addCommands(ai.searchResourceWithRangeCommand(15, 8));
                }
            }
        }

        // create base
        if (player.calcBaseCount() < 3) {
            ai.addCommands(ai.createBaseOnNearestEnemy());
        }
        
        ai.addCommands(ai.createAttakerCommand(INF));


        // set resource other workers
        ai.addCommands(ai.getResourceCommand(INF));
        if (player.calcVillageCount() < 20 && ai.isSearchable())
            ai.addCommands(ai.searchResourceNearestCommand(INF));
        
        // attack castle
        if (isValidIndex(field.castlePosition.first, field.castlePosition.second))
            ai.addCommands(ai.attackCastleCommand(INF));
                
        
        iOManager.output(ai.getCommands());
    }


    
    return 0;
}
