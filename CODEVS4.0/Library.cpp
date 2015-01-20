//
//  Library.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Library.h"
#include "Field.h"

int utl::dist(Position pos1, Position pos2) {
    return abs(pos1.first - pos2.first) + abs(pos1.second - pos2.second);
}

int utl::getHashID(Position pos) {
    return pos.second * MAX_FIELD_WIDTH + pos.first;
}

bool utl::isValidUnitID(int ID) {
    if (ID < 0) return false;
    else return true;
}

vector<Position> utl::framePositions(int d, bool shuffle) {
    vector<Position> positions;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (dist(Position(0, 0), Position(dx, dy)) == d) {
                Position p = Position(dx, dy);
                positions.push_back(p);
            }
        }
    }
    
    if (shuffle) random_shuffle(positions.begin(), positions.end());

    return positions;
}
vector<Position> utl::areaPositions(int d, bool shuffle) {
    vector<Position> positions;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (dist(Position(0, 0), Position(dx, dy)) <= d) {
                Position p = Position(dx, dy);
                positions.push_back(p);
            }
        }
    }
    
    if (shuffle) random_shuffle(positions.begin(), positions.end());
    
    return positions;
}
vector<Position> utl::viewRangePositions(int d) {
    vector<Position> positions;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (dist(Position(0, 0), Position(dx, dy)) <= d) {
                Position p = Position(dx, dy);
                positions.push_back(p);
            }
        }
    }
    return positions;
}