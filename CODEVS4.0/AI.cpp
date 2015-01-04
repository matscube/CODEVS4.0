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

void AI::setResourceLimit(int n) {
    player->necessaryResourceCount = n;
}

void AI::releaseResourceLimit() {
    player->necessaryResourceCount = 0;
}

vector<Command> AI::createWorkerCommand(int assign) {
    vector<Command> commands;

    int curAssign = 0;
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (!pUnitIte->second.isCreatableWorker()) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
        
        curAssign++;
        if (curAssign >= assign) break;
    }
    
    return commands;
}
vector<Command> AI::createVillageCommand(int assign) {
    vector<Command> commands;
    
    int curAssign = 0;
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (!pUnitIte->second.isCreatableVillage()) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateVillage;
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
        
        curAssign++;
        if (curAssign >= assign) break;
    }
    
    return commands;
}
vector<Command> AI::createBaseCommand(int assign) {
    vector<Command> commands;
    
    int curAssign = 0;
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (!pUnitIte->second.isCreatableBase()) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
        
        curAssign++;
        if (curAssign >= assign) break;
    }
    
    return commands;
}

// Create only assasin
vector<Command> AI::createAttakerCommand(int assign) {
    vector<Command> commands;
    
    int curAssign = 0;
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {

        if (!pUnitIte->second.isCreatableAttacker(PlayerUnitType::Assassin)) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateAssassin;
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
        
        curAssign++;
        if (curAssign >= assign) break;
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
        if (!pUnit->isMovable()) continue;
        
        int d = dists[i].first;
        if (d != 0) {
            PlayerUnitActionType at = pUnit->moveToTargetAction(res->x, res->y);
            Command com(pUnit->ID, at);
            commands.push_back(com);
            pUnit->fix(at);
        } else {
            pUnit->fixOnlyPosition();
        }
        res->occupancy++;

        curAssign++;
        if (curAssign >= assign) break;
    }

    return commands;
}

vector<Command> AI::getMinimumResourceCommand(int assign) {
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
        
        if (res->occupancy >= 1) continue;
        if (!pUnit->isMovable()) continue;
        
        int d = dists[i].first;
        if (d != 0) {
            PlayerUnitActionType at = pUnit->moveToTargetAction(res->x, res->y);
            Command com(pUnit->ID, at);
            commands.push_back(com);
            pUnit->fix(at);
        } else {
            // create village
            pUnit->fixOnlyPosition();
        }
        res->occupancy++;
        
        curAssign++;
        if (curAssign >= assign) break;
    }
    
    return commands;
}

vector<Command> AI::createVillageOnResource(int assign) {
    vector<Command> commands;
    
    map<int, int> villageCount; // <hashID, count>
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (pUnitIte->second.type == PlayerUnitType::Village) {
            int hashID = FieldUnit::getHashID(pUnitIte->second.x, pUnitIte->second.y);
            if (villageCount.find(hashID) != villageCount.end()) {
                villageCount[hashID]++;
            } else {
                villageCount[hashID] = 0;
            }
        }
    }
    
    int curAssign = 0;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        int hashID = FieldUnit::getHashID(pUnitIte->second.x, pUnitIte->second.y);
        if (field->resources.find(hashID) != field->resources.end()) {
            if (!pUnitIte->second.isCreatableVillage()) continue;
            if (villageCount.find(hashID) == villageCount.end()) {
                PlayerUnitActionType at = PlayerUnitActionType::CreateVillage;
                Command com(pUnitIte->second.ID, at);
                commands.push_back(com);
                pUnitIte->second.fix(at);
                
                curAssign++;
                if (curAssign >= assign) break;
            }
        }
    }
    return commands;
}

vector<Command> AI::createWorkerOnResource(int assign) {
    vector<Command> commands;
    
    map<int, int> workerCount; // <hashID, count>
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (pUnitIte->second.type == PlayerUnitType::Worker) {
            int hashID = FieldUnit::getHashID(pUnitIte->second.x, pUnitIte->second.y);
            if (workerCount.find(hashID) != workerCount.end()) {
                workerCount[hashID]++;
            } else {
                workerCount[hashID] = 1;
            }
        }
    }

    int curAssign = 0;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        int hashID = FieldUnit::getHashID(pUnitIte->second.x, pUnitIte->second.y);
        if (field->resources.find(hashID) != field->resources.end()) {
            if (!pUnitIte->second.isCreatableWorker()) continue;
            
            bool flg = false;
            if (workerCount.find(hashID) == workerCount.end()) flg = true;
            else if (workerCount[hashID] < MAX_GETTING_RESOURCE) flg = true;

            if (flg) {
                PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
                Command com(pUnitIte->second.ID, at);
                commands.push_back(com);
                pUnitIte->second.fix(at);

                curAssign++;
                if (curAssign >= assign) break;
            }
        }
    }
    return commands;
}

