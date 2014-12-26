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

        if (player.calcWorkerCount() < 110)
            ai.addCommands(ai.createWorkerCommand(5));
        
        if (isValidIndex(field.castlePosition.first, field.castlePosition.second))
            ai.addCommands(ai.attackCastleCommand(INF));
        
        if (ai.isSearchable())
            ai.addCommands(ai.searchResourceCommand(20));

//        ai.addCommands(ai.getResourceCommand(10));

        if (player.calcVillageCount() < 5)
            ai.addCommands(ai.createVillageCommand(INF));
        if (player.calcBaseCount() < 5)
            ai.addCommands(ai.createBaseCommand(10));
        
        ai.addCommands(ai.createAttakerCommand(INF));

        ai.addCommands(ai.getResourceCommand(INF));
        
        
        
        iOManager.output(ai.getCommands());
//        iOManager.testInput();
//        iOManager.testOutput();
    }


    
    return 0;
}
