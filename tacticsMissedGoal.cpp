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

void MySystem::Tactics::executeTacticsMissedGoal(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;
    Move& move = information.move;
    TacticsHockeyists tactHock = system.getTacticsHockeyists();

    for (Hockeyist hockeyist : world.getHockeyists()) {
        if (hockeyist.isTeammate() == false) {
            if (hockeyist.getTeammateIndex() == self.getTeammateIndex()) {
                Hockeyist* enemy = new Hockeyist(hockeyist);

                tactHock.pushEnemy(information, system, enemy);
            }
        }
    }
}
