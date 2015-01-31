//
//  ExtraAI_Search.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/21.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "ExtraAI.h"

// MARK: Search -------------------------------------------------------
vector<Position> ExtraAI::searchLineToRight1() {
    vector<Position> line;
    for (int x = 40; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight2() {
    vector<Position> line;
    for (int x = 35; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight3() {
    vector<Position> line;
    for (int x = 30; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight4() {
    vector<Position> line;
    for (int x = 30; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 31);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight5() {
    vector<Position> line;
    for (int x = 30; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 40);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight6() {
    vector<Position> line;
    for (int x = 45; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 49);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight7() {
    vector<Position> line;
    for (int x = 54; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 58);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight8() {
    vector<Position> line;
    for (int x = 63; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 67);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight9() {
    vector<Position> line;
    for (int x = 72; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 76);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToRight10() {
    vector<Position> line;
    for (int x = 81; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 85);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown1() {
    vector<Position> line;
    for (int y = 40; y < MAX_FIELD_HEIGHT; y++) {
        Position p(4, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown2() {
    vector<Position> line;
    for (int y = 35; y < MAX_FIELD_HEIGHT; y++) {
        Position p(13, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown3() {
    vector<Position> line;
    for (int y = 30; y < MAX_FIELD_HEIGHT; y++) {
        Position p(22, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown4() {
    vector<Position> line;
    for (int y = 30; y < MAX_FIELD_HEIGHT; y++) {
        Position p(31, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown5() {
    vector<Position> line;
    for (int y = 30; y < MAX_FIELD_HEIGHT; y++) {
        Position p(40, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown6() {
    vector<Position> line;
    for (int y = 45; y < MAX_FIELD_HEIGHT; y++) {
        Position p(49, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown7() {
    vector<Position> line;
    for (int y = 54; y < MAX_FIELD_HEIGHT; y++) {
        Position p(58, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown8() {
    vector<Position> line;
    for (int y = 63; y < MAX_FIELD_HEIGHT; y++) {
        Position p(67, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown9() {
    vector<Position> line;
    for (int y = 72; y < MAX_FIELD_HEIGHT; y++) {
        Position p(76, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchLineToDown10() {
    vector<Position> line;
    for (int y = 81; y < MAX_FIELD_HEIGHT; y++) {
        Position p(85, y);
        line.push_back(p);
    }
    return line;
}

vector<Position> ExtraAI::searchLineAlly1() {
    vector<Position> line;
    for (int x = 0; x < 40; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    for (int x = 9; x < 35; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    for (int x = 9; x < 30; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    for (int x = 9; x < 30; x++) {
        Position p(x, 31);
        line.push_back(p);
    }
    for (int y = 9; y < 40; y++) {
        Position p(4, y);
        line.push_back(p);
    }
    return line;
}

void ExtraAI::searchNoVisitedAreaCommand(vector<Position> area, int assign, map<PlayerUnitType, bool> types) {
    
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
        if (currentAssign >= assign) break;
        
        PlayerUnit *pUnit = player->units[dIte->second.first];
        Position pos = dIte->second.second;
        
        if (!pUnit->isMovable()) continue;
        if (field->willBeVisited[pos.first][pos.second]) continue;
        
        addCommandMove(pUnit, pos);
        currentAssign++;
        
        for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {
            int x = pos.first + viewRangeIte->first;
            int y = pos.second + viewRangeIte->second;
            if (isValidIndex(Position(x, y)))
                field->willBeVisited[x][y] = true;
        }
    }
}


void ExtraAI::searchUnkownFieldSmallCommand(int assign) {
    int currentAssign = 0;
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());

    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly1(), 1, allTypes());
}
void ExtraAI::searchUnkownFieldMediumCommand(int assign) {
    int currentAssign = 0;
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight5(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown5(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly1(), 1, allTypes());
}
void ExtraAI::searchUnkownFieldAllCommand(int assign) {
    int currentAssign = 0;
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight5(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight6(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight7(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight8(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight9(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToRight10(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown1(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown2(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown3(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown4(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown5(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown6(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown7(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown8(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown9(), 1, allTypes());
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineToDown10(), 1, allTypes());
    
    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly1(), 1, allTypes());
    //    if (++currentAssign <= assign) searchNoVisitedAreaCommand(searchLineAlly2(), 1, allTypes());
}


vector<Position> ExtraAI::searchEnemyCastleLine1() {
    vector<Position> line;
    for (int y = 59; y < MAX_FIELD_HEIGHT; y++) {
        Position p(95, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchEnemyCastleLine2() {
    vector<Position> line;
    for (int x = 59; x < MAX_FIELD_HEIGHT; x++) {
        Position p(x, 95);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchEnemyCastleLine3() {
    vector<Position> line;
    for (int y = 69; y < MAX_FIELD_HEIGHT; y++) {
        Position p(86, y);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchEnemyCastleLine4() {
    vector<Position> line;
    for (int x = 69; x < MAX_FIELD_HEIGHT; x++) {
        Position p(x, 86);
        line.push_back(p);
    }
    return line;
}
vector<Position> ExtraAI::searchEnemyCastleLine5() {
    vector<Position> line;
    Position p(81, 81);
    line.push_back(p);
    return line;
}

void ExtraAI::searchEnemyCastleCommand() {
    map<PlayerUnitType, bool> types;
//    types[PlayerUnitType::Knight] = true;
//    types[PlayerUnitType::Assassin] = true;
    types[PlayerUnitType::Assassin] = true;
    
    searchNoVisitedAreaCommand(searchEnemyCastleLine1(), 1, types);
    searchNoVisitedAreaCommand(searchEnemyCastleLine2(), 1, types);
    searchNoVisitedAreaCommand(searchEnemyCastleLine3(), 1, types);
    searchNoVisitedAreaCommand(searchEnemyCastleLine4(), 1, types);
    searchNoVisitedAreaCommand(searchEnemyCastleLine5(), 1, types);
}
