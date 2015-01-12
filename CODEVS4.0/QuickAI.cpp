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

// MARK: Search-----------------------

// MARK: searchLine
vector<Position> QuickAI::searchLineToRight1() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToRight2() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToRight3() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToRight4() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 31);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToRight5() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 40);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToDown1() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(4, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToDown2() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(13, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToDown3() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(22, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToDown4() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(31, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineToDown5() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(40, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLineAlly() {
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

void QuickAI::searchUnkownFieldSmallCommand() {
    searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToRight5(), 1, allTypes());

    searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToDown5(), 1, allTypes());
    
    searchNoVisitedAreaCommand(searchLineAlly(), 1, allTypes());
}
void QuickAI::searchUnkownFieldMediumCommand() {
    searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToRight4(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToRight5(), 1, allTypes());
    
    searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToDown4(), 1, allTypes());
    searchNoVisitedAreaCommand(searchLineToDown5(), 1, allTypes());
    
    searchNoVisitedAreaCommand(searchLineAlly(), 1, allTypes());
}

void QuickAI::createBaseOnRightLine() {
    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, PlayerUnit *> > workerOnLine; // <MAXY - y , unit>
    vector<pair<int, PlayerUnit *> > workerOutLine; // <MAXX - x + MaxY - y, unit>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        int d = MAX_FIELD_WIDTH - 1 - uIte->second.position.first;
        int fromMaxY = MAX_FIELD_HEIGHT - 1 - uIte->second.position.second;

        if (d == 0) {
            workerOnLine.push_back(make_pair(fromMaxY, &uIte->second));
        } else {
            workerOutLine.push_back(make_pair(d + fromMaxY, &uIte->second));
        }
    }
    
    int baseCount = 0;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        int d = MAX_FIELD_HEIGHT - 1 - uIte->second.position.first;
        if (d == 0) baseCount++;
    }
    
    vector<pair<int, PlayerUnit *> >::iterator wIte;

    int targetWorkerCount = 2;
    int workerCount = (int)workerOnLine.size();
    sort(workerOutLine.begin(), workerOutLine.end());
    for (wIte = workerOutLine.begin(); wIte != workerOutLine.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        
        if (workerCount >= targetWorkerCount) break;
        
        if (!worker->isMovable()) continue;
        PlayerUnitActionType at = PlayerUnitActionType::MoveRight;
        Command com(worker->ID, at);
        addCommand(com);
        worker->fix(at);
        workerCount++;
    }
    
    int targetBaseCount = 1;
    int basePointTH = 80;
    workerCount = 0;
    sort(workerOnLine.begin(), workerOnLine.end());
    for (wIte = workerOnLine.begin(); wIte != workerOnLine.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        
        if (workerCount >= targetWorkerCount) break;

        // create base
        if (worker->position.second >= basePointTH && targetBaseCount > baseCount) {
            if (!worker->isCreatableBase()) continue;
            PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
            Command com(worker->ID, at);
            addCommand(com);
            worker->fix(at);
            baseCount++;
            workerCount++;
            continue;
        // move down
        } else if (worker->position.second < MAX_FIELD_HEIGHT - 1) {
            if (!worker->isMovable()) continue;
            PlayerUnitActionType at = worker->moveToTargetAction(Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1));
            Command com(worker->ID, at);
            addCommand(com);
            worker->fix(at);
            workerCount++;
        } else {
            // on (99, 99)
        }
    }
    
}
void QuickAI::createAttackerOnRightLineCommand() {
    vector<PlayerUnitType> types;
    for (int i = 0; i < 10; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < 50; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < 40; i++) types.push_back(PlayerUnitType::Assassin);

    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        if (base->position.first == MAX_FIELD_WIDTH - 1) {

            PlayerUnitType pType = types[rand() % 100];
            if (!base->isCreatableAttacker(pType)) continue;

            PlayerUnitActionType at = CreateAttackerAction(pType);
            Command com(base->ID, at);
            addCommand(com);
            base->fix(at);
        }
    }
}
void QuickAI::createBaseOnDownLine() {
    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, PlayerUnit *> > workerOnLine; // <MaxyX - x , unit>
    vector<pair<int, PlayerUnit *> > workerOutLine; // <MaxY - y + MaxX - x, unit>
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        int d = MAX_FIELD_HEIGHT - 1 - uIte->second.position.second;
        int fromMaxX = MAX_FIELD_WIDTH - 1 - uIte->second.position.first;
        
        if (d == 0) {
            workerOnLine.push_back(make_pair(fromMaxX, &uIte->second));
        } else {
            workerOutLine.push_back(make_pair(d + fromMaxX, &uIte->second));
        }
    }
    
    int baseCount = 0;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        int d = MAX_FIELD_WIDTH - 1 - uIte->second.position.second;
        if (d == 0) baseCount++;
    }
    
    vector<pair<int, PlayerUnit *> >::iterator wIte;
    
    int targetWorkerCount = 2;
    int workerCount = (int)workerOnLine.size();
    sort(workerOutLine.begin(), workerOutLine.end());
    for (wIte = workerOutLine.begin(); wIte != workerOutLine.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        
        if (workerCount >= targetWorkerCount) break;
        
        if (!worker->isMovable()) continue;
        PlayerUnitActionType at = PlayerUnitActionType::MoveDown;
        Command com(worker->ID, at);
        addCommand(com);
        worker->fix(at);
        workerCount++;
    }
    
    int targetBaseCount = 1;
    int basePointTH = 80;
    workerCount = 0;
    sort(workerOnLine.begin(), workerOnLine.end());
    for (wIte = workerOnLine.begin(); wIte != workerOnLine.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        
        if (workerCount >= targetWorkerCount) break;
        
        // create base
        if (worker->position.first >= basePointTH && targetBaseCount > baseCount) {
            if (!worker->isCreatableBase()) continue;
            PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
            Command com(worker->ID, at);
            addCommand(com);
            worker->fix(at);
            baseCount++;
            workerCount++;
            continue;
            // move down
        } else if (worker->position.first < MAX_FIELD_WIDTH - 1) {
            if (!worker->isMovable()) continue;
            PlayerUnitActionType at = worker->moveToTargetAction(Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1));
            Command com(worker->ID, at);
            addCommand(com);
            worker->fix(at);
            workerCount++;
        } else {
            // on (99, 99)
        }
    }
}

