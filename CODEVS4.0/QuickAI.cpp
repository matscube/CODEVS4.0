//
//  QuickAI.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/08.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "QuickAI.h"

QuickAI::QuickAI(Game &game, Field &field, Player &player) {
    QuickAI::game = &game;
    QuickAI::field = &field;
    QuickAI::player = &player;
}

void QuickAI::resetWithTurn() {
    commands.clear();
}

vector<Command> QuickAI::getCommands() {
    return commands;
}

void QuickAI::addCommands(vector<Command> newCommands) {
    commands.insert(commands.begin(), newCommands.begin(), newCommands.end());
}


vector<Position> QuickAI::searchLine1() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 4);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine2() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 13);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine3() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 22);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine4() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 31);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchLine5() {
    vector<Position> line;
    for (int x = 0; x < MAX_FIELD_WIDTH; x++) {
        Position p(x, 40);
        line.push_back(p);
    }
    return line;
}
vector<Position> QuickAI::searchArea() {
    vector<Position> area;
    vector<Position> line1 = searchLine1();
    vector<Position> line2 = searchLine2();
    vector<Position> line3 = searchLine3();
    vector<Position> line4 = searchLine4();
    vector<Position> line5 = searchLine5();
    area.insert(area.end(), line1.begin(), line1.end());
    area.insert(area.end(), line2.begin(), line2.end());
    area.insert(area.end(), line3.begin(), line3.end());
    area.insert(area.end(), line4.begin(), line4.end());
    area.insert(area.end(), line5.begin(), line5.end());
    
    return area;
}

void QuickAI::searchCommand(vector<Position> area, int assign) {
    vector<Command> commands;
    
    vector<Position>::iterator areaIte;
    map<int, PlayerUnit>::iterator uIte;
    vector<pair<int, pair<int, Position> > > dToLine; // <d, <unitID, Position>>
    for (uIte = player->units.begin(); uIte != player->units.end(); uIte++) {
        if (!uIte->second.isMovable()) continue;
        
        for (areaIte = area.begin(); areaIte != area.end(); areaIte++) {
            if (field->willBeVisited[areaIte->first][areaIte->second]) continue;

            int d = dist(uIte->second.x, uIte->second.y, areaIte->first, areaIte->second);
            if (d > 0) {
                dToLine.push_back(make_pair(d, make_pair(uIte->second.ID, *areaIte)));
            }
        }
    }
    
    sort(dToLine.begin(), dToLine.end());
    
    vector<Position> viewRange = viewRangePositions(PlayerUnit::viewRange(PlayerUnitType::Worker));
    vector<Position>::iterator viewRangeIte;

    int currentAssign = 0;
    vector<pair<int, pair<int, Position> > >::iterator dIte;
    for (dIte = dToLine.begin(); dIte != dToLine.end(); dIte++) {
        PlayerUnit *pUnit = &player->units[dIte->second.first];
        Position pos = dIte->second.second;
        
        if (!pUnit->isMovable()) continue;
        if (field->willBeVisited[pos.first][pos.second]) continue;
        
        PlayerUnitActionType at = pUnit->moveToTargetAction(pos.first, pos.second);
        Command com(pUnit->ID, at);
        commands.push_back(com);
        pUnit->fix(at);
        
        for (viewRangeIte = viewRange.begin(); viewRangeIte != viewRange.end(); viewRangeIte++) {
            int x = pos.first + viewRangeIte->first;
            int y = pos.second + viewRangeIte->second;
            if (isValidIndex(x, y))
                field->willBeVisited[x][y] = true;
        }
        
        if (++currentAssign >= assign) break;
    }
    
    addCommands(commands);
}

void QuickAI::resourceAssignCommand(int assign) {
    vector<Command> commands;
    
    map<int, PlayerUnit>::iterator pIte;
    map<int, bool> isVillage; // <hashID, exist>
    for (pIte = player->units.begin(); pIte != player->units.end(); pIte++) {
        if (pIte->second.type != PlayerUnitType::Village) continue;
        int hashID = getHashID(pIte->second.x, pIte->second.y);
        isVillage[hashID] = true;
    }
    
    // no village resource, assign worker
    map<int, FieldUnit>::iterator resIte;
    vector<pair<int, pair<int, int> > > dToRes; // <dist, <unitID, resID>>
    for (resIte = field->resources.begin(); resIte != field->resources.end(); resIte++) {
        if (isVillage.find(resIte->second.hashID) != isVillage.end()) continue; // resource has village
        
        for (pIte = player->units.begin(); pIte != player->units.end(); pIte++) {
            int d = dist(pIte->second.x, pIte->second.y, resIte->second.x, resIte->second.y);
            dToRes.push_back(make_pair(d, make_pair(pIte->second.ID, resIte->second.hashID)));
        }
    }
    
    sort(dToRes.begin(), dToRes.end());
    
    vector<pair<int, pair<int, int> > >::iterator dIte;
    for (dIte = dToRes.begin(); dIte != dToRes.end(); dIte++) {
        if (dIte->first == 0) {
            
        } else {
            
        }
    }
    
    // create village

    // village resource
    //
    
    addCommands(commands);
}

void QuickAI::debug() {
    
}