// MARK: create base

Position AI::basePointNearestToEnemy() {
    Position target;
    if (player->type == PlayerType::Ally) {
        target = Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1);
    } else {
        target = Position(0, 0);
    }
    if (isValidIndex(field->enemyCastlePosition.first, field->enemyCastlePosition.second)) {
        target = field->enemyCastlePosition;
    }

    Position bestPos;
    int distToCastle = INF;
    
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {

        if (pUnitIte->second.type != PlayerUnitType::Village) continue;

        int hashID = FieldUnit::getHashID(pUnitIte->second.x, pUnitIte->second.y);
        if (field->resources.find(hashID) == field->resources.end()) continue;

        int d = dist(pUnitIte->second.x, pUnitIte->second.y, target.first, target.second);
        if (d < distToCastle) {
            bestPos = Position(pUnitIte->second.x, pUnitIte->second.y);
            distToCastle = d;
        }
    }
    
    return bestPos;
}

int AI::calcDistanceToEnemy(Position p) {
    Position target;
    if (player->type == PlayerType::Ally) {
        target = Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1);
    } else {
        target = Position(0, 0);
    }
    
    if (isValidIndex(field->enemyCastlePosition.first, field->enemyCastlePosition.second)) {
        target = field->enemyCastlePosition;
    }
    
    return dist(p.first, p.second, target.first, target.second);
}

vector<Command> AI::createBaseOnNearestEnemy() {
    vector<Command> commands;

    Position basePos = basePointNearestToEnemy();
    if (calcDistanceToEnemy(basePos) > 40 && player->calcVillageCount() < 12) return commands;
    
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {

        if (!pUnitIte->second.isCreatableBase()) continue;
        if (dist(pUnitIte->second.x, pUnitIte->second.y, basePos.first, basePos.second) != 0) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
        break;
    }
    
    return commands;
}

vector<Command> AI::randomWalkCommand() {
    vector<Command> commands;

    map<int, PlayerUnit>::iterator unitIte = player->units.begin();
    for (; unitIte != player->units.end(); unitIte++) {
        PlayerUnit *unit = &unitIte->second;
        if (!unit->isMovable()) continue;
        
        vector<int> prob;
        for (int i = 0; i < 35; i++) prob.push_back(0);
        for (int i = 0; i < 35; i++) prob.push_back(1);
        for (int i = 0; i < 15; i++) prob.push_back(2);
        for (int i = 0; i < 15; i++) prob.push_back(3);
        
        int p = prob[rand() % 100];
        PlayerUnitActionType at;
        if (p == 0) {
            at = PlayerUnitActionType::MoveDown;
            Command com(unit->ID, at);
            commands.push_back(com);
        } else if (p == 1) {
            at = PlayerUnitActionType::MoveRight;
            Command com(unit->ID, at);
            commands.push_back(com);
        } else if (p == 2) {
            at = PlayerUnitActionType::MoveUp;
            Command com(unit->ID, at);
            commands.push_back(com);
        } else {
            at = PlayerUnitActionType::MoveLeft;
            Command com(unit->ID, at);
            commands.push_back(com);
        }
        unit->fix(at);
    }
    return commands;
}

