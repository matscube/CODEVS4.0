//
//  ExtraAI_Custom.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/21.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "ExtraAI.h"


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

int ExtraAI::createCastleBaseCommand(int prob) {
    PlayerUnit *castle = &player->castle;

    int create = 0;
    if (rand() % 100 >= prob) return create;
    if (defenderBaseCount(castle->position)) return create;
    
    // Worker, if needed, create
    PlayerUnit *castleWorker; // = nullptr;
    bool flg = false;
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        if (worker->isMovable() && utl::dist(worker->position, castle->position) == 0) {
            castleWorker = worker;
            flg = true;
            break;
        }
    }

    // create worker
    if (flg) {
//    if (castleWorker == nullptr) {
        if (castle->isCreatableWorker()) {
            addCommandCreateWorker(castle);
        }
        return create; // wait for next turn
    }
    
    // create base
    if (castleWorker->isCreatableBase()) {
        addCommandCreateBase(castleWorker);
        create++;
    } else {
        castleWorker->fixOnlyPosition(); // wait for resource getting
    }
    
    return create;
}
int ExtraAI::createBaseCommand(Position position, int prob) {
    int create = 0;

    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, PlayerUnit *> > workerDists; // <dist, worker>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        int d = utl::dist(worker->position, position);
        
        workerDists.push_back(make_pair(d, worker));
    }
    
    sort(workerDists.begin(), workerDists.end());
    
    vector<pair<int, PlayerUnit *> >::iterator wIte;
    for (wIte = workerDists.begin(); wIte != workerDists.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        int d = wIte->first;
        
        if (d == 0) {
            if (worker->isCreatableBase()) {
                addCommandCreateBase(worker);
                create++;
            } else if (worker->isMovable()) {
                worker->fixOnlyPosition();
            }
        } else {
            if (!worker->isMovable()) continue;
            addCommandMove(worker, position);
        }
        break;
    }
    
    return create;
}

int ExtraAI::createDefenderCommand(Position position, int assign, int prob) {
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

int ExtraAI::calcCastleDefender() {
    int create = 0;
    map<int, PlayerUnit *>::iterator uPIte;
    for (uPIte = player->attackers.begin(); uPIte != player->attackers.end(); uPIte++) {
        int d = utl::dist(player->castle.position, uPIte->second->position);
        if (d <= PlayerUnit::viewRange(PlayerUnitType::Castle)) create++;
    }
    return create;
}
void ExtraAI::defendCastleCommand(int assign) {
    Position center = player->castle.position;
    
    // Select pool cell
    vector<pair<int, Position> > poolTargetDists;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            Position pos(center.first + dx, center.second + dy);
            if (utl::isValidPosition(pos)) {
                int d = utl::dist(pos, enemy->castle.position);
                poolTargetDists.push_back(make_pair(d, pos));
            }
        }
    }
    sort(poolTargetDists.begin(), poolTargetDists.end());
    
    int poolTargetCount = (assign + 9) / 10;
    int currentPoolTargetCount = 0;
    vector<Position> poolTargets;
    vector<pair<int, Position> >::iterator pIte;
    for (pIte = poolTargetDists.begin(); pIte != poolTargetDists.end(); pIte++) {
        if (currentPoolTargetCount < poolTargetCount) {
            poolTargets.push_back(pIte->second);
            currentPoolTargetCount++;
        } else {
            break;
        }
    }
    
    // Select Defender
    int poolTargetRange = PlayerUnit::viewRange(PlayerUnitType::Castle);
    map<int, PlayerUnit* >::iterator uPIte;
    vector<PlayerUnit *> attackers;
    for (uPIte = player->attackers.begin(); uPIte != player->attackers.end(); uPIte++) {
        PlayerUnit *attacker = uPIte->second;
        int d = utl::dist(attacker->position, center);
        if (d > poolTargetRange) continue;
        attackers.push_back(attacker);
    }
    
    // Assign defender to pool cell
    vector<PlayerUnit *>::iterator aIte;
    vector<Position>::iterator tIte;
    vector<pair<int, pair<PlayerUnit *, Position> > > poolAttackers;
    for (aIte = attackers.begin(); aIte != attackers.end(); aIte++) {
        PlayerUnit *attacker = *aIte;
        for (tIte = poolTargets.begin(); tIte != poolTargets.end(); tIte++) {
            Position target = *tIte;
            int d = utl::dist(attacker->position, target);
            poolAttackers.push_back(make_pair(d, make_pair(attacker, target)));
        }
    }
    sort(poolAttackers.begin(), poolAttackers.end());
    
    int poolCountField[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT] = {0};
    vector<pair<int, pair<PlayerUnit *, Position> > >::iterator paIte;
    for (paIte = poolAttackers.begin(); paIte != poolAttackers.end(); paIte++) {
        PlayerUnit *attacker = paIte->second.first;
        Position target = paIte->second.second;
        int d = paIte->first;
        
        if (poolCountField[target.first][target.second] >= 10) continue;
        if (d == 0) {
            if (attacker->isMovable()) {
                attacker->fixOnlyPosition();
                poolCountField[target.first][target.second]++;
            } else {
                poolCountField[target.first][target.second]++;
            }
        } else {
            if (attacker->isMovable()) {
                addCommandMove(attacker, target);
                poolCountField[target.first][target.second]++;
            }
        }
    }
}

