//
//  ExtraAI.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/17.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "ExtraAI.h"

ExtraAI::ExtraAI(Game &game, Field &field, Player &player, Player &enemy) {
    ExtraAI::game = &game;
    ExtraAI::field = &field;
    ExtraAI::player = &player;
    ExtraAI::enemy = &enemy;
    
    resetWithStage();
}

void ExtraAI::resetWithTurn() {
    commands.clear();
}
void ExtraAI::resetWithStage() {
    commands.clear();
}

vector<Command> ExtraAI::getCommands() {
    return commands;
}
void ExtraAI::addCommand(Command newCommand) {
    commands.push_back(newCommand);
}

// MARK: Basic Command ----------------------------------------------
void ExtraAI::addCommandMove(PlayerUnit *pUnit, Position target) {
    PlayerUnitActionType at = pUnit->moveToTargetAction(target);
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}

void ExtraAI::addCommandCreateVillage(PlayerUnit *pUnit) {
    PlayerUnitActionType at = PlayerUnitActionType::CreateVillage;
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}
void ExtraAI::addCommandCreateWorker(PlayerUnit *pUnit) {
    PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}

// MARK: Defend -----------------------------------------------------
int ExtraAI::defenderVillageCount() {
    int count = 0;
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->villages.begin(); uIte != player->villages.end(); uIte++) {
        PlayerUnit *village = &uIte->second;
        if (utl::dist(defenderVillagePosition, village->position) == 0) count++;
    }
    return count;
    
}

int ExtraAI::createDefenderVillageCommand(int assign, int prob) {
    int create = 0;
    if (rand() % 100 >= prob) return create;

    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, PlayerUnit *> > workerDists; // <dist, worker>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        if (!worker->isMovable()) continue;
        
        int d = utl::dist(defenderVillagePosition, worker->position);
        workerDists.push_back(make_pair(d, worker));
        
    }
    
    sort(workerDists.begin(), workerDists.end());
    
    int currentAssign = 0;
    vector<pair<int, PlayerUnit *> >::iterator wIte;
    for (wIte = workerDists.begin(); wIte != workerDists.end(); wIte++) {
        if (currentAssign >= assign) break;

        PlayerUnit *worker = wIte->second;
        int d = wIte->first;
        
        if (d == 0) {
            if (worker->isCreatableVillage()) {
                addCommandCreateVillage(worker);
                create++;
            } else {
                worker->fixOnlyPosition();
            }
        } else {
            addCommandMove(worker, defenderVillagePosition);
        }
        currentAssign++;
    }
    
    return create;
}

// MARK: Search -------------------------------------------------------
vector<Position> ExtraAI::searchLineToRight1() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight2() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight3() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight4() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 31);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight5() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 40);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown1() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(4, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown2() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(13, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown3() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(22, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown4() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(31, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown5() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(40, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineAlly() {
    vector<Position> line;
    for (int x = 0; x < 40; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    for (int y = 9; y < 40; y++) {
        Position p(4, y);
        line.push_back(p);
    }
    return line;
}

void ExtraAI::searchNoVisitedAreaCommand(vector<Position> area, int assign, map<PlayerUnitType, bool> types) {
    
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
        if (currentAssign >= assign) break;

        PlayerUnit *pUnit = player->units[dIte->second.first];
        Position pos = dIte->second.second;
        
        if (!pUnit->isMovable()) continue;
        if (field->willBeVisited[pos.first][pos.second]) continue;
        
        addCommandMove(pUnit, pos);
        currentAssign++;
        
        for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {
            int x = pos.first + viewRangeIte->first;
            int y = pos.second + viewRangeIte->second;
            if (isValidIndex(Position(x, y)))
                field->willBeVisited[x][y] = true;
        }
    }
}
map<PlayerUnitType, bool> ExtraAI::allTypes() {
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
map<PlayerUnitType, bool> ExtraAI::workerTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Worker] = true;
    return res;
}
map<PlayerUnitType, bool> ExtraAI::attackerTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Knight] = true;
    res[PlayerUnitType::Fighter] = true;
    res[PlayerUnitType::Assassin] = true;
    return res;
}

void ExtraAI::searchUnkownFieldSmallCommand(int assign) {
    int currentAssign = 0;
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly(), 1, allTypes());
}
void ExtraAI::searchUnkownFieldMediumCommand(int assign) {
    int currentAssign = 0;
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight5(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown5(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly(), 1, allTypes());
}

// MARK: Resource --------------------------------------------------------
int ExtraAI::calcResourceGetting() {
    int resourceGetting = 10;
    map<int, int> workerCount; // <hashID, workerCount>
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        int hashID = utl::getHashID(uIte->second.position.first, uIte->second.position.second);
        if (field->resources.find(hashID) != field->resources.end()) {
            if (workerCount.find(hashID) == workerCount.end()) {
                resourceGetting++;
                workerCount[hashID] = 1;
            } else if (workerCount[hashID] < 5) {
                resourceGetting++;
                workerCount[hashID]++;
            } else {
                // no count
            }
        }
    }
    return resourceGetting;
}

int ExtraAI::supplyFreeWorkerCommand(int need, int prob) {
    int workerCount = player->calcWorkerCount();
    int create = 0;
    if (rand() % 100 >= prob) return create;
    
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->villages.begin(); uIte != player->villages.end(); uIte++) {
        PlayerUnit *village = &uIte->second;
        if (workerCount >= need) break;
        if (!village->isCreatableWorker()) continue;
        addCommandCreateWorker(village);
        need--;
        create++;
    }
    return create;
}
void ExtraAI::getResourceCommand(int assign) {
    // calc worker count on resource
    map<int, PlayerUnit>::iterator uIte;
    map<int, FieldUnit>::iterator fIte;
    vector<pair<int, pair<PlayerUnit *, Position> > > workerDists; // <d, <worker, target> >
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        for (fIte = field->resources.begin(); fIte != field->resources.end(); fIte++) {
            Position resPos = fIte->second.position;
            int d = utl::dist(worker->position, resPos);
            workerDists.push_back(make_pair(d, make_pair(worker, resPos)));
        }
    }
    
    sort(workerDists.begin(), workerDists.end());

    int currentAssign = 0;
    int workerCount[MAX_FIELD_CELL_COUNT] = {0}; // <hashID, worker cnt>
    vector<pair<int, pair<PlayerUnit *, Position> > >::iterator wIte;
    for (wIte = workerDists.begin(); wIte != workerDists.end(); wIte++) {
        if (currentAssign >= assign) break;

        int d = wIte->first;
        PlayerUnit *worker = wIte->second.first;
        Position target = wIte->second.second;
        int hashID = utl::getHashID(target.first, target.second);
        
        if (workerCount[hashID] >= MAX_GETTING_RESOURCE) continue;
        
        if (d == 0) {
            if (!worker->isMovable()) {
                // no command
            } else {
                worker->fixOnlyPosition();
            }
        } else {
            if (!worker->isMovable()) continue;
            addCommandMove(worker, target);
        }
        workerCount[hashID]++;
        currentAssign++;
    }
}

// MARK: Debug --------------------------------------------------------
void ExtraAI::debug() {
    
}