vector<Command> AI::searchResourceNearestCommand(int assign) {
    vector<Command> commands;

    vector<Position> viewRange = viewRangePositions(PlayerUnit::viewRange(PlayerUnitType::Worker));
    vector<Position>::iterator viewRangeIte;

    int curAssign = 0;
    map<int, PlayerUnit>::iterator unitIte = player->units.begin();
    for (; unitIte != player->units.end(); unitIte++) {
        PlayerUnit *unit = &unitIte->second;

        if (!unit->isMovable()) continue;
        if (!isSearchable()) break;

        for (int d = 5; d < 100; d++) {
            vector<Position> positions = framePositions(d, true);
            vector<Position>::iterator posIte;
            for (posIte = positions.begin(); posIte != positions.end(); posIte++) {
                int x = unit->x + posIte->first;
                int y = unit->y + posIte->second;

                if (isValidIndex(x, y) && !field->willBeVisited[x][y]) {
                    PlayerUnitActionType at = unit->moveToTargetAction(x, y);
                    Command com(unit->ID, at);
                    commands.push_back(com);
                    unit->fix(at);
                    field->willBeVisited[x][y] = true;
                    
                    for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {
                        int x2 = x + viewRangeIte->first;
                        int y2 = y + viewRangeIte->second;
                        if (isValidIndex(x2, y2))
                            field->willBeVisited[x2][y2] = true;
                    }
                    
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

vector<Command> AI::searchResourceWithRangeCommand(int assign, int depth) {
    vector<Command> commands;

    if (!isSearchable()) {
        cerr << "Searchable field is nowhere." << endl;
        return commands;
    }
    
    vector<Position> viewRange = viewRangePositions(PlayerUnit::viewRange(PlayerUnitType::Worker));
    vector<Position>::iterator viewRangeIte;
    
    int curAssign = 0;
    map<int, PlayerUnit>::iterator pUnitIte;
    
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        PlayerUnit *pUnit = &pUnitIte->second;
        
        if (!pUnit->isMovable()) continue;
        
        int maxNewCellCount = 0;
        Position target;

        for (int d = depth; d < MAX_FIELD_WIDTH; d++) {

            vector<Position> positions = framePositions(d, true);
            vector<Position>::iterator posIte;
            
            for (posIte = positions.begin(); posIte != positions.end(); posIte++) {

                int newCellCount = 0;
                int baseX = pUnit->x + posIte->first;
                int baseY = pUnit->y + posIte->second;

                if (!isValidIndex(baseX, baseY)) continue;

                for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {

                    int x = baseX + viewRangeIte->first;
                    int y = baseY + viewRangeIte->second;
                    if (isValidIndex(x, y) && !field->willBeVisited[x][y]) {
                        newCellCount++;
                    }
                }

                if (newCellCount > maxNewCellCount) {
                    maxNewCellCount = newCellCount;
                    target = Position(baseX, baseY);
                }
            }
            
            if (maxNewCellCount) break;
        }

        if (maxNewCellCount) {
            PlayerUnitActionType at = pUnit->moveToTargetAction(target.first, target.second);
            Command com(pUnit->ID, at);
            commands.push_back(com);
            pUnit->fix(at);
            
            for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {
                int x = target.first + viewRangeIte->first;
                int y = target.second + viewRangeIte->second;
                if (isValidIndex(x, y))
                    field->willBeVisited[x][y] = true;
            }
            
            curAssign++;
            if (curAssign >= assign) break;
        }
    }
    
    return commands;
}

// MARK: defend castle
vector<Command> AI::setWorkerOnCastle() {
    vector<Command> commands;
    
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (pUnitIte->second.type != PlayerUnitType::Worker) continue;
        if (dist(pUnitIte->second.x, pUnitIte->second.y, field->allyCastlePosition.first, field->allyCastlePosition.second) == 0) return commands;
    }
    
    // no worker, then create by castle
    PlayerUnit *castle = &player->units[player->castleUnitID];
    
    if (castle->isCreatableWorker()) {
        PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
        Command com(castle->ID, at);
        commands.push_back(com);
        castle->fix(at);
    }
    
    return commands;
}

vector<Command> AI::createBaseOnCastle() {
    vector<Command> commands;
    
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (dist(pUnitIte->second.x, pUnitIte->second.y, field->allyCastlePosition.first, field->allyCastlePosition.second) != 0) continue;
        if (!pUnitIte->second.isCreatableBase()) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
    }
    
    return commands;
}

vector<Position> AI::defendingArea() {
    vector<Position> area = areaPositions(2, true);
    vector<Position>::iterator areaIte;
    vector<Position> defArea;
    for (areaIte = area.begin(); areaIte != area.end(); areaIte++) {
        Position pos = Position(areaIte->first + field->allyCastlePosition.first, areaIte->second + field->allyCastlePosition.second);
        if (isValidIndex(pos.first, pos.second)) {
            defArea.push_back(pos);
        }
    }
    return defArea;
}

bool AI::isBaseReady() {
    bool isReady = false;
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (dist(pUnitIte->second.x, pUnitIte->second.y, field->allyCastlePosition.first, field->allyCastlePosition.second) != 0) continue;
        if (pUnitIte->second.type == PlayerUnitType::Base) isReady = true;
    }
    
    return  isReady;
}

vector<Command> AI::createDefenderOnCastle() {
    vector<Command> commands;
    
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (dist(pUnitIte->second.x, pUnitIte->second.y, field->allyCastlePosition.first, field->allyCastlePosition.second) != 0) continue;
        if (pUnitIte->second.type != PlayerUnitType::Base) continue;
        
        // base on castle
        PlayerUnitType pType = PlayerUnitType::Knight;
        if (pUnitIte->second.isCreatableAttacker(pType)) {
            PlayerUnitActionType at = CreateAttackerAction(pType);
            Command com(pUnitIte->second.ID, at);
            commands.push_back(com);
            pUnitIte->second.fix(at);
        }
    }
    
    return commands;
}

