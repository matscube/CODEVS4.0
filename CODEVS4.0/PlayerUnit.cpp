//
//  PlayerUnit.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/24.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#include "PlayerUnit.h"
#include "Player.h"

string PlayerUnitTypeName(PlayerUnitType t) {
    switch (t) {
        case PlayerUnitType::Worker: return "Worker";
        case PlayerUnitType::Knight: return "Knight";
        case PlayerUnitType::Fighter: return "Fighter";
        case PlayerUnitType::Assassin: return "Assassin";
        case PlayerUnitType::Castle: return "Castle";
        case PlayerUnitType::Village: return "Village";
        case PlayerUnitType::Base: return "Base";
        case PlayerUnitType::Unknown: break;
    }
    return "Unkown Type";
}

string PlayerUnitStatusName(PlayerUnitStatus s) {
    switch (s) {
        case PlayerUnitStatus::Idle: return "Idle";
        case PlayerUnitStatus::Reserved: return "Reserved";
        case PlayerUnitStatus::FixPosition: return "FixPosition";
    }
    return "Unkwon Type";
}

PlayerUnitType UnitTypeCreated(PlayerUnitActionType at) {
    switch (at) {
        case PlayerUnitActionType::CreateWorker: return PlayerUnitType::Worker;
        case PlayerUnitActionType::CreateKnight: return PlayerUnitType::Knight;
        case PlayerUnitActionType::CreateFighter: return PlayerUnitType::Fighter;
        case PlayerUnitActionType::CreateAssassin: return PlayerUnitType::Assassin;
        case PlayerUnitActionType::CreateVillage: return PlayerUnitType::Village;
        case PlayerUnitActionType::CreateBase: return PlayerUnitType::Base;
            
        default: break;
    }
    cerr << "[UnitTypeCreated] Error: Unknown PlayerUnitTypeAction is called" << endl;
    return PlayerUnitType::Unknown;
}

PlayerUnitActionType CreateAttackerAction(PlayerUnitType t) {
    switch (t) {
        case PlayerUnitType::Knight: return PlayerUnitActionType::CreateKnight;
        case PlayerUnitType::Fighter: return PlayerUnitActionType::CreateFighter;
        case PlayerUnitType::Assassin: return PlayerUnitActionType::CreateAssassin;
            
        default:
            break;
    }
    cerr << "[PlayerUnitActionType] Error: Unknown Attacker Type is called" << endl;
    return PlayerUnitActionType::None;
}

// MARK: Utility
int PlayerUnit::getHashID() {
    return utl::getHashID(position);
}

