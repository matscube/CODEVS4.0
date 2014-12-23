//
//  Player.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Player.h"
#include "Library.h"

int PlayerUnit::cost(PlayerUnitType type) {
    switch (type) {
        case PlayerUnitType::Worker: return 40;
        case PlayerUnitType::Knight: return 20;
        case PlayerUnitType::Fighter: return 40;
        case PlayerUnitType::Assassin: return 60;
        case PlayerUnitType::Castle: return INF;
        case PlayerUnitType::Village: return 100;
        case PlayerUnitType::Base: return 500;
        default:
            cerr << "[PlayerUnit::cost] Error: Unknonwn PlayerUnitType is called, PlayerUnitType: " << &type << endl;
            return 0;
    }
}

string PlayerUnitTypeName(PlayerUnitType t) {
    switch (t) {
        case PlayerUnitType::Worker: return "Worker";
        case PlayerUnitType::Knight: return "Knight";
        case PlayerUnitType::Fighter: return "Fighter";
        case PlayerUnitType::Assassin: return "Assassin";
        case PlayerUnitType::Castle: return "Castle";
        case PlayerUnitType::Village: return "Village";
        case PlayerUnitType::Base: return "Base";
    }
    return "Unkown Type";
}

string PlayerUnitStatusName(PlayerUnitStatus s) {
    switch (s) {
        case PlayerUnitStatus::Idle: return "Idle";
        case PlayerUnitStatus::Reserved: return "Reserved";
    }
    return "Unkwon Type";
}

string PlayerUnit::action(PlayerUnitActionType type) {
    switch (type) {
        // Move Action
        case PlayerUnitActionType::MoveUp: return "U";
        case PlayerUnitActionType::MoveDown: return "D";
        case PlayerUnitActionType::MoveLeft: return "L";
        case PlayerUnitActionType::MoveRight: return "R";

        // Create Action
        case PlayerUnitActionType::CreateWorker: return "0";
        case PlayerUnitActionType::CreateKnight: return "1";
        case PlayerUnitActionType::CreateFighter: return "2";
        case PlayerUnitActionType::CreateAssassin: return "3";
        // Castle not creatable
        case PlayerUnitActionType::CreateVillage: return "5";
        case PlayerUnitActionType::CreateBase: return "6";
        default:
            cerr << "[PlayerUnit::action] Error: Unknonwn PlayerUnitActionType is called, PlayerUnitActionType: " << &type << endl;
            return "";
    }
}

int PlayerUnit::viewRange(PlayerUnitType type) {
    switch (type) {
        case PlayerUnitType::Worker: return 4;
        case PlayerUnitType::Knight: return 4;
        case PlayerUnitType::Fighter: return 4;
        case PlayerUnitType::Assassin: return 4;
        case PlayerUnitType::Castle: return 10;
        case PlayerUnitType::Village: return 10;
        case PlayerUnitType::Base: return 4;
        default:
            cerr << "[PlayerUnit::viewRange] Error: Unknonwn PlayerUnitType is called, PlayerUnitType: " << &type << endl;
            return 0;
    }
}

int PlayerUnit::attackRange(PlayerUnitType type) {
    switch (type) {
        case PlayerUnitType::Worker: return 2;
        case PlayerUnitType::Knight: return 2;
        case PlayerUnitType::Fighter: return 2;
        case PlayerUnitType::Assassin: return 2;
        case PlayerUnitType::Castle: return 10;
        case PlayerUnitType::Village: return 2;
        case PlayerUnitType::Base: return 2;
        default:
            cerr << "[PlayerUnit::attackRange] Error: Unknonwn PlayerUnitType is called, PlayerUnitType: " << &type << endl;
            return 0;
    }
}

Player::Player() {
    
}

void Player::resetWithTurn() {
    units.clear();
    map<int, PlayerUnit>::iterator iteUnit = units.begin();
    for (; iteUnit != units.end(); iteUnit++) {
        iteUnit->second.status = PlayerUnitStatus::Idle;
    }
}

PlayerUnit::PlayerUnit() {}
PlayerUnit::PlayerUnit(int ID, int x, int y, PlayerUnitType type) {
    PlayerUnit::ID = ID;
    PlayerUnit::x = x;
    PlayerUnit::y = y;
    PlayerUnit::type = type;
    status = PlayerUnitStatus::Idle;
}

PlayerUnitActionType PlayerUnit::moveToTargetAction(int targetX,int targetY) {
    int dx = targetX - x;
    int dy = targetY - y;

    if (dx == 0 && dy == 0) {
        return PlayerUnitActionType::None;
    }

    if (rand() % (abs(dx) + abs(dy)) < abs(dy)) {
        if (dy > 0) return PlayerUnitActionType::MoveDown;
        else return PlayerUnitActionType::MoveUp;
    } else {
        if (dx > 0) return PlayerUnitActionType::MoveRight;
        else return PlayerUnitActionType::MoveLeft;
    }
}

void PlayerUnit::setHitPoint(int hitPoint) {
    PlayerUnit::hitPoint = hitPoint;
}

bool PlayerUnit::isMovable() {
    switch (type) {
        case PlayerUnitType::Worker: return true;
        case PlayerUnitType::Knight: return true;
        case PlayerUnitType::Fighter: return true;
        case PlayerUnitType::Assassin: return true;
        case PlayerUnitType::Castle: return false;
        case PlayerUnitType::Village: return false;
        case PlayerUnitType::Base: return false;
    }
    cerr << "[PlayerUnit::isMovable] Error: Unkwon Type" << endl;
    return false;
}

bool PlayerUnit::isCommandable() {
    if (status == PlayerUnitStatus::Idle) return true;
    else return false;
}

void PlayerUnit::setReserved() {
    status = PlayerUnitStatus::Reserved;
}
