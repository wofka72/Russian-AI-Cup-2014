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
using namespace MySystem;

void MySystem::Tactics::computeOptimalTactics(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;
    Move& move = information.move;

    // ***************************************************
    // There we should to choose global tactic of behavior
    // ***************************************************

    
    Puck puck = world.getPuck();

    if (world.getMyPlayer().isJustMissedGoal()) {
        system.setCurrentTactics(MISSED_GOAL);

        if (system.getPreviousTactics() != system.getCurrentTactics()) {
            system.resetMyHockeyistsState(information, system);
        }
        system.setRolesAreBad(true);
    }
    else if (world.getMyPlayer().isJustScoredGoal()) {
        system.setCurrentTactics(SCORED_GOAL);

        if (system.getPreviousTactics() != system.getCurrentTactics()) {
            system.resetMyHockeyistsState(information, system);
        }
        system.setRolesAreBad(true);
    }

    else if (puck.getOwnerPlayerId() == world.getMyPlayer().getId()) {

        system.setCurrentTactics(ATTACK);
        
        if (system.getPreviousTactics() != system.getCurrentTactics()) {
            system.resetMyHockeyistsState(information, system);
        }

        int puckOwnerId = puck.getOwnerHockeyistId();
        Hockeyist* puckOwner = getHockeyistById(puckOwnerId, world);
        Hockeyist* otherTeammate = getMyOtherHockeyist(*puckOwner, world);

        system.setHockeyistsRole(puckOwner->getTeammateIndex(), HockeyistRole::ATTACKER);
        system.setHockeyistsRole(otherTeammate->getTeammateIndex(), HockeyistRole::DEFENCER);
    }
    else if (puck.getOwnerPlayerId() == world.getOpponentPlayer().getId()) {

        system.setCurrentTactics(DEFENCE);
        
        if (system.getPreviousTactics() != system.getCurrentTactics()) {
            system.resetMyHockeyistsState(information, system);
        }

        int puckOwnerId = puck.getOwnerHockeyistId();
        Hockeyist* puckOwnerOpponent = getHockeyistById(puckOwnerId, world);
        Hockeyist* nearestToPuckTeammate = getNearestTeammate(puck.getX(), puck.getY(), world);
        Hockeyist* otherTeammate = getMyOtherHockeyist(*nearestToPuckTeammate, world);
        int nearestToPuckTeamId= nearestToPuckTeammate->getTeammateIndex();

        system.setHockeyistsRole(nearestToPuckTeamId, HockeyistRole::ATTACKER);
        system.setHockeyistsRole(otherTeammate->getTeammateIndex(), HockeyistRole::DEFENCER);
    }
    else {

        system.setCurrentTactics(CATCH_PUCK);
        
        if (system.getPreviousTactics() != system.getCurrentTactics()) {
            system.resetMyHockeyistsState(information, system);
        }

        if (system.areRolesBad()) {
            Hockeyist* myOtherHockeyist = getMyOtherHockeyist(self, world);

            int selfId = self.getTeammateIndex();
            int otherId = myOtherHockeyist->getTeammateIndex();

            double rangeFromOtherToPuck = myOtherHockeyist->getDistanceTo(puck);
            double rangeFromSelfToPuck = self.getDistanceTo(puck);

            if (rangeFromOtherToPuck < rangeFromSelfToPuck) {
                system.setHockeyistsRole(selfId, DEFENCER);
                system.setHockeyistsRole(otherId, ATTACKER);
            }
            else {
                system.setHockeyistsRole(selfId, ATTACKER);
                system.setHockeyistsRole(otherId, DEFENCER);
            }

            system.setRolesAreBad(false);
        }
    }

    system.setTacticsChoosed(true);

}
