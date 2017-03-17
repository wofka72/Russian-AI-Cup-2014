#include "MyStrategy.h"

#include "MySystem.h"
#include "UsefulFunctions.h"
#include "Information.h"
#include "constants.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace std;
using namespace model;
using namespace UsefulFunctions;
using namespace MySystem;


void TacticsHockeyists::moveToPuck(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    Puck puck = world.getPuck();
    int selfId = self.getTeammateIndex();
    
    Sector puckSector = getPuckSector(information);

    system.setMovingSector(selfId, puckSector);
    system.setMovingSectorChoosed(selfId, true);

    moveQuickToSector(information, system, puckSector);
}

void TacticsHockeyists::catchPuck(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    Puck puck = world.getPuck();
    double distanceToPuck = self.getDistanceTo(puck);
    double angleToPuck = self.getAngleTo(puck);

    if (puck.getOwnerPlayerId() == world.getOpponentPlayer().getId()) {
        if (distanceToPuck <= game.getStickLength()
            && angleToPuck <= game.getStickSector() / 2) {

            move.setAction(ActionType::STRIKE);
        }
    }
    else if (puck.getOwnerPlayerId() < 0) {
        move.setAction(ActionType::TAKE_PUCK);
    }
}
