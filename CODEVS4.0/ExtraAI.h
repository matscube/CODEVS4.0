//
//  ExtraAI.h
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2015/01/17.
//  Copyright (c) 2015年 TakanoriMatsumoto. All rights reserved.
//

#ifndef __CODEVS4_0__ExtraAI__
#define __CODEVS4_0__ExtraAI__

#include "Game.h"
#include "Field.h"
#include "Player.h"
#include "Library.h"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

typedef pair<int, PlayerUnitActionType> Command;

class ExtraAI {
    Game *game;
    Field *field;
    Player *player;
    Player *enemy;
    vector<Command> commands;
    void addCommand(Command newCommand);
public:
    ExtraAI(Game &game, Field &field, Player &player, Player &enemy);
    void resetWithTurn();
    void resetWithStage();
    vector<Command> getCommands();
    
    // MARK: Basic Command
    void addCommandMove(PlayerUnit *pUnit, Position target, bool synchro = false, bool formed = false);
    void addCommandMove(PlayerUnit *pUnit, PlayerUnitActionType moveAction);
    void addCommandCreateVillage(PlayerUnit *pUnit);
    void addCommandCreateBase(PlayerUnit *pUnit);
    void addCommandCreateWorker(PlayerUnit *pUnit);
    void addCommandCreateAttacker(PlayerUnit *pUnit, PlayerUnitType unitType);
//    void moveCommand(Position target, vector<PlayerUnitType> types, int assign)
    
    // MARK: Cost limit
    void setCostLimit(int limit);
    
    // MARK: Defend
    int defenderBaseCount(Position position);
    int createCastleBaseCommand(int prob);
    int createBaseCommand(Position position, int prob);
    int createDefenderCommand(Position position, int assign, int prob);
    int calcCastleDefender();
    void defendCastleCommand(int assign); // moveCommand
    
    // MARK: Utility
    map<PlayerUnitType, bool> allTypes();
    map<PlayerUnitType, bool> workerTypes();
    map<PlayerUnitType, bool> attackerTypes();

    // MARK: Search
    void searchNoVisitedAreaCommand(vector<Position> area, int assign, map<PlayerUnitType, bool> types);
    vector<Position> searchLineToRight1();
    vector<Position> searchLineToRight2();
    vector<Position> searchLineToRight3();
    vector<Position> searchLineToRight4();
    vector<Position> searchLineToRight5();
    vector<Position> searchLineToRight6();
    vector<Position> searchLineToRight7();
    vector<Position> searchLineToRight8();
    vector<Position> searchLineToRight9();
    vector<Position> searchLineToRight10();
    vector<Position> searchLineToDown1();
    vector<Position> searchLineToDown2();
    vector<Position> searchLineToDown3();
    vector<Position> searchLineToDown4();
    vector<Position> searchLineToDown5();
    vector<Position> searchLineToDown6();
    vector<Position> searchLineToDown7();
    vector<Position> searchLineToDown8();
    vector<Position> searchLineToDown9();
    vector<Position> searchLineToDown10();
    vector<Position> searchLineAlly1();
    vector<Position> searchLineAlly2();
    void searchUnkownFieldSmallCommand(int assign);
    void searchUnkownFieldMediumCommand(int assign);
    void searchUnkownFieldAllCommand(int assign);
    
    vector<Position> searchEnemyCastleLine1();
    vector<Position> searchEnemyCastleLine2();
    vector<Position> searchEnemyCastleLine3();
    vector<Position> searchEnemyCastleLine4();
    vector<Position> searchEnemyCastleLine5();
    void searchEnemyCastleCommand();

    // MARK: Enemy
    int nearestEnemyDistance;
    int enemyCountToAllyCaslte;
    void updateEnemy();
    
    // MARK: Attacker
    vector<PlayerUnitType> attackerTypesPack(int knight, int fighter, int assasin);
    
    // MARK: Resource
    int calcResourceGetting();
    int createVillageOnResourceCommand(int assign, int prob);
    int getResourceCommand(int prob);
    void defendResourceCommand(int assign);
    int workerCreationCount[MAX_FIELD_WIDTH][MAX_FIELD_HEIGHT];
    

    // MARK: temp
    int supplyFreeWorkerWithVillageCommand(int need, int prob);
    
    // MARK: Attack
    vector<Position> backAttackMoveLineRight();
    vector<Position> backAttackMoveLineDown();
    void backAttackMoveRightCommand();
    void backAttackMoveDownCommand();
    vector<PlayerUnit *> attackBases();
    int calcAttackBase();
    void createOneAttackerBase();
    void createOneMoreAttackerBase();
    vector<PlayerUnitType> defaultAttackerPack();
    vector<PlayerUnitType> assassinAttackerPack();
    void createAttackerCommand();
    void attackCastleCommand();
    bool poolReleased;
    bool poolAttackerCommand(int need);
    void reunionAttackerCommand();
    
    
    // Worker
    void supplyWorkerForSearchCommand(int need);

    
    // MARK: Debug
    void debug();
    
};


#endif /* defined(__CODEVS4_0__ExtraAI__) */


