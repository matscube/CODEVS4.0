//
//  Field.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Field.h"
#include "Library.h"

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
            willBeViewed[x][y] = false;
            isVisited[x][y] = false;
            willBeVisited[x][y] = false;
        }
    }
    ofs = ofstream("/Users/matscube/field.txt");
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            isViewed[x][y] = false;
            willBeViewed[x][y] = false;
            isVisited[x][y] = false;
            willBeVisited[x][y] = false;
        }
    }
    resources.clear();
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
    ResourceUnit::occupancy = 0;
    ResourceUnit::hashID = getHashID(position);
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
            int hashID = utl::getHashID(pos.first, pos.second);
            res[hashID] = pos;
        }
    }
    return res;
}


void Field::resetStatusWithTurn() {
    memset(allyWorkers, 0, sizeof(allyWorkers));
    memset(reservedWorkers, 0, sizeof(reservedWorkers));
    memcpy(willBeVisited, isVisited, sizeof(isVisited));
    memcpy(willBeViewed, isViewed, sizeof(isViewed));
    map<int, ResourceUnit>::iterator resIte = resources.begin();
    for (; resIte != resources.end(); resIte++) {
        resIte->second.occupancy = 0;
    }
}

void Field::updateStatusWithAllyUnit(PlayerUnit allyUnit) {
    Position cPos = allyUnit.position;
    
    // Update FieldStatus
    int viewRange = PlayerUnit::viewRange(allyUnit.type);
    for (int x = cPos.first - viewRange; x <= cPos.first + viewRange; x++) {
        for (int y = cPos.second - viewRange; y <= cPos.second + viewRange; y++) {
            if (!isValidIndex(Position(x, y))) continue;
            if (utl::dist(cPos, Position(x, y)) > viewRange) continue;
        }
    }
    
    // Update Resource Occupancy
    allyWorkers[cPos.first][cPos.second]++;
}

void Field::updateStatusWithResourceUnit(ResourceUnit resourceUnit) {
    if (resources.find(resourceUnit.hashID) == resources.end()) {
        resources[resourceUnit.hashID] = resourceUnit;
    }
}

void Field::updateWithPlayerUnit(PlayerUnit *playerUnit) {
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
}

int ResourceUnit::getHashID(Position position) {
    return position.second * MAX_FIELD_WIDTH + position.first;
}
