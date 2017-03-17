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
using namespace UsefulFunctions;

void MySystem::Tactics::executeTacticsScoredGoal(Information& information, AllSystem& system) {
    TacticsHockeyists tactHock = system.getTacticsHockeyists();
    Hockeyist* myOtherHockeyist = getMyOtherHockeyist(information.self, information.world);

    double posX = myOtherHockeyist->getX();
    double posY = myOtherHockeyist->getY();
    posX += myOtherHockeyist->getRadius() * sin(myOtherHockeyist->getAngle()); 
    posY += myOtherHockeyist->getRadius() * cos(myOtherHockeyist->getAngle()); 
    information.move.setTurn(information.self.getAngleTo(posX, posY));
    information.move.setSpeedUp(getEffectiveSpeedByRange(information.self.getDistanceTo(posX, posY)));
}
