//
//  QuickAI.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/08.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "QuickAI.h"

QuickAI::QuickAI(Game &game, Field &field, Player &player, Player &enemy) {
    QuickAI::game = &game;
    QuickAI::field = &field;
    QuickAI::player = &player;
    QuickAI::enemy = &enemy;
}

void QuickAI::resetWithTurn() {
    commands.clear();
}

vector<Command> QuickAI::getCommands() {
    return commands;
}
void QuickAI::addCommand(Command newCommand) {
    commands.push_back(newCommand);
}
void QuickAI::addCommands(vector<Command> newCommands) {
    commands.insert(commands.end(), newCommands.begin(), newCommands.end());
}

// MARK: Search
vector<Position> QuickAI::searchLine1() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine2() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine3() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine4() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 31);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine5() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 40);
        line.push_back(p);
    }
    return line;
}

vector<Position> QuickAI::searchArea() {
    vector<Position> area;
    vector<Position> line1 = searchLine1();
    vector<Position> line2 = searchLine2();
    vector<Position> line3 = searchLine3();
    vector<Position> line4 = searchLine4();
    vector<Position> line5 = searchLine5();
    area.insert(area.end(), line1.begin(), line1.end());
    area.insert(area.end(), line2.begin(), line2.end());
    area.insert(area.end(), line3.begin(), line3.end());
    area.insert(area.end(), line4.begin(), line4.end());
    area.insert(area.end(), line5.begin(), line5.end());
    
    return area;
}
vector<Position> QuickAI::createBaseOnLine() {
    vector<Position> line;
    for (int y = 50; y < MAX_FIELD_HEIGHT; y++) {
        Position p(99, y);
        line.push_back(p);
    }
    return line;
}
void QuickAI::searchNoVisitedAreaCommand(vector<Position> area, int assign, map<PlayerUnitType, bool> types) {
    vector<Command> commands;
    
    map<int, PlayerUnit *>::iterator uIte;
    vector<Position>::iterator areaIte;
    vector<pair<int, pair<int, Position> > > dToLine; // <d, <unitID, Position>>
    for (uIte = player->units.begin(); uIte != player->units.end(); uIte++) {
        if (types.find(uIte->second->type) == types.end()) continue;
        if (!uIte->second->isMovable()) continue;
        
        for (areaIte = area.begin(); areaIte != area.end(); areaIte++) {
            if (field->willBeVisited[areaIte->first][areaIte->second]) continue;

            int d = utl::dist(uIte->second->position, *areaIte);
            if (d > 0) {
                dToLine.push_back(make_pair(d, make_pair(uIte->second->ID, *areaIte)));
            }
        }
    }
    
    sort(dToLine.begin(), dToLine.end());
    
    vector<Position> viewRange = utl::viewRangePositions(PlayerUnit::viewRange(PlayerUnitType::Worker));
    vector<Position>::iterator viewRangeIte;

    int currentAssign = 0;
    vector<pair<int, pair<int, Position> > >::iterator dIte;
    for (dIte = dToLine.begin(); dIte != dToLine.end(); dIte++) {
        PlayerUnit *pUnit = player->units[dIte->second.first];
        Position pos = dIte->second.second;
        
        if (!pUnit->isMovable()) continue;
        if (field->willBeVisited[pos.first][pos.second]) continue;
        
        PlayerUnitActionType at = pUnit->moveToTargetAction(pos);
        Command com(pUnit->ID, at);
        commands.push_back(com);
        pUnit->fix(at);
        
        for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {
            int x = pos.first + viewRangeIte->first;
            int y = pos.second + viewRangeIte->second;
            if (isValidIndex(Position(x, y)))
                field->willBeVisited[x][y] = true;
        }
        
        if (++currentAssign >= assign) break;
    }
    
    addCommands(commands);
}
map<PlayerUnitType, bool> QuickAI::allTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Castle] = true;
    res[PlayerUnitType::Worker] = true;
    res[PlayerUnitType::Knight] = true;
    res[PlayerUnitType::Fighter] = true;
    res[PlayerUnitType::Assassin] = true;
    res[PlayerUnitType::Village] = true;
    res[PlayerUnitType::Base] = true;
    return res;
}
map<PlayerUnitType, bool> QuickAI::workerTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Worker] = true;
    return res;
}
map<PlayerUnitType, bool> QuickAI::attackerTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Knight] = true;
    res[PlayerUnitType::Fighter] = true;
    res[PlayerUnitType::Assassin] = true;
    return res;
}