void QuickAI::createAttackerOnDownLineCommand() {
    vector<PlayerUnitType> types;
    for (int i = 0; i < 10; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < 50; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < 40; i++) types.push_back(PlayerUnitType::Assassin);
    
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        if (base->position.second == MAX_FIELD_HEIGHT - 1) {
            
            PlayerUnitType pType = types[rand() % 100];
            if (!base->isCreatableAttacker(pType)) continue;
            
            PlayerUnitActionType at = CreateAttackerAction(pType);
            Command com(base->ID, at);
            addCommand(com);
            base->fix(at);
        }
    }
}
void QuickAI::poolAttackerOnBaseCommand() {
    map<int, PlayerUnit *>::iterator uPIte;
    map<int, PlayerUnit>::iterator uIte;
    
    vector<pair<int, pair<PlayerUnit *, PlayerUnit *> > > distToBase; // <dist, <base, attacke>>
    map<int, int> attackerCount; // <baseID, attackerCount>
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        for (uPIte = player->attackers.begin(); uPIte != player->attackers.end(); uPIte++) {
            PlayerUnit *attacker = uPIte->second;
            
            int d = utl::dist(attacker->position, base->position);
            distToBase.push_back(make_pair(d, make_pair(base, attacker)));
            
            if (d == 0) {
                if (attackerCount.find(base->ID) == attackerCount.end()) {
                    attackerCount[base->ID] = 1;
                } else {
                    attackerCount[base->ID]++;
                }
            }
        }
    }
    
    sort(distToBase.begin(), distToBase.end());
    
    // lock attacker
    int poolCount = 10;
    map<int, bool> isLockedBase; // <baseID, isLocked>
    map<int, int>::iterator aIte;
    for (aIte = attackerCount.begin(); aIte != attackerCount.end(); aIte++) {
        if (aIte->second < poolCount) {
            isLockedBase[aIte->first] = true;
        } else {
            isLockedBase[aIte->first] = false;
        }
    }
    
    vector<pair<int, pair<PlayerUnit *, PlayerUnit *> > >::iterator dIte;
    for (dIte = distToBase.begin(); dIte != distToBase.end(); dIte++) {

        int baseID = dIte->second.first->ID;
        if (isLockedBase[baseID]) {
            
            PlayerUnit *attacker = dIte->second.second;
            if (!attacker->isMovable()) continue;

            if (dIte->first == 0) {
                attacker->fixOnlyPosition();
            } else {
                PlayerUnitActionType at = attacker->moveToTargetAction(dIte->second.first->position);
                Command com(attacker->ID, at);
                addCommand(com);
                attacker->fix(at);
            }
        }
    }
}

