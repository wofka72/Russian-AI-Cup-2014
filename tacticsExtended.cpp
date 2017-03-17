#include "MyStrategy.h"

#include "AllSystem.h"
#include "UsefulFunctions.h"
#include "constants.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace std;
using namespace model; using namespace UsefulFunctions;

void MySystem::Tactics::executeTacticsExtended(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    if (self.getState() == HockeyistState::SWINGING) {
        move.setAction(ActionType::STRIKE);
        return;
    }

    if (world.getPuck().getOwnerPlayerId() == self.getPlayerId()) {
        if (world.getPuck().getOwnerHockeyistId() == self.getId()) {
            Player* opponentPlayer = new Player(world.getOpponentPlayer());

            double netX = 0.5 * (opponentPlayer->getNetBack() + opponentPlayer->getNetFront());
            double netY = 0.5 * (opponentPlayer->getNetBottom() + opponentPlayer->getNetTop());
            netY += (self.getY() < netY ? 0.5 : -0.5) * game.getGoalNetHeight();

            double angleToNet = self.getAngleTo(netX, netY);
            move.setTurn(angleToNet);

            if (abs(angleToNet) < game.getStrikeAngleDeviation()) {
                move.setAction(ActionType::SWING);
            }
        } else {
            Hockeyist* nearestOpponent = getNearestOpponent(self.getX(), self.getY(), world);
            if (nearestOpponent != NULL) {
                if (self.getDistanceTo(*nearestOpponent) > game.getStickLength()) {
                    move.setSpeedUp(1.0);
                } else if (abs(self.getAngleTo(*nearestOpponent)) < 0.5 * game.getStickSector()) {
                    move.setAction(ActionType::STRIKE);
                }
                move.setTurn(self.getAngleTo(*nearestOpponent));
            }
        }
    } else {
        move.setSpeedUp(1.0);
        move.setTurn(self.getAngleTo(world.getPuck()));
        move.setAction(TAKE_PUCK);
    }
}
