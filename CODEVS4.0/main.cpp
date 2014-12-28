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
    
    Game game = Game();
    Field field = Field();
    Player player = Player();
    AI ai = AI(game, field, player);
    
    
    IOManager iOManager = IOManager(game, field, player);

    for (int i = 0; i < 1000; i++) {

        iOManager.input();
        // debug output
        ai.debug();
        
        // AI
        ai.resetWithTurn();

        // ready to create base
//        if (player.calcVillageCount() > 5) {
//            ai.setResourceLimit(PlayerUnit::cost(PlayerUnitActionType::CreateBase));
//        }
        
        // attack castle
//        if (isValidIndex(field.castlePosition.first, field.castlePosition.second))
//            ai.addCommands(ai.attackCastleCommand(INF));
        
        // get resource fastly
        ai.addCommands(ai.searchEnemyCastle(1));
//        ai.addCommands(ai.getMinimumResourceCommand(INF));
//        ai.addCommands(ai.createVillageOnResource(INF));
//        ai.addCommands(ai.createWorkerOnResource(INF));

        
//        if (ai.isSearchable())
//            ai.addCommands(ai.searchResourceCommand(10));

        // create base
        ai.releaseResourceLimit();
        if (player.calcVillageCount() > 5 && player.calcBaseCount() < 5) {
//            ai.addCommands(ai.createBaseOnNearestEnemy(INF));
        }
        
//        ai.addCommands(ai.createAttakerCommand(INF));
        

        // set resource other workers
//        ai.addCommands(ai.getResourceCommand(INF));
//        ai.addCommands(ai.searchResourceCommand(INF));
        
        
        
        iOManager.output(ai.getCommands());
//        iOManager.testInput();
//        iOManager.testOutput();
    }


    
    return 0;
}
