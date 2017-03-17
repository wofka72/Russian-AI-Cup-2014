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

void MySystem::Tactics::executeTacticsDefence(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;
    Move& move = information.move;
    TacticsHockeyists tactHock = system.getTacticsHockeyists();

    int selfId = self.getTeammateIndex();

    
    switch (system.getHockeyistsRole(selfId)) {

        case ATTACKER:
        {
            system.setMovingSectorChoosed(selfId, false);
            Hockeyist* enemy = getHockeyistById(world.getPuck().getOwnerHockeyistId(), world);
            tactHock.moveToEnemy(information, system, enemy);
            tactHock.strikePuckAndOwner(information, system);
        }
            break;

        case SUPPORT:

            break;

        case DEFENCER:

            if (isOpponentInAttackDangerousDefencer(information, system) == false) {
                if (isInSector(information, system.getMovingSector(selfId))) {
                    tactHock.turnToPuck(information, system);
                    tactHock.strikePuckAndOwner(information, system);
                }
                else {
                    tactHock.moveToMyNet(information, system);
                }
            }
            else { // opponent is dangerous
            //    tactHock.moveToOppAttackSector(information, system);
            //    tactHock.pushEnemy(information, system, getHockeyistById(world.getPuck().getOwnerHockeyistId(), world));
            //    tactHock.defenceMyNetDefencer(information, system);
                system.setMovingSectorChoosed(selfId, false);
                tactHock.moveToPuck(information, system);
                tactHock.strikePuckAndOwner(information, system);
            }
            
            break;

        case NONE_ROLE:

            break;
    }
}
