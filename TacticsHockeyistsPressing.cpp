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


void TacticsHockeyists::pushEnemy(Information& information, AllSystem& system, Hockeyist* enemy) {
    const Hockeyist& self = information.self; 
    const World& world = information.world; 

    if (enemy == NULL) {
        enemy = getNearestOpponent(self.getX(), self.getY(), world);
    }

    moveToEnemy(information, system, enemy);
    strikeUnit(information, *enemy);
}

void TacticsHockeyists::strikeUnit(Information& information, const Unit& unit) {
    const Hockeyist& self = information.self;
    const Game& game = information.game;

    double angleToUnit = self.getAngleTo(unit);
    double rangeToUnit = self.getDistanceTo(unit);

    if (rangeToUnit <= game.getStickLength() && angleToUnit <= game.getStickSector() / 2) {
        information.move.setAction(ActionType::STRIKE);
    }
}

void TacticsHockeyists::strikePuckAndOwner(Information& information, AllSystem& system) {
    const World& world = information.world;
    Puck puck = world.getPuck();
    Hockeyist* puckOwner = getHockeyistById(puck.getOwnerHockeyistId(), world);

    if (puckOwner->isTeammate() == false) {
        strikeUnit(information, *puckOwner);
        strikeUnit(information, puck);
    }
}
