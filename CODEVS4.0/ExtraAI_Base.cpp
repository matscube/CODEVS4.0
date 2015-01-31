//
//  ExtraAI.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/17.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "ExtraAI.h"

ExtraAI::ExtraAI(Game &game, Field &field, Player &player, Player &enemy) {
    ExtraAI::game = &game;
    ExtraAI::field = &field;
    ExtraAI::player = &player;
    ExtraAI::enemy = &enemy;
    
    resetWithStage();
}

void ExtraAI::resetWithTurn() {
    enemyCountToAllyCaslte = 0;
    commands.clear();
}
void ExtraAI::resetWithStage() {
    nearestEnemyDistance = INF;
    enemyCountToAllyCaslte = 0;
    memset(workerCreationCount, 0, sizeof(workerCreationCount));
    commands.clear();
    poolReleased = false;
}

vector<Command> ExtraAI::getCommands() {
    return commands;
}
void ExtraAI::addCommand(Command newCommand) {
    commands.push_back(newCommand);
}

// MARK: Basic Command ----------------------------------------------
void ExtraAI::addCommandMove(PlayerUnit *pUnit, Position target, bool synchro, bool formed) {
    if (synchro && formed) {
        int x = pUnit->position.first;
        int y = pUnit->position.second;
        int dx = target.first - x;
        int dy = target.second - y;
        if (dx == 0 && dy == 0) return;
  
        Position next = pUnit->position;
        PlayerUnitActionType at;
        // Priority of right and down
        if (dx > 0 && field->unitCount[x + 1][y] < 10) {
            at = PlayerUnitActionType::MoveRight;
            field->unitCount[x + 1][y]++;
        } else if (dy > 0 && field->unitCount[x][y + 1] < 10) {
            at = PlayerUnitActionType::MoveDown;
            field->unitCount[x][y + 1]++;
        } else if (dx < 0 && field->unitCount[x - 1][y] < 10) {
            at = PlayerUnitActionType::MoveLeft;
            field->unitCount[x - 1][y]++;
        } else if (dy < 0 && field->unitCount[x][y - 1] < 10) {
            at = PlayerUnitActionType::MoveUp;
            field->unitCount[x][y - 1]++;
        } else {
            // Move target is full
            at = PlayerUnitActionType::None;
            field->unitCount[x][y]++;
        }
        
        if (at == PlayerUnitActionType::None) {
            pUnit->fixOnlyPosition();
        } else {
            Command com(pUnit->ID, at);
            addCommand(com);
            pUnit->fix(at);
        }
    } else {
        PlayerUnitActionType at = pUnit->moveToTargetAction(target, synchro);
        Command com(pUnit->ID, at);
        addCommand(com);
        pUnit->fix(at);
    }
}
void ExtraAI::addCommandMove(PlayerUnit *pUnit, PlayerUnitActionType moveAction) {
    bool move = false;
    if (moveAction == PlayerUnitActionType::MoveUp) move = true;
    if (moveAction == PlayerUnitActionType::MoveDown) move = true;
    if (moveAction == PlayerUnitActionType::MoveRight) move = true;
    if (moveAction == PlayerUnitActionType::MoveLeft) move = true;
    if (!move) {
        cerr << "Error: addCommandMove, action type is not move type." << endl;
    }
    
    Command com(pUnit->ID, moveAction);
    addCommand(com);
    pUnit->fix(moveAction);
}

void ExtraAI::addCommandCreateVillage(PlayerUnit *pUnit) {
    PlayerUnitActionType at = PlayerUnitActionType::CreateVillage;
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}
void ExtraAI::addCommandCreateBase(PlayerUnit *pUnit) {
    PlayerUnitActionType at = PlayerUnitActionType::CreateBase;
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}
void ExtraAI::addCommandCreateWorker(PlayerUnit *pUnit) {
    PlayerUnitActionType at = PlayerUnitActionType::CreateWorker;
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}
void ExtraAI::addCommandCreateAttacker(PlayerUnit *pUnit, PlayerUnitType unitType) {
    PlayerUnitActionType at = CreateAttackerAction(unitType);
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
}

// MARK: Utility
map<PlayerUnitType, bool> ExtraAI::allTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Castle] = true;
    res[PlayerUnitType::Worker] = true;
    res[PlayerUnitType::Knight] = true;
    res[PlayerUnitType::Fighter] = true;
    res[PlayerUnitType::Assassin] = true;
    res[PlayerUnitType::Village] = true;
    res[PlayerUnitType::Base] = true;
    return res;
}
map<PlayerUnitType, bool> ExtraAI::workerTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Worker] = true;
    return res;
}
map<PlayerUnitType, bool> ExtraAI::attackerTypes() {
    map<PlayerUnitType, bool> res;
    res[PlayerUnitType::Knight] = true;
    res[PlayerUnitType::Fighter] = true;
    res[PlayerUnitType::Assassin] = true;
    return res;
}

// MARK: Cost limit
void ExtraAI::setCostLimit(int limit) {
    player->necessaryResourceCount = limit;
}

// MARK: Debug --------------------------------------------------------
void ExtraAI::debug() {
    
}
