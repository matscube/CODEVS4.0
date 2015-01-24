//
//  ExtraAI_Quick.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/24.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "ExtraAI.h"

// MARK: new
vector<Position> ExtraAI::backAttackMoveLineRight() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 40);
        line.push_back(p);
    }
    for (int y = 40; y < MAX_FIELD_HEIGHT - 40; y++) {
        Position p(MAX_FIELD_WIDTH - 1, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::backAttackMoveLineDown() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(40, y);
        line.push_back(p);
    }
    for (int x = 40; x < MAX_FIELD_WIDTH - 40; x++) {
        Position p(x, MAX_FIELD_WIDTH - 1);
        line.push_back(p);
    }
    return line;
}
void ExtraAI::backAttackMoveRightCommand() {
    searchNoVisitedAreaCommand(backAttackMoveLineRight(), 1, workerTypes());
}
void ExtraAI::backAttackMoveDownCommand() {
    searchNoVisitedAreaCommand(backAttackMoveLineDown(), 1, workerTypes());
}
vector<PlayerUnit *> ExtraAI::attackBases() {
    vector<PlayerUnit *> attackBases;
    
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        int d = utl::dist(Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1), base->position);
        if (d <= 40) {
            attackBases.push_back(base);
        }
        //        int x = base->position.first;
        //        int y = base->position.second;
        //        if (x == MAX_FIELD_WIDTH - 1 || y == MAX_FIELD_HEIGHT - 1) count++;
    }
    return attackBases;
}
int ExtraAI::calcAttackBase() {
    vector<PlayerUnit *> bases = attackBases();
    return (int)bases.size();
}
void ExtraAI::createOneAttackerBase() {
    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, PlayerUnit *> > downWorkers; // <dist, worker>
    vector<pair<int, PlayerUnit *> > rightWorkers; // <dist, worker>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        if (!uIte->second.isMovable()) continue;
        
        if (uIte->second.position.first == MAX_FIELD_WIDTH - 1) {
            int d = MAX_FIELD_HEIGHT - 1 - uIte->second.position.second;
            rightWorkers.push_back(make_pair(d, &uIte->second));
        } else if (uIte->second.position.second == MAX_FIELD_HEIGHT - 1) {
            int d = MAX_FIELD_WIDTH - 1 - uIte->second.position.first;
            downWorkers.push_back(make_pair(d, &uIte->second));
        }
    }
    
    vector<pair<int, PlayerUnit *> > workers;
    workers.insert(workers.end(), downWorkers.begin(), downWorkers.end());
    workers.insert(workers.end(), rightWorkers.begin(), rightWorkers.end());
    sort(workers.begin(), workers.end());
    
    vector<pair<int, PlayerUnit *> >::iterator wIte;
    for (wIte = workers.begin(); wIte != workers.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        if (!worker->isCreatableBase()) continue;
        if (utl::dist(worker->position, Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1)) > 40) continue;
        
        addCommandCreateBase(worker);
        break;
    }
    
    sort(downWorkers.begin(), downWorkers.end());
    sort(rightWorkers.begin(), rightWorkers.end());
    
    for (wIte = rightWorkers.begin(); wIte != rightWorkers.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        if (!worker->isMovable()) continue;
        addCommandMove(worker, PlayerUnitActionType::MoveDown);
    }
    for (wIte = downWorkers.begin(); wIte != downWorkers.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        if (!worker->isMovable()) continue;
        addCommandMove(worker, PlayerUnitActionType::MoveRight);
    }
}
void ExtraAI::createOneMoreAttackerBase() {
    map<int, PlayerUnit>::iterator uIte;
    PlayerUnit *targetBase = nullptr;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        if (base->position.first == MAX_FIELD_WIDTH - 1 || base->position.second == MAX_FIELD_HEIGHT - 1) {
            targetBase= base;
        }
    }
    
    if (targetBase == nullptr) {
        cerr << "Error base is nowhere." << endl;
        return;
    }
    
    PlayerUnit *targetWorker = nullptr;
    int dToBase = INF;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        int d = utl::dist(worker->position, targetBase->position);
        
        if (d != 0 && !worker->isMovable()) continue;
        
        if (d < dToBase) {
            dToBase = d;
            targetWorker = worker;
        }
    }
    
    if (targetWorker == nullptr) {
        cerr << "Warning: missing commandable worker." << endl;
        return;
    }
    
    if (dToBase == 0) {
        if (targetWorker->isCreatableBase()) {
            addCommandCreateBase(targetWorker);
        } else {
            targetWorker->fixOnlyPosition();
        }
    } else {
        addCommandMove(targetWorker, targetBase->position);
    }
}