/*------------------------------------------------*/
int PlayerUnit::cost(PlayerUnitActionType at) {
    switch (at) {
            // Move Action
        case PlayerUnitActionType::MoveUp: return 0;
        case PlayerUnitActionType::MoveDown: return 0;
        case PlayerUnitActionType::MoveLeft: return 0;
        case PlayerUnitActionType::MoveRight: return 0;
            
            // Create Action
        case PlayerUnitActionType::CreateWorker: return 40;
        case PlayerUnitActionType::CreateKnight: return 20;
        case PlayerUnitActionType::CreateFighter: return 40;
        case PlayerUnitActionType::CreateAssassin: return 60;
        case PlayerUnitActionType::CreateVillage: return 100;
        case PlayerUnitActionType::CreateBase: return 500;
            
        case PlayerUnitActionType::None: return 0;
        default:
            cerr << "[PlayerUnit::cost] Error: Unknonwn PlayerUnitActionType is called" << endl;
            return 0;
    }
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



PlayerUnit::PlayerUnit() {}
PlayerUnit::PlayerUnit(int ID, Position position, PlayerUnitType type, Player *player) {
    PlayerUnit::ID = ID;
    PlayerUnit::position = position;
    PlayerUnit::type = type;
    PlayerUnit::player = player;
    status = PlayerUnitStatus::Idle;
}

PlayerUnitActionType PlayerUnit::moveToTargetAction(Position target, bool synchro) {
    int dx = target.first - position.first;
    int dy = target.second - position.second;
    
    if (dx == 0 && dy == 0) {
        return PlayerUnitActionType::None;
    }
    
    if (synchro) {
        if (dx > 0) return PlayerUnitActionType::MoveRight;
        else if (dx < 0) return PlayerUnitActionType::MoveLeft;
        else if (dy > 0) return PlayerUnitActionType::MoveDown;
        else if (dy < 0) return PlayerUnitActionType::MoveUp;
        else {
            cerr << "Error: pair of dx and dy is invalid" << endl;
            return PlayerUnitActionType::None;
        }
    } else {
        if (rand() % (abs(dx) + abs(dy)) < abs(dy)) {
            if (dy > 0) return PlayerUnitActionType::MoveDown;
            else return PlayerUnitActionType::MoveUp;
        } else {
            if (dx > 0) return PlayerUnitActionType::MoveRight;
            else return PlayerUnitActionType::MoveLeft;
        }
    }
}

void PlayerUnit::setHitPoint(int hitPoint) {
    PlayerUnit::hitPoint = hitPoint;
}

bool PlayerUnit::isMovable() {
    if (status != PlayerUnitStatus::Idle) return false;
    
    switch (type) {
        case PlayerUnitType::Worker: return true;
        case PlayerUnitType::Knight: return true;
        case PlayerUnitType::Fighter: return true;
        case PlayerUnitType::Assassin: return true;
        case PlayerUnitType::Castle: return false;
        case PlayerUnitType::Village: return false;
        case PlayerUnitType::Base: return false;
        case PlayerUnitType::Unknown: return false;
    }
    cerr << "[PlayerUnit::isMovable] Error: Unkwon Type" << endl;
    return false;
}
bool PlayerUnit::isCreatableWorker() {
    if (status == PlayerUnitStatus::Reserved) return false;
    if (player->resourceCount - PlayerUnit::cost(PlayerUnitActionType::CreateWorker) < player->necessaryResourceCount) return false;
    
    
    switch (type) {
        case PlayerUnitType::Worker: return false;
        case PlayerUnitType::Knight: return false;
        case PlayerUnitType::Fighter: return false;
        case PlayerUnitType::Assassin: return false;
        case PlayerUnitType::Castle: return true;
        case PlayerUnitType::Village: return true;
        case PlayerUnitType::Base: return false;
            
        default: break;
    }
    cerr << "[PlayerUnit::isCreatableWorker] Error: Unkwon Type" << endl;
    return false;
}

bool PlayerUnit::isCreatableVillage() {
    if (status == PlayerUnitStatus::Reserved) return false;
    if (player->resourceCount - PlayerUnit::cost(PlayerUnitActionType::CreateVillage) < player->necessaryResourceCount) return false;
    
    switch (type) {
        case PlayerUnitType::Worker: return true;
        case PlayerUnitType::Knight: return false;
        case PlayerUnitType::Fighter: return false;
        case PlayerUnitType::Assassin: return false;
        case PlayerUnitType::Castle: return false;
        case PlayerUnitType::Village: return false;
        case PlayerUnitType::Base: return false;
            
        default: break;
    }
    cerr << "[PlayerUnit::isCreatableVillage] Error: Unkwon Type" << endl;
    return false;
}
bool PlayerUnit::isCreatableAttacker(PlayerUnitType t) {
    if (status == PlayerUnitStatus::Reserved) return false;
    if (player->resourceCount - PlayerUnit::cost(CreateAttackerAction(t)) < player->necessaryResourceCount) return false;

    switch (type) {
        case PlayerUnitType::Worker: return false;
        case PlayerUnitType::Knight: return false;
        case PlayerUnitType::Fighter: return false;
        case PlayerUnitType::Assassin: return false;
        case PlayerUnitType::Castle: return false;
        case PlayerUnitType::Village: return false;
        case PlayerUnitType::Base: return true;
            
        default: break;
    }
    cerr << "[PlayerUnit::isCreatableAttacker] Error: Unkwon Type" << endl;
    return false;
}
bool PlayerUnit::isCreatableBase() {
    if (status == PlayerUnitStatus::Reserved) return false;
    if (player->resourceCount - PlayerUnit::cost(PlayerUnitActionType::CreateBase) < player->necessaryResourceCount) return false;
    
    switch (type) {
        case PlayerUnitType::Worker: return true;
        case PlayerUnitType::Knight: return false;
        case PlayerUnitType::Fighter: return false;
        case PlayerUnitType::Assassin: return false;
        case PlayerUnitType::Castle: return false;
        case PlayerUnitType::Village: return false;
        case PlayerUnitType::Base: return false;
            
        default: break;
    }
    cerr << "[PlayerUnit::isCreatableBase] Error: Unkwon Type" << endl;
    return false;
}

bool PlayerUnit::isAttacker() {
    switch (type) {
        case PlayerUnitType::Worker: return false;
        case PlayerUnitType::Knight: return true;
        case PlayerUnitType::Fighter: return true;
        case PlayerUnitType::Assassin: return true;
        case PlayerUnitType::Castle: return false;
        case PlayerUnitType::Village: return false;
        case PlayerUnitType::Base: return false;
            
        default: break;
    }
    cerr << "[PlayerUnit::isAttacker] Error: Unkwon Type" << endl;
    return false;
}

void PlayerUnit::fix(PlayerUnitActionType at) {
    player->resourceCount -= PlayerUnit::cost(at);
    status = PlayerUnitStatus::Reserved;
}
void PlayerUnit::fixOnlyPosition() {
    status = PlayerUnitStatus::FixPosition;
}
