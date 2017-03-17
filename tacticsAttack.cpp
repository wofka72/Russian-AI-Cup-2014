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

void MySystem::Tactics::executeTacticsAttack(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;
    Move& move = information.move;
    TacticsHockeyists tactHock = system.getTacticsHockeyists();

    int selfId = self.getTeammateIndex();
    

    switch (system.getHockeyistsRole(selfId)) {

        case ATTACKER:

            checkAttack(information, system);
            
            if (system.getHockeyistsInAttack(selfId)[0] == true) {
                if (isInStrikeZone(information, system, self) == false) {
                    system.resetHockeyistState(information, system, selfId);
                }
            }
            
            if (system.isJustPassed()) {
                changeByRolesWith(information, system, DEFENCER);
                system.setJustPassed(false);
            }
            else if (system.isHockeyistsInStrike(selfId)) {
                tactHock.strikePuckAttacker(information, system);
            }
            else if (isOpponentDangerousPass(information, system) /*|| system.isPassingNeed()*/) {
                //system.setPassingNeed(true);
                tactHock.passToOther(information, system);
            }

            system.setPassingNeed(false);
            tactHock.moveToAttack(information, system);
            
            break;

        case SUPPORT:

            break;

        case DEFENCER:

            if (isChangeByRolesNeed(information, system)) {
                changeByRolesWith(information, system, ATTACKER);
            }
            else if (world.getPuck().getOwnerHockeyistId() == self.getId()) {
                if (isOpponentDangerousPass(information,system)) {
                    tactHock.passToOther(information, system);
                }
                else {
                    changeByRolesWith(information, system, ATTACKER);
                }
            }
            else if (system.isPassingNeed()) {
                tactHock.turnToPuck(information, system);
                tactHock.catchPuck(information, system);
            }
            else if (isInSector(information, system.getMovingSector(selfId))) {
                tactHock.turnToPuck(information, system);
                tactHock.strikePuckDefencer(information, system);
            }
            else {
                tactHock.moveToMyNet(information, system);
            }

            break;

        case NONE_ROLE:

            break;
    }
}
