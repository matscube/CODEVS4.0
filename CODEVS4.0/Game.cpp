//
//  Game.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Game.h"

Game::Game(int firstStage) {
    currentStage = firstStage;
    currentTurn = -1;
}

bool Game::isNextStage(int newStage) {
    if (newStage != currentStage) {
        cerr << "Reset New Stage: " << newStage << endl;
        currentTurn = -1;
        currentStage = newStage;
        return true;
    } else {
        return false;
    }

}