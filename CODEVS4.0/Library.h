//
//  Library.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__Library__
#define __CODEVS4_0__Library__

#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

#define INF 1<<29
#define MAX_FIELD_WIDTH 100
#define MAX_FIELD_HEIGHT 100
#define MAX_FIELD_CELL_COUNT 10000
#define MAX_GETTING_RESOURCE 5

using namespace std;

typedef pair<int, int> Position;

// TODO: Logger

namespace utl {
//    int dist(int x1, int y1, int x2, int y2);
    int dist(Position pos1, Position pos2);
    int distSingle(Position pos1, Position pos2);
    int getHashID(Position pos);
    bool isValidUnitID(int ID);
    bool isValidPosition(Position pos);
    vector<Position> framePositions(int d, bool shuffle);
    vector<Position> areaPositions(int d, bool shuffle);
    vector<Position> viewRangePositions(int d);
}

enum class PlayerType {
    Unknown,
    Ally,
    Enemy,
};

enum class CastleMode {
    Unknown,
    Alone,
    Defending,
};


class Player;
enum class PlayerUnitActionType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    CreateWorker,
    CreateKnight,
    CreateFighter,
    CreateAssassin,
    CreateVillage,
    CreateBase,
    None,
};

enum class PlayerUnitType {
    Worker,
    Knight,
    Fighter,
    Assassin,
    Castle,
    Village,
    Base,
    Unknown,
};

enum class PlayerUnitStatus {
    Idle,
    FixPosition,
    Reserved,
};

#endif /* defined(__CODEVS4_0__Library__) */