// MARK: pattern 3
void QuickAI::assignRightLineCommand() {
    map<int, PlayerUnit>::iterator uIte;
    PlayerUnit *pUnit = nullptr;
    int d = INF;
    int countOnRightLine = 0;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        if (!uIte->second.isMovable()) continue;

        int dX = MAX_FIELD_WIDTH - 1 - uIte->second.position.first;
        int dY = MAX_FIELD_HEIGHT - 1 - uIte->second.position.second;
        if (dX == 0) {
            countOnRightLine++;
            continue;
        }
        if (dY == 0) continue;

        if (dX + dY < d) {
            pUnit = &uIte->second;
            d = dX + dY;
        }
    }
    
    if (countOnRightLine == 0 && pUnit != nullptr) {
        PlayerUnitActionType at = PlayerUnitActionType::MoveRight;
        Command com(pUnit->ID, at);
        addCommand(com);
        pUnit->fix(at);
    }
}
void QuickAI::assignDownLineCommand() {
    map<int, PlayerUnit>::iterator uIte;
    PlayerUnit *pUnit = nullptr;
    int d = INF;
    int countOnDownLine = 0;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        if (!uIte->second.isMovable()) continue;
        
        int dX = MAX_FIELD_WIDTH - 1 - uIte->second.position.first;
        int dY = MAX_FIELD_HEIGHT - 1 - uIte->second.position.second;
        if (dY == 0) {
            countOnDownLine++;
            continue;
        }
        if (dX == 0) continue;

        if (dX + dY < d) {
            pUnit = &uIte->second;
            d = dX + dY;
        }
    }
    
    if (countOnDownLine == 0 && pUnit != nullptr) {
        PlayerUnitActionType at = PlayerUnitActionType::MoveDown;
        Command com(pUnit->ID, at);
        addCommand(com);
        pUnit->fix(at);
    }
}
int QuickAI::baseCountOnEnemyArea() {
    map<int, PlayerUnit>::iterator uIte;
    int count = 0;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        int x = base->position.first;
        int y = base->position.second;
        if (x == MAX_FIELD_WIDTH - 1 || y == MAX_FIELD_HEIGHT - 1) count++;
    }
    return count;
}
void QuickAI::createOneBaseOnEnemyAreaCommand() {
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
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
        Command com(worker->ID, at);
        addCommand(com);
        worker->fix(at);
        break;
    }
    
    sort(downWorkers.begin(), downWorkers.end());
    sort(rightWorkers.begin(), rightWorkers.end());
    
    for (wIte = rightWorkers.begin(); wIte != rightWorkers.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        if (!worker->isMovable()) continue;
        
        PlayerUnitActionType at = PlayerUnitActionType::MoveDown;
        Command com(worker->ID, at);
        addCommand(com);
        worker->fix(at);
    }
    for (wIte = downWorkers.begin(); wIte != downWorkers.end(); wIte++) {
        PlayerUnit *worker = wIte->second;
        if (!worker->isMovable()) continue;

        PlayerUnitActionType at = PlayerUnitActionType::MoveRight;
        Command com(worker->ID, at);
        addCommand(com);
        worker->fix(at);
    }
}

