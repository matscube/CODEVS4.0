//
//  IOManager.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "IOManager.h"

void IOManager::input() {
    int timeLimit; cin >> timeLimit;
    int currentStage; cin >> currentStage;
    int currentTurn; cin >> currentTurn;
    int playerResourceCount; cin >> playerResourceCount;
    
    int playerUnitCount; cin >> playerUnitCount;
    for (int i = 0; i < playerUnitCount; i++) {
        int unitID, unitPosY, unitPosX, unitHP, unitType;
        cin >> unitID >> unitPosY >> unitPosX >> unitHP >> unitType;
    }
    
    int enemyUnitCount; cin >> enemyUnitCount;
    for (int i = 0; i < enemyUnitCount; i++) {
        int unitID, unitPosY, unitPosX, unitHP, unitType;
        cin >> unitID >> unitPosY >> unitPosX >> unitHP >> unitType;
    }
    
    int resourceCount; cin >> resourceCount;
    for (int i = 0; i < resourceCount; i++) {
        int resourcePosY, resourcePosX;
        cin >> resourcePosY >> resourcePosX;
    }
}