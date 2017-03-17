#include "MyStrategy.h"

#include "AllSystem.h"
#include "UsefulFunctions.h"
#include "constants.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace std;
using namespace model;
using namespace MySystem;
using namespace UsefulFunctions;


void TacticsHockeyists::moveToAttack(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    int selfId = self.getTeammateIndex();

    bool wasInAttackSector = system.getHockeyistsInAttack(information.self.getTeammateIndex())[0];
    bool wasInStrikeSector = system.getHockeyistsInAttack(information.self.getTeammateIndex())[1];

    if (wasInAttackSector) {
        moveToStrikeSector(information, system);
        system.setMovingSectorChoosed(selfId, false);
    }
    else {
        moveToAttackSector(information, system);
    }
}

void TacticsHockeyists::moveToAttackSector(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;
    
    int selfId = self.getTeammateIndex();
    Sector attackSector = system.getMovingSector(selfId);

    if (system.isMovingSectorChoosed(selfId) == false) {
        attackSector = getAttackSector(information, system);
        system.setMovingSector(selfId, attackSector);
        system.setMovingSectorChoosed(selfId, true);
    }

    moveQuickToSector(information, system, attackSector);
}

void TacticsHockeyists::moveToStrikeSector(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    int selfId = self.getTeammateIndex();
    Sector strikeSector = system.getMovingSector(selfId);

    if (system.isMovingSectorChoosed(selfId) == false) {
        strikeSector = getStrikeSector(information, system);
        system.setMovingSector(selfId, strikeSector);
        system.setMovingSectorChoosed(selfId, false);
    }

    moveQuickToSector(information, system, strikeSector);
}

void TacticsHockeyists::strikePuckAttacker(Information& information, AllSystem& system) {
    int selfId = information.self.getTeammateIndex();

    if (getEffectiveSwingingTicks(information, system) <= information.self.getSwingTicks()) {
        information.move.setAction(ActionType::STRIKE);        
    }
    else {
        information.move.setAction(ActionType::SWING);
    }
}

void TacticsHockeyists::moveToSupportSector(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;
    
    int selfId = self.getTeammateIndex();
    Sector supportSector = system.getMovingSector(selfId);

    if (system.isMovingSectorChoosed(selfId) == false) {
        supportSector = getSupportSector(information, system);
        system.setMovingSector(selfId, supportSector);
        system.setMovingSectorChoosed(selfId, true);
    }

    moveQuickToSector(information, system, supportSector);
}

void TacticsHockeyists::mayBeContinueAttack(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    int selfId = self.getTeammateIndex();
    Sector strikeSector = getStrikeSector(information, system);
    double angleToStrikeSector = self.getAngleTo(strikeSector.x, strikeSector.y);

    if (abs(angleToStrikeSector) < PI / 2) {
        system.setHockeyistsInAttack(selfId, 0, true);
        system.setHockeyistsInAttack(selfId, 0, true);   
    }
}
