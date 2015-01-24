//
//  IOManager.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "IOManager.h"
#include "Game.h"
#include "Field.h"
#include "Player.h"

IOManager::IOManager(Game &game, Field &field, Player &player, Player &enemy) {
    IOManager::game = &game;
    IOManager::field = &field;
    IOManager::player = &player;
    IOManager::enemy = &enemy;
    ofs = ofstream("/Users/matscube/iomanager.txt");
}

void IOManager::resetWithStage(Game &game, Field &field, Player &player, Player &enemy) {
    IOManager::game = &game;
    IOManager::field = &field;
    IOManager::player = &player;
    IOManager::enemy = &enemy;
    ofs = ofstream("/Users/matscube/iomanager.txt");
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

int IOManager::inputStage() {
    // Input game information
    nextInt(); // timelimit;
    return nextInt();
}

void IOManager::inputBody() {
    game->currentTurn = nextInt();
    player->resourceCount = nextInt();
//    cerr << "Current Stage, Turn: " << game->currentStage << ", " << game->currentTurn << endl;
    
    // Input PlayerUnit Information
    player->resetWithTurn();
    enemy->resetWithTurn();
    field->resetWithTurn();
    int playerUnitCount = nextInt();
    for (int i = 0; i < playerUnitCount; i++) {
        int unitID = nextInt();
        int unitPosY = nextInt();
        int unitPosX = nextInt();
        int unitHP = nextInt();
        int unitType = nextInt();

        PlayerUnit unit = PlayerUnit(unitID, Position(unitPosX, unitPosY), PlayerUnitType(unitType), player);
        unit.setHitPoint(unitHP);

        field->updateWithPlayerUnit(&unit);
        player->updateUnit(unit);
    }
    
    // Input EnemyUnit Information
    int enemyUnitCount = nextInt();
    vector<PlayerUnit> enemyUnits;
    for (int i = 0; i < enemyUnitCount; i++) {
        int unitID = nextInt();
        int unitPosY = nextInt();
        int unitPosX = nextInt();
        int unitHP = nextInt();
        int unitType = nextInt();
        
        PlayerUnit unit = PlayerUnit(unitID, Position(unitPosX, unitPosY), PlayerUnitType(unitType), enemy);
        unit.setHitPoint(unitHP);
        
        field->updateWithPlayerUnit(&unit);
        enemy->updateUnit(unit);
        enemyUnits.push_back(unit);
    }
    enemy->updateCastleMode(enemyUnits);
    
    // Input ResourceUnit Information
    int resourceCount; cin >> resourceCount;
    for (int i = 0; i < resourceCount; i++) {
        int resourcePosY = nextInt();
        int resourcePosX = nextInt();

        ResourceUnit unit = ResourceUnit(Position(resourcePosX, resourcePosY));
        field->updateWithResourceUnit(unit);
    }
    
    field->updateResourceStatus();
    
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
    vector<Command> resCommands;
    
    vector<Command>::iterator ite;
    for (ite = commands.begin(); ite != commands.end(); ite++) {
        if (ite->second == PlayerUnitActionType::None) {
//            cerr << "[IOManager::output] Warning: 'None' action is called." << endl;
            continue;
        } else {
            resCommands.push_back(*ite);
        }
    }
    
    ofs << "Turn: " << game->currentTurn << endl;
    ofs << "Resource: " << player->resourceCount << endl;
    
    ofs << "Input: " << endl;
    
    int sz = (int)resCommands.size();
    cout << sz << endl; cout.flush();
    ofs << sz << endl;
    
    for (ite = resCommands.begin(); ite != resCommands.end(); ite++) {
        if (ite->second == PlayerUnitActionType::None) {
            cerr << "[IOManager::output] Error: 'None' action is included in output." << endl;
            continue;
        }
        cout << ite->first << " " << PlayerUnit::action(ite->second) << endl; cout.flush();
        
        ofs << ite->first << " " << PlayerUnit::action(ite->second) << endl; ofs.flush();
    }
    ofs << endl;
}

void IOManager::testOutput() {
    cout << 0 << endl;
    cout.flush();
}