void QuickAI::createOneMoreBaseOnEnemyAreaCommand() {
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
            PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
            Command com(targetWorker->ID, at);
            addCommand(com);
            targetWorker->fix(at);
        } else {
            targetWorker->fixOnlyPosition();
        }
    } else {
        PlayerUnitActionType at = targetWorker->moveToTargetAction(targetBase->position);
        Command com(targetWorker->ID, at);
        addCommand(com);
        targetWorker->fix(at);
    }
}

void QuickAI::createAttackerOnBaseCommand() {
    vector<PlayerUnitType> types;
    for (int i = 0; i < 10; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < 50; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < 40; i++) types.push_back(PlayerUnitType::Assassin);
    
    map<int, PlayerUnit>::iterator uIte;
    for (uIte = player->bases.begin(); uIte != player->bases.end(); uIte++) {
        PlayerUnit *base = &uIte->second;
        if (base->position.second == MAX_FIELD_HEIGHT - 1 || base->position.first == MAX_FIELD_WIDTH - 1) {
            
            PlayerUnitType pType = types[rand() % 100];
            if (!base->isCreatableAttacker(pType)) continue;
            
            PlayerUnitActionType at = CreateAttackerAction(pType);
            Command com(base->ID, at);
            addCommand(com);
            base->fix(at);
        }
    }
}

// MARK: pattern 2
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
    for (int i = 0; i < 10; i++) types.push_back(PlayerUnitType::Knight);
    for (int i = 0; i < 50; i++) types.push_back(PlayerUnitType::Fighter);
    for (int i = 0; i < 40; i++) types.push_back(PlayerUnitType::Assassin);
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

vector<Position> QuickAI::searchEnemyCastleLine1() {
    vector<Position> line;
    for (int y = 59; y < MAX_FIELD_HEIGHT; y++) {
        Position p(95, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchEnemyCastleLine2() {
    vector<Position> line;
    for (int x = 59; x < MAX_FIELD_HEIGHT; x++) {
        Position p(x, 95);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchEnemyCastleLine3() {
    vector<Position> line;
    for (int y = 69; y < MAX_FIELD_HEIGHT; y++) {
        Position p(86, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchEnemyCastleLine4() {
    vector<Position> line;
    for (int x = 69; x < MAX_FIELD_HEIGHT; x++) {
        Position p(x, 86);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchEnemyCastleLine5() {
    vector<Position> line;
    Position p(81, 81);
    line.push_back(p);
    return line;
}

void QuickAI::searchEnemyCastleCommand() {
    map<PlayerUnitType, bool> assasinType;
    assasinType[PlayerUnitType::Assassin] = true;
    
    searchNoVisitedAreaCommand(searchEnemyCastleLine1(), 1, assasinType);
    searchNoVisitedAreaCommand(searchEnemyCastleLine2(), 1, assasinType);
    searchNoVisitedAreaCommand(searchEnemyCastleLine3(), 1, assasinType);
    searchNoVisitedAreaCommand(searchEnemyCastleLine4(), 1, assasinType);
    searchNoVisitedAreaCommand(searchEnemyCastleLine5(), 1, assasinType);
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
void QuickAI::supplyMovableWorkerWithCastle(int need) {
    map<int, PlayerUnit>::iterator uIte;
    int workerCount = 0;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
//        if (uIte->second.isMovable()) workerCount++;
        int hashID = utl::getHashID(uIte->second.position.first, uIte->second.position.second);
        if (field->resources.find(hashID) == field->resources.end()) workerCount++;
    }
    
    if (workerCount < need) {
        PlayerUnit *castle = &player->castle;
        if (!castle->isCreatableWorker()) return;
        
        PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
        Command com(castle->ID, at);
        addCommand(com);
        castle->fix(at);
    }
}
void QuickAI::supplyWorkerForSearchCommand(int need) {
    map<int, PlayerUnit>::iterator uIte;
    int searchWorkerCount = 0;
    for (uIte = player->workers.begin(); uIte != player->workers.end(); uIte++) {
        PlayerUnit *worker = &uIte->second;
        int hashID = utl::getHashID(worker->position.first, worker->position.second);
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

void QuickAI::debug() {
    
}
