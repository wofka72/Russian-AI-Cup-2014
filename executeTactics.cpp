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

void MySystem::Tactics::executeTactics(Information& information, AllSystem& system) {

    if ((information.self.getState() == HockeyistState::SWINGING) &&
        (information.self.getId() != information.world.getPuck().getOwnerHockeyistId())) {

        information.move.setAction(ActionType::CANCEL_STRIKE);
    }
    else {

        switch (system.getCurrentTactics()) {
            case EXTENDED:
                executeTacticsExtended(information, system);
                break;
            case PRESSING:
                executeTacticsPressing(information, system);
                break;
            case MISSED_GOAL:
                executeTacticsMissedGoal(information, system);
                break;
            case SCORED_GOAL:
                executeTacticsScoredGoal(information, system);
                break;

            case CATCH_PUCK:
                executeTacticsCatchPuck(information, system);
                break;
            case ATTACK:
                executeTacticsAttack(information, system);
                break;
            case DEFENCE:
                executeTacticsDefence(information, system);
                break;

            case CONTRATTACK:
                executeTacticsContrattack(information, system);
                break;
            case HOLD_PUCK:
                executeTacticsHoldPuck(information, system);
                break;

            default:
                executeTacticsDefault(information, system);
        }
    }

    if (information.self.getState() == HockeyistState::SWINGING) {
        if (information.self.getSwingTicks() > information.game.getMaxEffectiveSwingTicks()
            || information.self.getId() != information.world.getPuck().getOwnerHockeyistId()) {
            
            information.move.setAction(ActionType::CANCEL_STRIKE);
        }
    }
}