void QuickAI::searchUnkownFieldCommand() {
    searchNoVisitedAreaCommand(searchLine1(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLine2(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLine3(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLine4(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLine5(), 1, allTypes());
}
void QuickAI::createBaseOnLineCommand() {
    vector<Position> line = createBaseOnLine();
    vector<Position>::iterator pIte;
    bool isOnLine[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT] = {false};
    for (pIte = line.begin(); pIte != line.end(); pIte++) {
        isOnLine[pIte->first][pIte->second] = true;
    }
    
    // Worker creatable worker on line
    map<int, PlayerUnit>::iterator uIte;
    PlayerUnit *bestUnit = nullptr;
    int distToTarget = INF;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        if (isOnLine[uIte->second.position.first][uIte->second.position.second]) {
            int d = utl::dist(uIte->second.position, Position(MAX_FIELD_WIDTH, MAX_FIELD_HEIGHT));
            if (d < distToTarget) {
                d = distToTarget;
                bestUnit = &uIte->second;
            }
        }
    }
    
    bool baseIsNeeded = true;
    int baseCount = 0;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        if (isOnLine[uIte->second.position.first][uIte->second.position.second]) {
            baseCount++;
        }
    }
    
    if (baseCount > 5) baseIsNeeded = false;
    
    if (baseIsNeeded && bestUnit != nullptr && bestUnit->isCreatableBase()) {
        PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
        Command com(bestUnit->ID, at);
        addCommand(com);
        bestUnit->fix(at);
    } else {
        searchNoVisitedAreaCommand(createBaseOnLine(), 1, allTypes());
    }
}

void QuickAI::createAttackerOnLineCommand() {
    vector<Position> line = createBaseOnLine();
    vector<Position>::iterator pIte;
    bool isOnLine[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT] = {false};
    for (pIte = line.begin(); pIte != line.end(); pIte++) {
        isOnLine[pIte->first][pIte->second] = true;
    }
    
    map<int, PlayerUnit>::iterator uIte;
    vector<PlayerUnit *> baseUnits;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        if (isOnLine[uIte->second.position.first][uIte->second.position.second]) {
            baseUnits.push_back(&uIte->second);
        }
    }
  
    vector<PlayerUnitType> types;
    for (int i = 0; i < 20; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < 50; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < 30; i++) types.push_back(PlayerUnitType::Assassin);
    PlayerUnitType pType = types[rand() % 100];
    
    vector<PlayerUnit *>::iterator bIte;
    for (bIte = baseUnits.begin(); bIte != baseUnits.end(); bIte++) {
        PlayerUnit *pUnit = *bIte;
        if (!pUnit->isCreatableAttacker(pType)) continue;

        PlayerUnitActionType at = CreateAttackerAction(pType);
        Command com(pUnit->ID, at);
        addCommand(com);
        pUnit->fix(at);
    }

}

vector<Position> QuickAI::searchEnemyCastleLine() {
    vector<Position> line;
    for (int y = 59; y < MAX_FIELD_HEIGHT; y++) {
        Position p(95, y);
        line.push_back(p);
    }
    for (int y = 69; y < MAX_FIELD_HEIGHT; y++) {
        Position p(86, y);
        line.push_back(p);
    }
    for (int x = 59; x < MAX_FIELD_HEIGHT; x++) {
        Position p(x, 95);
        line.push_back(p);
    }
    for (int x = 69; x < MAX_FIELD_HEIGHT; x++) {
        Position p(x, 86);
        line.push_back(p);
    }
    Position p(81, 81);
    line.push_back(p);

    return line;
}

void QuickAI::searchEnemyCastleCommand() {
    searchNoVisitedAreaCommand(searchEnemyCastleLine(), 1, attackerTypes());
}

void QuickAI::attackCastleCommand() {
    Position target = enemy->castle.position;

    map<int, PlayerUnit *>::iterator uPIte;
    for (uPIte = player->attackers.begin(); uPIte != player->attackers.end(); uPIte++) {
        PlayerUnit *pUnit = uPIte->second;
        if (!pUnit->isMovable()) continue;
        
        PlayerUnitActionType at = pUnit->moveToTargetAction(target);
        Command com(pUnit->ID, at);
        addCommand(com);
        pUnit->fix(at);
    }
}
// MARK: Resource
void QuickAI::createVillageOnResourceCommand() {
    vector<Command> commands;
    
    map<int, PlayerUnit>::iterator uIte;
    map<int, bool> isVillage; // <hashID, exist>
    for (uIte = player->villages.begin(); uIte != player->villages.end(); uIte++) {
        int hashID = uIte->second.getHashID();
        isVillage[hashID] = true;
    }
    
    // no village resource, assign worker
    map<int, FieldUnit>::iterator resIte;
    vector<pair<int, pair<int, int> > > dToRes; // <dist, <unitID, resID>>
    for (resIte = field->resources.begin(); resIte != field->resources.end(); resIte++) {
        if (isVillage.find(resIte->second.hashID) != isVillage.end()) continue; // resource has village
        
        for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
            int d = utl::dist(uIte->second.position, resIte->second.position);
            dToRes.push_back(make_pair(d, make_pair(uIte->second.ID, resIte->second.hashID)));
        }
    }
    
    sort(dToRes.begin(), dToRes.end());
    
    vector<pair<int, pair<int, int> > >::iterator dIte;
    map<int, bool> resWillHaveVillage; // <resID, villageOK>
    for (dIte = dToRes.begin(); dIte != dToRes.end(); dIte++) {
        PlayerUnit *pUnit = &player->workers[dIte->second.first];
        FieldUnit *res = &field->resources[dIte->second.second];
        
        if (resWillHaveVillage.find(res->hashID) != resWillHaveVillage.end()) continue;
        
        if (dIte->first == 0) {
            if (!pUnit->isCreatableVillage()) {
                pUnit->fixOnlyPosition(); // wait
                resWillHaveVillage[res->hashID] = true;
                continue;
            }
            // create village
            PlayerUnitActionType at = PlayerUnitActionType::CreateVillage;
            Command com(pUnit->ID, at);
            commands.push_back(com);
            pUnit->fix(at);
            resWillHaveVillage[res->hashID] = true;
        } else {
            if (!pUnit->isMovable()) continue;
            // move to village
            PlayerUnitActionType at = pUnit->moveToTargetAction(res->position);
            Command com(pUnit->ID, at);
            commands.push_back(com);
            pUnit->fix(at);
            resWillHaveVillage[res->hashID] = true;
        }
    }
    
    addCommands(commands);
}