// MARK: Enemy -----------------------------------------------------------
void ExtraAI::updateEnemy() {
    map<int, PlayerUnit *>::iterator uPIte;
    for (uPIte = enemy->units.begin(); uPIte != enemy->units.end(); uPIte++) {
        Position pos = uPIte->second->position;
        int d = utl::dist(pos, player->castle.position);
        if (nearestEnemyDistance > d) nearestEnemyDistance = d;
        
        if (d <= PlayerUnit::viewRange(PlayerUnitType::Castle)) {
            enemyCountToAllyCaslte++;
        }
    }
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
        int hashID = utl::getHashID(uIte->second.position);
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
    map<int, ResourceUnit>::iterator resIte;
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
        ResourceUnit *res = &field->resources[dIte->second.second];
        
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
    map<int, ResourceUnit>::iterator resIte;
    for (uIte = player->villages.begin(); uIte != player->villages.end(); uIte++) {
        PlayerUnit *village = &uIte->second;
        int hashID = village->getHashID();
        int count = 0;
        if (workerCount.find(hashID) != workerCount.end()) {
            count = workerCount[hashID];
        }
        
        if (workerCreationCount[village->position.first][village->position.second] > 9) continue;
        
        if (count < MAX_GETTING_RESOURCE) {
            if (!village->isCreatableWorker()) continue;
            addCommandCreateWorker(village);
            create++;
            
            workerCreationCount[village->position.first][village->position.second]++;
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
void ExtraAI::defendResourceCommand(int assign) {
    map<int, ResourceUnit>::iterator rIte;
    map<int, ResourceUnit *> targets;
    for (rIte = field->resources.begin(); rIte != field->resources.end(); rIte++) {
        ResourceUnit *resource = &rIte->second;
        if (resource->status == ResourceUnitStatus::Enemy) {
            targets[resource->hashID] = resource;
        }
    }
    
    vector<pair<int, pair<PlayerUnit *, Position> > > attackerDists; // <d, <unit, pos> >
    map<int, PlayerUnit *>::iterator uPIte;
    map<int, ResourceUnit *>::iterator tIte;
    for (uPIte = player->attackers.begin(); uPIte != player->attackers.end(); uPIte++) {
        PlayerUnit *attacker = uPIte->second;
        for (tIte = targets.begin(); tIte != targets.end(); tIte++) {
            ResourceUnit *res = tIte->second;
            
            int d = utl::dist(attacker->position, res->position);
            attackerDists.push_back(make_pair(d, make_pair(attacker, res->position)));
        }
    }
    
    sort(attackerDists.begin(), attackerDists.end());
    
    int currentAssign = 0;
    int need = 5;
    int willBeVisited[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT] = {0};
    vector<pair<int, pair<PlayerUnit *, Position> > >::iterator dIte;
    for (dIte = attackerDists.begin(); dIte != attackerDists.end(); dIte++) {
        int d = dIte->first;
        PlayerUnit *attacker = dIte->second.first;
        Position target = dIte->second.second;
        
        if (currentAssign >= assign) break;
        if (willBeVisited[target.first][target.second] >= need) continue;
        
        if (d == 0) {
            if (attacker->isMovable()) {
                attacker->fixOnlyPosition();
            }
        } else {
            if (!attacker->isMovable()) continue;
            addCommandMove(attacker, target);
        }
        currentAssign++;
        willBeVisited[target.first][target.second]++;
    }
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



