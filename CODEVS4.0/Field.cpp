//
//  Field.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Field.h"
#include "Library.h"

Field::Field() {
    initStatus();
}

FieldUnit::FieldUnit() {}
FieldUnit::FieldUnit(int x, int y, FieldUnitType type) {
    FieldUnit::x = x;
    FieldUnit::y = y;
    FieldUnit::type = type;
    FieldUnit::hashID = getHashID(x, y);
}


void Field::resetStatusWithTurn() {
    memset(allyWorkers, 0, sizeof(allyWorkers));
    memset(reservedWorkers, 0, sizeof(reservedWorkers));
}

void Field::initStatus() {
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        for (int y = 0; y < MAX_FIELD_HEIGHT; y++) {
            status[x][y] = FieldStatus::Unknown;
        }
    }
}
void Field::updateStatusWithAllyUnit(PlayerUnit allyUnit) {
    int cx = allyUnit.x;
    int cy = allyUnit.y;
    
    // Update FieldStatus
    int viewRange = PlayerUnit::viewRange(allyUnit.type);
    for (int x = cx - viewRange; x <= cx + viewRange; x++) {
        for (int y = cy - viewRange; y <= cy + viewRange; y++) {
            if (x < 0 && x >= MAX_FIELD_WIDTH) continue;
            if (y < 0 && y >= MAX_FIELD_HEIGHT) continue;
            if (dist(cx, cy, x, y) > viewRange) continue;
            if (status[x][y] != FieldStatus::Unknown) continue;

            status[cx][cy] = FieldStatus::Visited;
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

int FieldUnit::getHashID(int x, int y) {
    return y * MAX_FIELD_WIDTH + x;
}
