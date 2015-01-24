//
//  Player.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/23.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include "Player.h"

Player::Player() {
    resetWithStage();
}

string castleModeString(CastleMode mode) {
    switch (mode) {
        case CastleMode::Unknown:
            return "Unknown";
        case CastleMode::Alone:
            return "Alone";
        case CastleMode::Defending:
            return "Defending";
        default:
            return "";
    }
}

// MARK: Unit
void Player::clearUnits() {
    // clear castle
    units.clear();
    attackers.clear();
    villages.clear();
    workers.clear();
    knights.clear();
    fighters.clear();
    assassins.clear();
    bases.clear();
}
void Player::updateUnit(PlayerUnit pUnit) {
    switch (pUnit.type) {
        case PlayerUnitType::Castle:
            isViewdCastle = true;
            updateType(pUnit);
            castle = pUnit;
            break;
        case PlayerUnitType::Worker:
            workers[pUnit.ID] = pUnit;
            units[pUnit.ID] = &workers[pUnit.ID];
            break;
        case PlayerUnitType::Knight:
            knights[pUnit.ID] = pUnit;
            units[pUnit.ID] = &knights[pUnit.ID];
            attackers[pUnit.ID] = &knights[pUnit.ID];
            break;
        case PlayerUnitType::Fighter:
            fighters[pUnit.ID] = pUnit;
            units[pUnit.ID] = &fighters[pUnit.ID];
            attackers[pUnit.ID] = &fighters[pUnit.ID];
            break;
        case PlayerUnitType::Assassin:
            assassins[pUnit.ID] = pUnit;
            units[pUnit.ID] = &assassins[pUnit.ID];
            attackers[pUnit.ID] = &assassins[pUnit.ID];
            break;
        case PlayerUnitType::Village:
            villages[pUnit.ID] = pUnit;
            units[pUnit.ID] = &villages[pUnit.ID];
            break;
        case PlayerUnitType::Base:
            bases[pUnit.ID] = pUnit;
            units[pUnit.ID] = &bases[pUnit.ID];
            break;
        case PlayerUnitType::Unknown:
            cerr << "[PlayerUnit::updateUnit] Error: Unkown Type" << endl;
    }
    
}

void Player::updateType(PlayerUnit p) {
    if (p.type != PlayerUnitType::Castle) return;

    if (utl::dist(p.position, Position(0, 0)) < utl::dist(p.position, Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1))) {
        type = PlayerType::Ally;
    } else {
        type = PlayerType::Enemy;
    }
}

// MARK: For enemy
void Player::updateCastleMode(vector<PlayerUnit> inputEnemyUnits) {
    Position target = castle.position;
    int castleDefenderCount = 0;
    bool castleIsViewd = false;
    vector<PlayerUnit>::iterator uIte;
    for (uIte = inputEnemyUnits.begin(); uIte != inputEnemyUnits.end(); uIte++) {
        PlayerUnit *unit = &*uIte;
        if (unit->type == PlayerUnitType::Castle) castleIsViewd = true;
        if (!unit->isAttacker()) continue;
        int d = utl::dist(unit->position, target);
        if (d <= 2) castleDefenderCount++;
    }
    if (castleIsViewd && castleDefenderCount >= 3) castleMode = CastleMode::Defending;
    else if (castleIsViewd) castleMode = CastleMode::Alone;
}


// MARK: Reset
void Player::resetWithStage() {
    type = PlayerType::Unknown;
    isViewdCastle = false;
    resourceCount = 0;
    necessaryResourceCount = 0;
    castleMode = CastleMode::Unknown;
    
    clearUnits();
    resetCastle();
}
void Player::resetCastle() {
    if (type == PlayerType::Ally) {
        castle = PlayerUnit(INF, Position(0, 0), PlayerUnitType::Castle, this);
    } else if (type == PlayerType::Enemy) {
        castle = PlayerUnit(INF, Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1), PlayerUnitType::Castle, this);
    } else {
        // Default Enemy
        castle = PlayerUnit(INF, Position(MAX_FIELD_WIDTH - 1, MAX_FIELD_HEIGHT - 1), PlayerUnitType::Castle, this);
    }
}

void Player::resetWithTurn() {
    clearUnits();
    necessaryResourceCount = 0;
}

// MARK: Calc Count
int Player::calcWorkerCount() {
    return (int)workers.size();
}

int Player::calcVillageCount() {
    return (int)villages.size();
}

int Player::calcBaseCount() {
    return (int)bases.size();
}
int Player::calcAssassinCount() {
    return (int)assassins.size();
}
int Player::calcAttackerCount() {
    return (int)attackers.size();
}