void QuickAI::fixResourceCommand() {
    vector<Command> commands;
    
    // calc worker count on resource
    map<int, int> workerCount; // <hashID, worker cnt>
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        int hashID = uIte->second.getHashID();
        if (workerCount.find(hashID) == workerCount.end()) {
            workerCount[hashID] = 1;
        } else {
            workerCount[hashID]++;
        }
    }
    
    // create worker on village/resource
    map<int, FieldUnit>::iterator resIte;
    for (uIte = player->villages.begin(); uIte != player->villages.end(); uIte++) {
        int hashID = uIte->second.getHashID();
        int count = 0;
        if (workerCount.find(hashID) != workerCount.end()) {
            count = workerCount[hashID];
        }
        
        if (count < MAX_GETTING_RESOURCE) {
            if (!uIte->second.isCreatableWorker()) continue;
            PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
            Command com(uIte->second.ID, at);
            commands.push_back(com);
            uIte->second.fix(at);
        }
    }
    
    // fix worker on vilalge/resource
    map<int, int> fixedWorkerCount; // <res hashID, worker cnt>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        int hashID = uIte->second.getHashID();
        if (field->resources.find(hashID) != field->resources.end()) {
            
            if (fixedWorkerCount.find(hashID) == fixedWorkerCount.end()) {
                fixedWorkerCount[hashID] = 1;
                uIte->second.fixOnlyPosition();
            } else {
                if (fixedWorkerCount[hashID] < MAX_GETTING_RESOURCE) {
                    fixedWorkerCount[hashID]++;
                    uIte->second.fixOnlyPosition();
                }
            }
        }
    }
    
    addCommands(commands);
}

// MARK: Worker
void QuickAI::supplyWorker(int need) {
    
}

void QuickAI::debug() {
    
}
