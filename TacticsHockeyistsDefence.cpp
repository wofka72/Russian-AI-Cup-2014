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


void TacticsHockeyists::moveToMyNet(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;
    Move& move = information.move;

    int selfId = self.getTeammateIndex();
    Sector defenceSector = system.getMovingSector(selfId);

    if (system.isMovingSectorChoosed(selfId) == false) {
        defenceSector = getDefenceSector(information, system);
        system.setMovingSector(selfId, defenceSector);
        system.setMovingSectorChoosed(selfId, true);
    }

    moveApproxToSectorDefence(information, system, defenceSector);
}

void TacticsHockeyists::strikePuckDefencer(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;
    Move& move = information.move;

    Puck puck = world.getPuck();

    double angleToPuck = self.getAngleTo(puck);
    double distanceToPuck = self.getDistanceTo(puck);
    if (angleToPuck <= game.getStickSector() / 2 && distanceToPuck <= game.getStickLength()) {
        if (isPuckDangerous(information)) {
            move.setAction(ActionType::STRIKE);
        }
        else {
            catchPuck(information, system);
        }
    }
}

void TacticsHockeyists::moveApproxToSectorDefence(Information& information, AllSystem& system, Sector defenceSector) {
    const Hockeyist& self = information.self;
    Move& move = information.move;
    
    double range = self.getDistanceTo(defenceSector.x, defenceSector.y);
    double angle = self.getAngleTo(defenceSector.x, defenceSector.y);

    if (range < 200.0) {
        if (angle < 0.0) {
            move.setTurn(angle + PI);
        }
        else {
            move.setTurn(angle - PI);
        }

        move.setSpeedUp(-1.0);
    }
    else if (angle > MIN_ANGLE_ANGLE_TO_SPEED) {
        move.setSpeedUp(getEffectiveSpeedByAngle(angle));
        move.setTurn(angle);
    }
    else {
        move.setSpeedUp(getEffectiveSpeedByRange(range));
        move.setTurn(angle);
    }


}

void TacticsHockeyists::moveToOppAttackSector(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    const World& world = information.world;
    
    int selfId = self.getTeammateIndex();
    Sector oppAttackerSector = system.getMovingSector(selfId);

    Hockeyist* nearestOpp = getHockeyistById(world.getPuck().getOwnerHockeyistId(), world);
    double range = self.getDistanceTo(*nearestOpp);

    if (range < 150.0) {
        
    }

    if (system.isMovingSectorChoosed(selfId) == false) {
        oppAttackerSector = getOpponentAttackSector(information, system);
        system.setMovingSector(selfId, oppAttackerSector);
        system.setMovingSectorChoosed(selfId, true);
    }
    
    moveQuickToSector(information, system, oppAttackerSector);
}

void TacticsHockeyists::moveToPuckDefencer(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    const World& world = information.world;
    Move& move = information.move;

    Puck puck = world.getPuck();
    int selfId = self.getTeammateIndex();

    Hockeyist* puckOwner = getHockeyistById(puck.getOwnerHockeyistId(), world);

    double posHockX = puckOwner->getX();
    double posHockY = puckOwner->getY();
    double speedHockX = puckOwner->getSpeedX();
    double speedHockY = puckOwner->getSpeedY();

    for (int i = 0; i < TICKS_PROGNOSE; ++i) {
        posHockX += speedHockX;
        posHockY += speedHockY;

        speedHockX *= 0.98;
        speedHockY *= 0.98;
    }

    Sector puckForDef = { posHockX, posHockY, 50 };

    moveQuickToSector(information, system, puckForDef);
}

void TacticsHockeyists::defenceMyNetDefencer(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    const World& world = information.world;

    Puck puck = information.world.getPuck();
    Hockeyist* puckOwner = getHockeyistById(puck.getOwnerHockeyistId(), world);

    double range = self.getDistanceTo(*puckOwner);

    if (range < 250.0) {
        moveToPuck(information, system);
        strikePuckAndOwner(information, system);
    }
    else {
        moveToPuckDefencer(information, system);
    }
}