//
//  Field.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "Field.h"
#include "PlayerUnit.h"
#include "Player.h"

bool isValidIndex(Position p) {
    if (p.first < 0 || p.first >= MAX_FIELD_WIDTH) return false;
    if (p.second < 0 || p.second >= MAX_FIELD_HEIGHT) return false;
    return true;
}

Field::Field() {
    resetWithStage();
}

void Field::resetWithStage() {
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            isViewed[x][y] = false;
            isVisited[x][y] = false;
        }
    }
    ofs = ofstream("/Users/matscube/Desktop/field_debug.txt");
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            isViewed[x][y] = false;
            isVisited[x][y] = false;
        }
    }
    resources.clear();
    resetWithTurn();
}

void Field::resetWithTurn() {
    memcpy(willBeVisited, isVisited, sizeof(isVisited));
    memcpy(willBeViewed, isViewed, sizeof(isViewed));
    memset(enemyWorkerCount, 0, sizeof(enemyWorkerCount));
    memset(allyWorkerCount, 0, sizeof(allyWorkerCount));
    memset(unitCount, 0, sizeof(unitCount));
}

int Field::calcVisited() {
    int cnt = 0;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            if (isVisited[x][y]) cnt++;
        }
    }
    return cnt;
}

ResourceUnit::ResourceUnit() {}
ResourceUnit::ResourceUnit(Position position) {
    ResourceUnit::position = position;
    ResourceUnit::hashID = utl::getHashID(position);
    ResourceUnit::status = ResourceUnitStatus::Default;
}

map<int, Position> Field::enemyCastlePositions(PlayerType pType) {
    map<int, Position> res;
    for (int dx = 0; dx <= 40; dx++) {
        for (int dy = 0; dy <= 40 - dx; dy++) {
            Position pos;
            if (pType == PlayerType::Ally) {
                pos = Position(MAX_FIELD_WIDTH - 1 - dx, MAX_FIELD_HEIGHT - 1 - dy);
            } else {
                pos = Position(dx, dy);
            }
            int hashID = utl::getHashID(pos);
            res[hashID] = pos;
        }
    }
    return res;
}



void Field::updateWithResourceUnit(ResourceUnit resourceUnit) {
    if (resources.find(resourceUnit.hashID) == resources.end()) {
        resources[resourceUnit.hashID] = resourceUnit;
    }
}

void Field::updateResourceStatus() {
    map<int, ResourceUnit>::iterator rIte;
    for (rIte = resources.begin(); rIte != resources.end(); rIte++) {
        ResourceUnit *res = &rIte->second;
        if (enemyWorkerCount[res->position.first][res->position.second]) {
            res->status = ResourceUnitStatus::Enemy;
        } else if (allyWorkerCount[res->position.first][res->position.second]) {
            res->status = ResourceUnitStatus::Ally;
        } else {
            if (res->status == ResourceUnitStatus::Ally) {
                res->status = ResourceUnitStatus::Enemy;
            } else {
                // no operation
            }
        }
    }
    // TODO: ally worker -> no worker, then default status, but enemy status now. fix me
}

void Field::updateWithPlayerUnit(PlayerUnit *playerUnit) {
    if (playerUnit->player->type == PlayerType::Ally) {
        Position cPos = playerUnit->position;
        if (isValidIndex(cPos)) isVisited[cPos.first][cPos.second] = true;

        int viewRange = PlayerUnit::viewRange(playerUnit->type);
        for (int x = cPos.first - viewRange; x <= cPos.first + viewRange; x++) {
            for (int y = cPos.second - viewRange; y <= cPos.second + viewRange; y++) {
                if (!isValidIndex(Position(x, y))) continue;
                if (utl::dist(cPos, Position(x, y)) > viewRange) continue;
                isViewed[x][y] = true;
                // set value on willBeVisited by resetWithTurn
            }
        }
        
        if (playerUnit->type == PlayerUnitType::Worker) {
            allyWorkerCount[playerUnit->position.first][playerUnit->position.second]++;
        }
    } else if (playerUnit->player->type == PlayerType::Enemy) {
        if (playerUnit->type == PlayerUnitType::Worker) {
            enemyWorkerCount[playerUnit->position.first][playerUnit->position.second]++;
        }
    }
}

void Field::debugStatusInfo() {
    int defaultCount = 0;
    int allyCount = 0;
    int enemyCount = 0;
    map<int, ResourceUnit>::iterator rIte;
    for (rIte = resources.begin(); rIte != resources.end(); rIte++) {
        if (rIte->second.status == ResourceUnitStatus::Default) defaultCount++;
        if (rIte->second.status == ResourceUnitStatus::Ally) allyCount++;
        if (rIte->second.status == ResourceUnitStatus::Enemy) enemyCount++;
    }
    
    cerr << "ResourceStatus: def " << defaultCount << ", ally " << allyCount << ", enemy " << enemyCount << endl;
}
