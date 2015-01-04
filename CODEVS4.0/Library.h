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
#include "Field.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

#define INF 1<<29

using namespace std;

int dist(int x1, int y1, int x2, int y2);
int getHashID(int x, int y);
bool isValidUnitID(int ID);
vector<Position> framePositions(int d, bool shuffle);
vector<Position> areaPositions(int d, bool shuffle);
vector<Position> viewRangePositions(int d);

#endif /* defined(__CODEVS4_0__Library__) */