vector<Command> AI::setDefenderOnCastle() {
    vector<Command> commands;
    
    vector<Position> targets = defendingArea();
    int targetSize = (int)targets.size();
    
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (dist(pUnitIte->second.x, pUnitIte->second.y, field->allyCastlePosition.first, field->allyCastlePosition.second) >= 10) continue;
        // TODO: experiment value
        if (!pUnitIte->second.isAttacker()) continue;
        if (!pUnitIte->second.isMovable()) continue;
        
        Position target = targets[rand() % targetSize];
        PlayerUnitActionType at = pUnitIte->second.moveToTargetAction(target.first, target.second);
        Command com(pUnitIte->second.ID, at);
        commands.push_back(com);
        pUnitIte->second.fix(at);
    }
    
    return commands;
}


// MARK: attack castle
vector<Command> AI::searchEnemyCastle(int assign) {
    vector<Command> commands;

    map<int, Position> targets = field->enemyCastlePositions(player->type);

    map<int, Position>::iterator targetIte;
    map<int, PlayerUnit>::iterator pUnitIte;
    vector<pair<int, pair<int, int> > > dists; // (dist, (targetID, unitID))
    for (targetIte = targets.begin(); targetIte != targets.end(); targetIte++) {
        for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
            if (!pUnitIte->second.isMovable()) continue;
            if (field->willBeVisited[targetIte->second.first][targetIte->second.second]) continue;
            if (field->isVisited[targetIte->second.first][targetIte->second.second]) continue;
            
            int d = dist(targetIte->second.first, targetIte->second.second, pUnitIte->second.x, pUnitIte->second.y);
            dists.push_back(make_pair(d, make_pair(targetIte->first, pUnitIte->second.ID)));
        }
    }
    
    sort(dists.begin(), dists.end());
    
    map<int, bool> willBeVisitedTarget;
    int curAssign = 0;
    for (int i = 0; i < dists.size(); i++) {
        int unitID = dists[i].second.second;
        int targetID = dists[i].second.first;

        PlayerUnit *pUnit = &player->units[unitID];
        Position pos = targets[targetID];

        if (willBeVisitedTarget.find(targetID) != willBeVisitedTarget.end()) continue;
        if (!pUnit->isMovable()) continue;
        
        int d = dists[i].first;
        if (d != 0) {
            PlayerUnitActionType at = pUnit->moveToTargetAction(pos.first, pos.second);
            Command com(pUnit->ID, at);
            commands.push_back(com);
            pUnit->fix(at);
            willBeVisitedTarget[targetID] = true;
        }
        
        curAssign++;
        if (curAssign >= assign) break;
    }
    
    return commands;
}

vector<Command> AI::attackCastleCommand(int assign) {
    vector<Command> commands;
    
    int curAssign = 0;
    map<int, PlayerUnit>::iterator pUnitIte;
    for (pUnitIte = player->units.begin(); pUnitIte != player->units.end(); pUnitIte++) {
        if (pUnitIte->second.isMovable()) {
            PlayerUnitActionType at = pUnitIte->second.moveToTargetAction(field->enemyCastlePosition.first, field->enemyCastlePosition.second);
            Command com(pUnitIte->second.ID, at);
            commands.push_back(com);
            pUnitIte->second.fix(at);
            
            curAssign++;
            if (curAssign >= assign) break;
        }
    }
    
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

