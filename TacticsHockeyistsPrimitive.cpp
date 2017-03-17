#include "MyStrategy.h"

#include "MySystem.h"
#include "UsefulFunctions.h"
#include "constants.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace std;
using namespace model;
using namespace UsefulFunctions;
using namespace MySystem;


void TacticsHockeyists::moveApproxToSector(Information& information, AllSystem& system, const Sector& sector) {
    const Hockeyist& self = information.self;
    Move& move = information.move;
    
    double range = self.getDistanceTo(sector.x, sector.y);
    double angle = self.getAngleTo(sector.x, sector.y);

    if (angle > MIN_ANGLE_ANGLE_TO_SPEED) {
        move.setSpeedUp(getEffectiveSpeedByAngle(angle));
    }
    else {
        move.setSpeedUp(getEffectiveSpeedByRange(range));
    }

    move.setTurn(angle);
}

void TacticsHockeyists::moveQuickToSector(Information& information, AllSystem& system, const Sector& sector) {
    const Hockeyist& self = information.self;
    Move& move = information.move;

    double angle = self.getAngleTo(sector.x, sector.y);

    move.setSpeedUp(getEffectiveSpeedByAngle(angle));
    move.setTurn(angle);
}

void TacticsHockeyists::moveToEnemy(Information& information, AllSystem& system, Hockeyist* enemy) {
    Sector enemySector = getPrognosedSectorUnit(information, information.self, *enemy, false);
    moveQuickToSector(information, system, enemySector);
}

void TacticsHockeyists::turnToSector(Information& information, const Sector& sector) {
    information.move.setTurn(information.self.getAngleTo(sector.x, sector.y));
}

void TacticsHockeyists::turnToPuck(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    double angleToPuck = getEffectiveAngleToPuck(information);

    move.setTurn(angleToPuck);
}

void TacticsHockeyists::passToOther(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;
    
    Hockeyist* mySecondHockeyist = getMyOtherHockeyist(self, world);

    double angleToSH = getEffectivePassAngle(information, *mySecondHockeyist);
    if (abs(angleToSH) <= game.getPassSector() / 2) {
        move.setPassAngle(getEffectivePassAngle(information, *mySecondHockeyist));
        move.setPassPower(getEffectivePassPower(information, *mySecondHockeyist));
        move.setAction(ActionType::PASS);
        system.setJustPassed(true);
        system.setPassingNeed(false);
    }
        move.setTurn(angleToSH);
}

void TacticsHockeyists::moveToMovingSector(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    int selfId = self.getTeammateIndex();
    const Sector movingSector = system.getMovingSector(selfId);
    
    moveQuickToSector(information, system, movingSector);
}