void ExtraAI::createAttackerCommand() {
    vector<PlayerUnitType> types;
    for (int i = 0; i < 10; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < 50; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < 40; i++) types.push_back(PlayerUnitType::Assassin);
    
    vector<PlayerUnit *> bases = attackBases();
    vector<PlayerUnit *>::iterator uPIte;
    for (uPIte = bases.begin(); uPIte != bases.end(); uPIte++) {
        PlayerUnit *base = *uPIte;
        
        PlayerUnitType pType = types[rand() % 100];
        if (!base->isCreatableAttacker(pType)) continue;
        addCommandCreateAttacker(base, pType);
    }
}

void ExtraAI::attackCastleCommand() {
    Position target = enemy->castle.position;
    
    map<int, PlayerUnit *>::iterator uPIte;
    for (uPIte = player->attackers.begin(); uPIte != player->attackers.end(); uPIte++) {
        
        PlayerUnit *pUnit = uPIte->second;
        if (!pUnit->isMovable()) continue;
        addCommandMove(pUnit, target, true, true);
    }
}

void ExtraAI::poolAttackerCommand(int need) {
    vector<PlayerUnit *> bases = attackBases();
    if (bases.size() == 0) {
//        cerr << "Base Size : 0" << endl;
        return;
    }
    PlayerUnit *base = bases[0];
    
    // Count pool count
    Position center = base->position;
    int poolCountSingleRange = 4;
    int poolCount = 0;
    map<int, PlayerUnit *>::iterator uIte;
    for (uIte = player->attackers.begin(); uIte != player->attackers.end(); uIte++) {
        PlayerUnit *attacker = uIte->second;
        int dS = utl::distSingle(attacker->position, center);
        if (dS <= poolCountSingleRange) poolCount++;
    }
    cerr << "PoolCount: " << poolCount << endl;
    if (poolCount >= need) {
        cerr << "Release" << endl;
        return;
    } else {
        cerr << "Pooling" << endl;
    }
    
    
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
    
    int poolTargetCount = (need + 9) / 10;
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
  
    // Select Attacker
    int poolTargetSingleRange = 1;
    vector<PlayerUnit *> attackers;
    for (uIte = player->attackers.begin(); uIte != player->attackers.end(); uIte++) {
        PlayerUnit *attacker = uIte->second;
        int distSToBase = utl::distSingle(attacker->position, base->position);
        if (distSToBase > poolTargetSingleRange) continue;
        attackers.push_back(attacker);
    }
    
    // Assign attacker to pool cell
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

// MARK: Worker
void ExtraAI::supplyWorkerForSearchCommand(int need) {
    map<int, PlayerUnit>::iterator uIte;
    int searchWorkerCount = 0;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        int hashID = utl::getHashID(worker->position);
        if (field->resources.find(hashID) == field->resources.end()) {
            searchWorkerCount++;
        }
    }
    
    Position target = Position(50, 50);
    vector<pair<int, PlayerUnit *> > distToTarget; // <d, p>
    
    int castleDist = utl::dist(player->castle.position, target);
    distToTarget.push_back(make_pair(castleDist, &player->castle));
    for (uIte = player->villages.begin(); uIte != player->villages.end(); uIte++) {
        int d = utl::dist(uIte->second.position, target);
        distToTarget.push_back(make_pair(d, &uIte->second));
    }
    
    sort(distToTarget.begin(), distToTarget.end());
    
    vector<pair<int, PlayerUnit *> >::iterator dIte;
    for (dIte = distToTarget.begin(); dIte != distToTarget.end(); dIte++) {
        if (!dIte->second->isCreatableWorker()) continue;
        
        if (need > searchWorkerCount) {
            PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
            Command com(dIte->second->ID, at);
            addCommand(com);
            dIte->second->fix(at);
            need--;
        }
    }
}


