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
    ofs = ofstream("/Users/matscube/hoge.txt");
    fieldOfs = ofstream("/Users/matscube/AIfield.txt");
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

vector<Command> AI::getResourceCommand(int assign) {
    vector<Command> commands;

    map<int, FieldUnit>::iterator resIte;
    map<int, PlayerUnit>::iterator pUnitIte;
    vector<pair<int, pair<int, int> > > dists; // (dist, (resID, unitID))
    for (resIte = field->resources.begin(); resIte != field->resources.end(); resIte++) {
        for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
            if (pUnitIte->second.type != PlayerUnitType::Worker) continue;
            
            int d = dist(resIte->second.x, resIte->second.y, pUnitIte->second.x, pUnitIte->second.y);
            dists.push_back(make_pair(d, make_pair(resIte->second.hashID, pUnitIte->second.ID)));
        }
    }

    sort(dists.begin(), dists.end());
    
    int curAssign = 0;
    for (int i = 0; i < dists.size(); i++) {
        int unitID = dists[i].second.second;
        int resID = dists[i].second.first;
        PlayerUnit *pUnit = &player->units[unitID];
        FieldUnit *res = &field->resources[resID];

        if (res->occupancy >= MAX_GETTING_RESOURCE) continue;
        if (!pUnit->isCommandable()) continue;
        
        int d = dists[i].first;
        if (d != 0) {
            Command com(pUnit->ID, pUnit->moveToTargetAction(res->x, res->y));
            commands.push_back(com);
        }
        res->occupancy++;
        pUnit->setReserved();

        curAssign++;
        if (curAssign >= assign) break;
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

vector<Command> AI::searchResourceCommand(int assign) {
    vector<Command> commands;

    int curAssign = 0;
    map<int, PlayerUnit>::iterator unitIte = player->units.begin();
    for (; unitIte != player->units.end(); unitIte++) {
        PlayerUnit *unit = &unitIte->second;

        if (!unit->isCommandable()) continue;
        if (!unit->isMovable()) continue;

        for (int d = 5; d < 100; d++) {
            for (int dx = -d; dx <= d; dx++) {
                int dy = (d - abs(dx));
                int multi = (rand() % 2) ? 1 : -1;
                dy *= multi;
                
                int x = unit->x + dx;
                int y = unit->y + dy;
                
/*                if (isValidIndex(x, y)) {
                    ofs << "d: " << d << " dx:" << dx << " x:" << x << " y:" << y << " isVisited:" << field->willBeVisited[x][y] << endl;
                }*/
                
                if (isValidIndex(x, y) && !field->willBeVisited[x][y]) {
                    Command com(unit->ID, unit->moveToTargetAction(x, y));
                    commands.push_back(com);
                    unit->setReserved();
                    field->willBeVisited[x][y] = true;

                    curAssign++;
                    if (curAssign >= assign) goto finishAssign;

                    goto breakLoop;
                }

                y = unit->y - dy;
                
/*                if (isValidIndex(x, y)) {
                    ofs << "d: " << d << " dx:" << dx << " x:" << x << " y:" << y << " isVisited:" << field->willBeVisited[x][y] << endl;
                }*/

                if (isValidIndex(x, y) && !field->willBeVisited[x][y]) {
                    Command com(unit->ID, unit->moveToTargetAction(x, y));
                    commands.push_back(com);
                    unit->setReserved();
                    field->willBeVisited[x][y] = true;
                    
                    curAssign++;
                    if (curAssign >= assign) goto finishAssign;

                    goto breakLoop;
                }
            }
        }
        breakLoop:
        continue;
    }
    finishAssign:
    
    return commands;
}


bool AI::isSearchable() {
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            if (!field->isVisited[x][y]) return true;
        }
    }
    return false;
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
    
    
#if 0
    int visitedCnt = 0;
    for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
        for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
//            if (field->isVisited[x][y]) {
            if (field->willBeVisited[x][y]) {
                fieldOfs << "#";
                visitedCnt++;
            } else {
                fieldOfs << ".";
            }
        }
        fieldOfs << endl;
    }
//    cerr << visitedCnt << endl;

#endif
    
#if 0
    fieldOfs << "Field********" << endl;
    map<int, FieldUnit>::iterator fUIte = field->resources.begin();
    for (; fUIte != field->resources.end(); fUIte++) {
        fieldOfs << fUIte->first << " " << fUIte->second.hashID << " " << fUIte->second.x << " " << fUIte->second.y << endl;
    }
    
#endif
}

