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
void ExtraAI::addCommandCreateBase(PlayerUnit *pUnit) {
    PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
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
void ExtraAI::addCommandCreateAttacker(PlayerUnit *pUnit, PlayerUnitType unitType) {
    PlayerUnitActionType at = CreateAttackerAction(unitType);
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}

// MARK: Defend -----------------------------------------------------
int ExtraAI::defenderBaseCount(Position position) {
    int count = 0;
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        if (utl::dist(position, base->position) == 0) count++;
    }
    return count;
    
}

int ExtraAI::createDefenderBaseCommand(Position position, int assign, int prob) {
    int create = 0;
    if (rand() % 100 >= prob) return create;
    if (defenderBaseCount(position)) return create;

    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, PlayerUnit *> > workerDists; // <dist, worker>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        if (!worker->isMovable()) continue;
        
        int d = utl::dist(position, worker->position);
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
            if (worker->isCreatableBase()) {
                addCommandCreateBase(worker);
                create++;
            } else {
                worker->fixOnlyPosition();
            }
        } else {
            addCommandMove(worker, position);
        }
        currentAssign++;
    }
    
    return create;
}
int ExtraAI::createCastleDefenderCommand(Position position, int assign, int prob) {
    int create = 0;
    if (rand() % 100 >= prob) return create;
    
    vector<PlayerUnitType> attackerTypes = attackerTypesPack(0, 60, 40);
    
    int currentAssign = 0;
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        if (currentAssign >= assign) break;
        
        PlayerUnit *base = &uIte->second;
        if (utl::dist(base->position, position) != 0) continue;

        PlayerUnitType type = attackerTypes[rand() % 100];
        if (!base->isCreatableAttacker(type)) continue;
        addCommandCreateAttacker(base, type);
        create++;
        currentAssign++;
    }
    
    return create;
}

void ExtraAI::defendCastleCommand(int assign) {
    Position target = player->castle.position;
    map<int, PlayerUnit *>::iterator uIte;
    vector<pair<int, PlayerUnit *> > attackerDists; // <d, attaker>
    for (uIte = player->attackers.begin(); uIte != player->attackers.end(); uIte++) {
        PlayerUnit *attacker = uIte->second;
        int d = utl::dist(attacker->position, target);

        attackerDists.push_back(make_pair(d, attacker));
    }
    
    sort(attackerDists.begin(), attackerDists.end());
    
    int currentAssign = 0;
    vector<pair<int, PlayerUnit *> >::iterator aIte;
    for (aIte = attackerDists.begin(); aIte != attackerDists.end(); aIte++) {
        if (currentAssign >= assign) break;
        int d = aIte->first;
        PlayerUnit *attacker = aIte->second;
        
        if (d == 0) {
            if (!attacker->isMovable()) continue;
            attacker->fixOnlyPosition();
            currentAssign++;
        } else {
            if (!attacker->isMovable()) continue;
            addCommandMove(attacker, target);
            currentAssign++;
        }
    }
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
vector<Position> ExtraAI::searchLineToRight6() {
    vector<Position> line;
    for (int x = 45; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 49);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight7() {
    vector<Position> line;
    for (int x = 54; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 58);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight8() {
    vector<Position> line;
    for (int x = 63; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 67);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight9() {
    vector<Position> line;
    for (int x = 72; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 76);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight10() {
    vector<Position> line;
    for (int x = 81; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 85);
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
vector<Position> ExtraAI::searchLineToDown6() {
    vector<Position> line;
    for (int y = 45; y < MAX_FIELD_HEIGHT; y++) {
        Position p(49, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown7() {
    vector<Position> line;
    for (int y = 54; y < MAX_FIELD_HEIGHT; y++) {
        Position p(58, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown8() {
    vector<Position> line;
    for (int y = 63; y < MAX_FIELD_HEIGHT; y++) {
        Position p(67, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown9() {
    vector<Position> line;
    for (int y = 72; y < MAX_FIELD_HEIGHT; y++) {
        Position p(76, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown10() {
    vector<Position> line;
    for (int y = 81; y < MAX_FIELD_HEIGHT; y++) {
        Position p(85, y);
        line.push_back(p);
    }
    return line;
}

vector<Position> ExtraAI::searchLineAlly1() {
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
vector<Position> ExtraAI::searchLineAlly2() {
    vector<Position> line;
    for (int x = 0; x < 40; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    for (int x = 10; x < 40; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    for (int x = 10; x < 40; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    for (int x = 10; x < 40; x++) {
        Position p(x, 31);
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
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly1(), 1, allTypes());
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
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly1(), 1, allTypes());
}
void ExtraAI::searchUnkownFieldAllCommand(int assign) {
    int currentAssign = 0;
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight5(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight6(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight7(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight8(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight9(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight10(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown5(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown6(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown7(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown8(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown9(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown10(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly1(), 1, allTypes());
//    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly2(), 1, allTypes());
}

// MARK: Attacker --------------------------------------------------------
vector<PlayerUnitType> ExtraAI::attackerTypesPack(int knight, int fighter, int assasin) {
    vector<PlayerUnitType> types;
    for (int i = 0; i < knight; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < fighter; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < assasin; i++) types.push_back(PlayerUnitType::Assassin);
    return types;
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

int ExtraAI::createVillageOnResourceCommand(int assign, int prob) {
    int create = 0;

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
    
    int currentAssign = 0;
    vector<pair<int, pair<int, int> > >::iterator dIte;
    map<int, bool> resWillHaveVillage; // <resID, villageOK>
    for (dIte = dToRes.begin(); dIte != dToRes.end(); dIte++) {
        if (currentAssign >= assign) break;
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
            if (rand() % 100 < prob) {
                addCommandCreateVillage(pUnit);
                create++;
            } else {
                pUnit->fixOnlyPosition();
            }
            resWillHaveVillage[res->hashID] = true;
        } else {
            if (!pUnit->isMovable()) continue;
            // move to village
            addCommandMove(pUnit, res->position);
            resWillHaveVillage[res->hashID] = true;
        }
        currentAssign++;
    }
    return create;
}

int ExtraAI::getResourceCommand(int prob) {
    int create = 0;
    if (rand() % 100 >= prob) return create;

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
            addCommandCreateWorker(&uIte->second);
            create++;
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
    return create;
}

// MARK: temp --------------------------------------------------------
int ExtraAI::supplyFreeWorkerWithVillageCommand(int need, int prob) {
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


// MARK: Debug --------------------------------------------------------
void ExtraAI::debug() {
    
}

