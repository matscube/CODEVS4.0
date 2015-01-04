//
//  Library.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Library.h"
#include "Field.h"

int dist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int getHashID(int x, int y) {
    return y * MAX_FIELD_WIDTH + x;
}

bool isValidUnitID(int ID) {
    if (ID < 0) return false;
    else return true;
}

vector<Position> framePositions(int d, bool shuffle) {
    vector<Position> positions;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (dist(0, 0, dx, dy) == d) {
                Position p = Position(dx, dy);
                positions.push_back(p);
            }
        }
    }
    
    if (shuffle) random_shuffle(positions.begin(), positions.end());

    return positions;
}
vector<Position> areaPositions(int d, bool shuffle) {
    vector<Position> positions;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (dist(0, 0, dx, dy) <= d) {
                Position p = Position(dx, dy);
                positions.push_back(p);
            }
        }
    }
    
    if (shuffle) random_shuffle(positions.begin(), positions.end());
    
    return positions;
}
vector<Position> viewRangePositions(int d) {
    vector<Position> positions;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (dist(0, 0, dx, dy) <= d) {
                Position p = Position(dx, dy);
                positions.push_back(p);
            }
        }
    }
    return positions;
}