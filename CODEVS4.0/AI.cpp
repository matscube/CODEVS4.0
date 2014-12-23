//
//  AI.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "AI.h"


AI::AI(Game &game, Field &field, Player &player) {
    AI::game = &game;
    AI::field = &field;
    AI::player = &player;
}

void AI::resetWithTurn() {
    commands.clear();
}

vector<Command> AI::getCommands() {
    return commands;
}

void AI::addCommands(vector<Command> newCommands) {
    commands.insert(commands.begin(), newCommands.begin(), newCommands.end());
}

vector<Command> AI::createWorkerCommand() {
    vector<Command> commands;
    // TODO : find castleID
    int castleID = 0;
    PlayerUnit *castleUnit = &player->units[castleID];

    if (castleUnit->isCommandable()) {
        // TODO : make isCreatable func
        if (player->resourceCount >= PlayerUnit::cost(PlayerUnitType::Worker)) {
            Command com(castleID, PlayerUnitActionType::CreateWorker);
            commands.push_back(com);
        }
    }
    
    return commands;
}

void AI::fixWorkerOnResource() {
    map<int, PlayerUnit>::iterator pUnitIte = player->units.begin();
    for (; pUnitIte != player->units.end(); pUnitIte++) {
        PlayerUnit *pUnit = &pUnitIte->second;
        
        if (field->status[pUnit->x][pUnit->y] == FieldStatus::Resource
            && field->reservedWorkers[pUnit->x][pUnit->y] < 5 ) {
            
            field->reservedWorkers[pUnit->x][pUnit->y]++;
            pUnit->setReserved();
        }
    }
}

vector<Command> AI::getResourceCommand() {
    vector<Command> commands;

    map<int, FieldUnit>::iterator resIte = field->resources.begin();
    for (; resIte != field->resources.end(); resIte++) {
        FieldUnit *fUnit = &resIte->second;

        int rest = 5 - field->allyWorkers[fUnit->x][fUnit->y];
        if (rest <= 0) continue;
        
        map<int, PlayerUnit>::iterator pUnitIte = player->units.begin();
        for (; pUnitIte != player->units.end(); pUnitIte++) {
            PlayerUnit *pUnit = &pUnitIte->second;
            
            if (!pUnit->isMovable()) continue;
            if (!pUnit->isCommandable()) continue;
            
            PlayerUnitActionType action = pUnit->moveToTargetAction(fUnit->x, fUnit->y);
            if (action != PlayerUnitActionType::None) {
                Command com(pUnit->ID, action);
                commands.push_back(com);
                pUnit->setReserved();
            }
        }
    }
    return commands;
}



vector<Command> AI::randomWalkCommand() {
    vector<Command> commands;

    map<int, PlayerUnit>::iterator unitIte = player->units.begin();
    for (; unitIte != player->units.end(); unitIte++) {
        PlayerUnit *unit = &unitIte->second;
        if (!unit->isMovable()) continue;
        if (!unit->isCommandable()) continue;
        
        vector<int> prob;
        for (int i = 0; i < 35; i++) prob.push_back(0);
        for (int i = 0; i < 35; i++) prob.push_back(1);
        for (int i = 0; i < 15; i++) prob.push_back(2);
        for (int i = 0; i < 15; i++) prob.push_back(3);
        
        int p = prob[rand() % 100];
        if (p == 0) {
            Command com(unit->ID, PlayerUnitActionType::MoveDown);
            commands.push_back(com);
        } else if (p == 1) {
            Command com(unit->ID, PlayerUnitActionType::MoveRight);
            commands.push_back(com);
        } else if (p == 2) {
            Command com(unit->ID, PlayerUnitActionType::MoveUp);
            commands.push_back(com);
        } else {
            Command com(unit->ID, PlayerUnitActionType::MoveLeft);
            commands.push_back(com);
        }
        unit->setReserved();
    }
    return commands;
}

vector<Command> AI::searchResourceCommand() {
    vector<Command> commands;
    
    map<int, PlayerUnit>::iterator unitIte = player->units.begin();
    for (; unitIte != player->units.end(); unitIte++) {
        PlayerUnit *unit = &unitIte->second;
        if (unit->type == PlayerUnitType::Castle) {
            continue;
        }

        if (unit->status == PlayerUnitStatus::Idle) {
            if (rand() % 2) {
                Command com(unit->ID, PlayerUnitActionType::MoveDown);
                commands.push_back(com);
            } else {
                Command com(unit->ID, PlayerUnitActionType::MoveRight);
                commands.push_back(com);
            }
                
        }
    }
    
    return commands;
}

void AI::debug() {
#if 0
    // Player
    cerr << "-----begin PlayerUnit debug-----" << endl;
    map<int, PlayerUnit> units = player->units;
    map<int, PlayerUnit>::iterator unitIte = units.begin();
    for (; unitIte != units.end(); unitIte++) {
        PlayerUnit unit = unitIte->second;
        cerr << unit.ID << " " << unit.x << " " << unit.y << " " << PlayerUnitTypeName(unit.type) << " " << PlayerUnitStatusName(unit.status) << endl;
    }
    cerr << "-----end debug-----" << endl;
#endif

#if 0
    // Field
    cerr << "-----begin FieldUnit debug-----" << endl;
    map<int, FieldUnit> resources = field->resources;
    cerr << "Visible Resource count: " << resources.size() << endl;
    cerr << "-----end debug-----" << endl;
#endif
}

