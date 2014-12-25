//
//  IOManager.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "IOManager.h"

IOManager::IOManager(Game &game, Field &field, Player &player) {
    IOManager::game = &game;
    IOManager::field = &field;
    IOManager::player = &player;
}

int IOManager::nextInt() {
    int n = -1;
    cin >> n;
    if (n == -1) {
        cerr << "[IOManager::nextInt] Error: input error." << endl;
    }
//    cerr << n << endl;
    return n;
}

void IOManager::testInput() {
    int timeLimit; cin >> timeLimit;
    int currentStage; cin >> currentStage;
    int currentTurn; cin >> currentTurn; cerr << "Current Turn: " << currentTurn << endl;
    int resourceCount; cin >> resourceCount;
    
    int allyUnitCount; cin >> allyUnitCount;
    for (int i = 0; i < allyUnitCount; i++) {
        int unitID; cin >> unitID;
        int posY; cin >> posY;
        int posX; cin >> posX;
        int unitHP; cin >> unitHP;
        int unitType; cin >> unitType;
    }
    
    int enemyUnitCount; cin >> enemyUnitCount;
    for (int i = 0; i < enemyUnitCount; i++) {
        int unitID; cin >> unitID;
        int posY; cin >> posY;
        int posX; cin >> posX;
        int unitHP; cin >> unitHP;
        int unitType; cin >> unitType;
    }
    
    int visibleResourceCount; cin >> visibleResourceCount;
    for (int i = 0; i < visibleResourceCount; i++) {
        int posY; cin >> posY;
        int posX; cin >> posX;
    }
    finishInput();
}

void IOManager::input() {
    // Input game information
    int timeLimit = nextInt();
    game->currentStage = nextInt();
    game->currentTurn = nextInt();
    player->resourceCount = nextInt();
    cerr << "Current Stage, Turn: " << game->currentStage << ", " << game->currentTurn << endl;
    
    // Input PlayerUnit Information
    player->resetWithTurn();
    field->resetStatusWithTurn();
    int playerUnitCount = nextInt();
    for (int i = 0; i < playerUnitCount; i++) {
        int unitID = nextInt();
        int unitPosY = nextInt();
        int unitPosX = nextInt();
        int unitHP = nextInt();
        int unitType = nextInt();

        PlayerUnit unit = PlayerUnit(unitID, unitPosX, unitPosY, PlayerUnitType(unitType));
        unit.setHitPoint(unitHP);
        field->updateStatusWithAllyUnit(unit);
        field->updateVisited(&unit);
        player->units[unit.ID] = unit;
    }
    
    // Input EnemyUnit Information
    int enemyUnitCount = nextInt();
    for (int i = 0; i < enemyUnitCount; i++) {
        int unitID = nextInt();
        int unitPosY = nextInt();
        int unitPosX = nextInt();
        int unitHP = nextInt();
        int unitType = nextInt();
    }
    
    // Input FieldUnit Information
    int resourceCount; cin >> resourceCount;
    for (int i = 0; i < resourceCount; i++) {
        int resourcePosY = nextInt();
        int resourcePosX = nextInt();

        FieldUnit unit = FieldUnit(resourcePosX, resourcePosY, FieldUnitType::Resource);
        field->updateStatusWithFieldUnit(unit);
    }
    
    finishInput();
}

void IOManager::finishInput() {
    string endStr;
    cin >> endStr;
    if (endStr != "END") {
        cerr << "[IOManager] Error: end of input" << endl;
    }
}

void IOManager::firstOutput() {
    cout << "matscube" << endl; cout.flush();
}

void IOManager::output(vector<Command> commands) {
    int sz = (int)commands.size();
    
    cout << sz << endl; cout.flush();
    
    vector<Command>::iterator ite = commands.begin();
    while (ite != commands.end()) {
        if (ite->second == PlayerUnitActionType::None) {
            cerr << "[IOManager::output] Error: 'None' action is called." << endl;
            continue;
        } else {
//            cerr << ite->first << " " << PlayerUnit::action(ite->second) << endl; cerr.flush();
        }
        cout << ite->first << " " << PlayerUnit::action(ite->second) << endl; cout.flush();
        ite++;
    }
}

void IOManager::testOutput() {
    cout << 0 << endl;
    cout.flush();
}
