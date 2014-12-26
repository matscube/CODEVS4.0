//
//  Field.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Field.h"
#include "Library.h"

bool isValidIndex(int x, int y) {
    if (x < 0 || x >= MAX_FIELD_WIDTH) return false;
    if (y < 0 || y >= MAX_FIELD_HEIGHT) return false;
    return true;
}

string FieldStatusName(FieldStatus s) {
    switch (s) {
        case FieldStatus::Unknown: return "Unknown";
        case FieldStatus::Visited: return "Visited";
        case FieldStatus::Resource: return "Resource";
        case FieldStatus::AllyCastle: return "AllyCastle";
        case FieldStatus::EnemyCastle: return "EnemyCastle";
    }
    return "Unknown FieldStatus type";
}

Field::Field() {
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            status[x][y] = FieldStatus::Unknown;
            isVisited[x][y] = false;
            willBeVisited[x][y] = false;
        }
    }
    ofs = ofstream("/Users/matscube/field.txt");
}

FieldUnit::FieldUnit() {}
FieldUnit::FieldUnit(int x, int y, FieldUnitType type) {
    FieldUnit::x = x;
    FieldUnit::y = y;
    FieldUnit::type = type;
    FieldUnit::occupancy = 0;
    FieldUnit::hashID = getHashID(x, y);
}


void Field::resetStatusWithTurn() {
    memset(allyWorkers, 0, sizeof(allyWorkers));
    memset(reservedWorkers, 0, sizeof(reservedWorkers));
    memcpy(willBeVisited, isVisited, sizeof(isVisited));
    map<int, FieldUnit>::iterator resIte = resources.begin();
    for (; resIte != resources.end(); resIte++) {
        resIte->second.occupancy = 0;
    }
}

void Field::updateStatusWithAllyUnit(PlayerUnit allyUnit) {
    int cx = allyUnit.x;
    int cy = allyUnit.y;
    
    // Update FieldStatus
    int viewRange = PlayerUnit::viewRange(allyUnit.type);
    for (int x = cx - viewRange; x <= cx + viewRange; x++) {
        for (int y = cy - viewRange; y <= cy + viewRange; y++) {
            if (!isValidIndex(x, y)) continue;
            if (dist(cx, cy, x, y) > viewRange) continue;
            if (status[x][y] != FieldStatus::Unknown) continue;
            
            status[x][y] = FieldStatus::Visited;
        }
    }
    
    // Update Resource Occupancy
    allyWorkers[cx][cy]++;
}

void Field::updateStatusWithFieldUnit(FieldUnit fieldUnit) {
    if (fieldUnit.type == FieldUnitType::Resource) {
        status[fieldUnit.x][fieldUnit.y] = FieldStatus::Resource;
        if (resources.find(fieldUnit.hashID) == resources.end()) {
            resources[fieldUnit.hashID] = fieldUnit;
        }
    }
}

void Field::updateVisited(PlayerUnit *playerUnit) {
    int cx = playerUnit->x;
    int cy = playerUnit->y;
    int viewRange = PlayerUnit::viewRange(playerUnit->type);
    for (int x = cx - viewRange; x <= cx + viewRange; x++) {
        for (int y = cy - viewRange; y <= cy + viewRange; y++) {
            if (!isValidIndex(x, y)) continue;
            if (dist(cx, cy, x, y) > viewRange) continue;
            isVisited[x][y] = true;
            // set value on willBeVisited by resetWithTurn
        }
    }
}

int FieldUnit::getHashID(int x, int y) {
    return y * MAX_FIELD_WIDTH + x;
}