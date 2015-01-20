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
    commands.clear();
}
void ExtraAI::resetWithStage() {
    commands.clear();
}

vector<Command> ExtraAI::getCommands() {
    return commands;
}
void ExtraAI::addCommand(Command newCommand) {
    commands.push_back(newCommand);
}

// MARK: Basic Command ----------------------------------------------
void ExtraAI::addCommandMove(PlayerUnit *pUnit, Position target) {
    PlayerUnitActionType at = pUnit->moveToTargetAction(target);
    Command com(pUnit->ID, at);
    addCommand(com);
    pUnit->fix(at);
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

// MARK: Debug --------------------------------------------------------
void ExtraAI::debug() {
    
}
