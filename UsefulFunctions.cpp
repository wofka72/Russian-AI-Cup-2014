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

Hockeyist* UsefulFunctions::getNearestOpponent(const double x, const double y, const World& world) {
    Hockeyist* nearestOpponent = NULL;
    double nearestOpponentRange = 0.0;

    for (Hockeyist hockeyist : world.getHockeyists()) {
        if (hockeyist.isTeammate() || hockeyist.getType() == HockeyistType::GOALIE
            || hockeyist.getState() == HockeyistState::RESTING) {
            continue;
        }

        double opponentRange = hypot(x - hockeyist.getX(), y - hockeyist.getY());

        if (nearestOpponent == NULL || opponentRange < nearestOpponentRange) {
            nearestOpponent = new Hockeyist(hockeyist);
            nearestOpponentRange = opponentRange;
        }
    }

    return nearestOpponent;
}

Hockeyist* UsefulFunctions::getNearestTeammate(const double x, const double y, const World& world) {
    Hockeyist* nearestTeammate = NULL;
    double nearestTeammateRange = 0.0;

    for (Hockeyist hockeyist : world.getHockeyists()) {
        if (!hockeyist.isTeammate() || hockeyist.getType() == HockeyistType::GOALIE
            || hockeyist.getState() == HockeyistState::RESTING) {
            continue;
        }

        double teammateRange = hypot(x - hockeyist.getX(), y - hockeyist.getY());

        if (nearestTeammate == NULL || teammateRange < nearestTeammateRange) {
            nearestTeammate = new Hockeyist(hockeyist);
            nearestTeammateRange = teammateRange;
        }
    }

    return nearestTeammate;
}

Hockeyist* UsefulFunctions::getMyOtherHockeyist(const Hockeyist& thisHockeyist, const World& world) {
    Hockeyist* myOtherHockeyist = NULL;

    for (Hockeyist hockeyist : world.getHockeyists()) {
        if (hockeyist.isTeammate() && hockeyist.getId() != thisHockeyist.getId()
            && hockeyist.getState() != HockeyistState::RESTING
            && hockeyist.getType() != HockeyistType::GOALIE) {

            myOtherHockeyist = new Hockeyist(hockeyist);
            break;
        }
    }

    return myOtherHockeyist;
}

Hockeyist* UsefulFunctions::getHockeyistById(const int hockeyistId, const World& world) {
    Hockeyist* hockeyistById = NULL;

    for (Hockeyist hockeyist : world.getHockeyists()) {
        if (hockeyist.getId() == hockeyistId) {
            hockeyistById = new Hockeyist(hockeyist);
            break;
        }
    }

    return hockeyistById;
}

Hockeyist* getMyHockeyistByRole(const Information& information, AllSystem& system, const HockeyistRole role) {
    Hockeyist* hockeyistByRole = NULL;

    for (Hockeyist hockeyist : information.world.getHockeyists()) {
        if (hockeyist.isTeammate()) {
            int hockeyistIndex = hockeyist.getTeammateIndex();
            if (system.getHockeyistsRole(hockeyistIndex) == role) {
                hockeyistByRole = new Hockeyist(hockeyist);
                break;
            }
        }
    }

    return hockeyistByRole;
}


double UsefulFunctions::getEffectiveSpeedByAngle(const double angle) {
    double effectiveSpeed = PI / 2 - abs(angle);
    effectiveSpeed *= abs(effectiveSpeed);
    return effectiveSpeed;
}

double UsefulFunctions::getEffectiveSpeedByRange(const double range) {
    double effectiveSpeed = (range - MIN_RANGE_RANGE_TO_SPEED) / (MAX_RANGE_RANGE_TO_SPEED - MIN_RANGE_RANGE_TO_SPEED);
    return effectiveSpeed;
}

double UsefulFunctions::getEffectiveAngleToPuck(Information& information) {
    Puck puck = information.world.getPuck();
    double effectiveAngleToPuck = information.self.getAngleTo(puck);
    return effectiveAngleToPuck;
}

double UsefulFunctions::getEffectivePassAngle(Information& information, const Hockeyist& hockeyist) {

    const Hockeyist& self = information.self;

    double posHockX = hockeyist.getX(); 
    double posHockY = hockeyist.getY(); 

    if (self.getDistanceTo(hockeyist) < 300.0) {
        double hockAngle = hockeyist.getAngle();
        posHockX += 80.0 * cos(hockAngle);
        posHockY += 80.0 * sin(hockAngle);
    }

    double speedXHock = hockeyist.getSpeedX();
    double speedYHock = hockeyist.getSpeedY();

    double newPosHockX = posHockX;
    double newPosHockY = posHockY;

    double angleSelfSpeed = atan2(self.getSpeedY(), self.getSpeedX());
    double anglePass = self.getAngleTo(hockeyist);

    double speedSelf = hypot(self.getSpeedX(), self.getSpeedY());
    double rangeToHockeyist = self.getDistanceTo(hockeyist);

    double speedPuck = 15.0 * getEffectivePassPower(information, hockeyist) + speedSelf * cos(anglePass - angleSelfSpeed);
    int ticksPrognose = rangeToHockeyist / speedPuck;
    for (int i = 0; i < ticksPrognose; ++i) {
        newPosHockX += speedXHock;
        newPosHockY += speedYHock;
        speedXHock *= 0.95;
        speedYHock *= 0.95;
    }

    double angleForPass = self.getAngleTo(newPosHockX, newPosHockY);

    return angleForPass;
}
  
double UsefulFunctions::getEffectivePassPower(Information& information, const Hockeyist& hockeyist) {
    const Hockeyist& self = information.self;
    double distance = self.getDistanceTo(hockeyist);
    double passPower;

    if (distance >= RANGE_MAX_PASS_POWER) {
        passPower = MAX_PASS_POWER;
    }
    else if (distance <=  RANGE_MIN_PASS_POWER) {
        passPower = MIN_PASS_POWER;
    }
    else {
        double a = (MAX_PASS_POWER - MIN_PASS_POWER) / (RANGE_MAX_PASS_POWER - RANGE_MIN_PASS_POWER);
        double b = MAX_PASS_POWER - RANGE_MAX_PASS_POWER * a;
        passPower = a * distance + b;
    }

    return passPower;
}

int UsefulFunctions::getEffectiveSwingingTicks(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    return (information.game.getMaxEffectiveSwingTicks());
}

AttackSideType UsefulFunctions::getEffectiveAttackSide(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    const World& world = information.world;

    AttackSideType attackSide;

    Sector attackSectorTop = getAttackSectorTop(information, system);
    Sector attackSectorBottom = getAttackSectorBottom(information, system);
    
    int puckOwnerId = world.getPuck().getOwnerHockeyistId();
    Hockeyist* puckOwner = getHockeyistById(puckOwnerId, world);
    Hockeyist* nearestOpponent = getNearestOpponent(puckOwner->getX(), puckOwner->getY(), world);
    
    double angleToNearestOpponent = puckOwner->getAngleTo(*nearestOpponent);
    double angleToTop = puckOwner->getAngleTo(attackSectorTop.x, attackSectorTop.y);
    double angleToBottom = puckOwner->getAngleTo(attackSectorBottom.x, attackSectorBottom.y);
    
    if (abs(angleToTop - angleToNearestOpponent) > abs(angleToBottom - angleToNearestOpponent)) {
        attackSide = AttackSideType::TOP;   
    }
    else {
        attackSide = AttackSideType::BOTTOM;   
    }
    return attackSide;
}

DefenceSideType UsefulFunctions::getEffectiveDefenceSide(Information& information, AllSystem& system) {
    return (DefenceSideType::DEFENCE_CENTER);
}

OpponentAttackSideType UsefulFunctions::getEffectiveOpponentAttackSide(Information& information) {
    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;

    Puck puck = world.getPuck();
    Hockeyist* puckOwner = getHockeyistById(puck.getOwnerHockeyistId(), world);
    
    if (puckOwner != NULL) {
        if (puckOwner->isTeammate() == false) {
            double yCenter = 0.5 * (game.getRinkTop() + game.getRinkBottom());

            if (puckOwner->getY() >= yCenter - 100.0
                && puckOwner->getSpeedY() >= 0.0) {
                return OpponentAttackSideType::BOTTOM_OPP;
            }
            else if (puckOwner->getY() <= yCenter + 100.0
                && puckOwner->getSpeedY() <= 0.0) {
                return OpponentAttackSideType::TOP_OPP;
            }
        }
    }

    return OpponentAttackSideType::TOP_OPP;
}


Sector UsefulFunctions::getPuckSector(Information& information) {
    Puck puck = information.world.getPuck();
    Sector puckSector = getPrognosedSectorUnit(information, information.self, puck, true);
    return puckSector;
}


Sector UsefulFunctions::getPrognosedSectorUnit(Information& information,
                                               const Hockeyist& hockeyist,
                                               const Unit& unit,
                                               const bool isUnitPuck) {
    
    const Game& game = information.game;


    double unitSpeedX = unit.getSpeedX();
    double unitSpeedY = unit.getSpeedY();
    double newUnitX = unit.getX();
    double newUnitY = unit.getY();

    double newRange = hockeyist.getDistanceTo(unit);
    double myPrognoseWayLength = 0.0;
    double hockeyistSpeed = hypot(hockeyist.getSpeedX(), hockeyist.getSpeedY()) * cos (hockeyist.getAngleTo(unit));

    while (newRange > myPrognoseWayLength + 100.0) {
        newUnitX += unitSpeedX;
        newUnitY += unitSpeedY;
            
        if (isUnitPuck) {
            unitSpeedX *= 0.999;
            unitSpeedY *= 0.999;
        }
        else {    
            unitSpeedX *= 0.98;
            unitSpeedY *= 0.98;

            double angle = unit.getAngle();

            unitSpeedX += (0.116) * cos(angle);
            unitSpeedY += (0.116) * sin(angle);
        }

        if (newUnitX < game.getRinkLeft()) {
            unitSpeedX *= 0.25;
            unitSpeedX = -unitSpeedX;
            newUnitX += (game.getRinkLeft() - newUnitX);
        }
        else if (newUnitX > game.getRinkRight()) {
            unitSpeedX *= 0.25;
            unitSpeedX = -unitSpeedX;
            newUnitX -= (newUnitX - game.getRinkRight());
        }

        if (newUnitY < game.getRinkTop()) {
            unitSpeedY *= 0.25;
            unitSpeedY = -unitSpeedY;
            newUnitY += (game.getRinkTop() - newUnitY);
        }
        else if (newUnitY > game.getRinkBottom()) {
            unitSpeedY *= 0.25;
            unitSpeedY = -unitSpeedY;
            newUnitY -= (newUnitY - game.getRinkBottom());
        }

        newRange = hockeyist.getDistanceTo(newUnitX, newUnitY);

        myPrognoseWayLength += hockeyistSpeed;
        hockeyistSpeed += 0.116;
        hockeyistSpeed *= 0.98;

        if (hockeyistSpeed > 5.0) {
            hockeyistSpeed = 5.0;
        }
    }
    
    Sector unitPrognoseSector = { newUnitX, newUnitY, 50 };

    return unitPrognoseSector;    
}

Sector UsefulFunctions::getAttackSector(Information& information, AllSystem& system) {
    Sector attackSector = (getEffectiveAttackSide(information, system) == AttackSideType::TOP)
        ? getAttackSectorTop(information, system) : getAttackSectorBottom(information, system);
    return attackSector;
}

Sector UsefulFunctions::getSupportSector(Information& information, AllSystem& system) {
    Sector supportSector = (getEffectiveAttackSide(information, system) == AttackSideType::TOP)
        ? getSupportSectorBottom(information, system) : getSupportSectorTop(information, system);
    return supportSector;
}

Sector UsefulFunctions::getDefenceSector(Information& information, AllSystem& system) {
    DefenceSideType defenceSide = getEffectiveDefenceSide(information, system);
    Sector defenceSector;
    if (defenceSide == DefenceSideType::DEFENCE_CENTER) {
        defenceSector = getDefenceSectorCenter(information, system);
    }
    else if (defenceSide == DefenceSideType::DEFENCE_TOP) {
        defenceSector = getDefenceSectorTop(information, system);
    }
    else { // defenceSide == DefenceSideType::DEFENCE_BOTTOM
        defenceSector = getDefenceSectorBottom(information, system);
    }

    return defenceSector;
}

Sector UsefulFunctions::getOpponentAttackSector(Information& information, AllSystem& system) {
    Sector oppAttackSector = (getEffectiveOpponentAttackSide(information) == OpponentAttackSideType::TOP_OPP)
        ? getOpponentAttackSectorTop(information, system) : getOpponentAttackSectorBottom(information, system);
    return oppAttackSector;
}

Sector UsefulFunctions::getDefenceSectorCenter(Information& information, AllSystem& system) {
    const Game& game = information.game;
    
    double posX = 0.5 * (game.getRinkLeft() + game.getRinkRight());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? -1.0 : 1.0) * DELTA_SECTOR_DEFENCE_X;
    double posY = 0.5 * (game.getRinkTop() + game.getRinkBottom());

    Sector defenceSectorCenter = { posX, posY, SECTOR_RADIUS_DEFENCE };
    return defenceSectorCenter;
}

Sector UsefulFunctions::getDefenceSectorTop(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkLeft() + game.getRinkRight());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? -1.0 : 1.0)
        * (DELTA_SECTOR_DEFENCE_X + DELTA_SECTOR_DEFENCE_TOP_BOT);
    double posY = 0.5 * (game.getRinkTop() + game.getRinkBottom());
    posY -= DELTA_SECTOR_DEFENCE_Y;

    Sector defenceSectorTop = { posX, posY, SECTOR_RADIUS_DEFENCE };
    return defenceSectorTop;
}

Sector UsefulFunctions::getDefenceSectorBottom(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkLeft() + game.getRinkRight());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? -1.0 : 1.0)
        * (DELTA_SECTOR_DEFENCE_X + DELTA_SECTOR_DEFENCE_TOP_BOT);
    double posY = 0.5 * (game.getRinkTop() + game.getRinkBottom());
    posY -= DELTA_SECTOR_DEFENCE_Y;

    Sector defenceSectorBottom = { posX, posY, SECTOR_RADIUS_DEFENCE };
    return defenceSectorBottom;
}

Sector UsefulFunctions::getStrikeSector(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    
    const Player opponentPlayer = world.getOpponentPlayer();
    double strikeX = 0.5 * (opponentPlayer.getNetFront() + opponentPlayer.getNetBack());
    double strikeY = 0.5 * (opponentPlayer.getNetTop() + opponentPlayer.getNetBottom());
    strikeY += (self.getY() < strikeY ? 0.5 : -0.5) * game.getGoalNetHeight();
    strikeX += (system.getPlayerSide() == PlayerSideType::LEFT ? -1.0 : 1.0) * DELTA_SECTOR_STRIKE_NET_X;

    Sector strikeSector = { strikeX, strikeY, SECTOR_RADIUS_STRIKE };

    if (self.getDistanceTo(strikeSector.x, strikeSector.y) <= strikeSector.radius) {
        mayBeChangeStrikeSector(information, system, strikeSector);
    }

    return strikeSector;
}

Sector UsefulFunctions::getAttackSectorTop(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkRight() + game.getRinkLeft());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? 1.0 : -1.0) * DELTA_SECTOR_ATTACK_X;
    double posY = game.getRinkTop();
    posY += DELTA_SECTOR_ATTACK_Y;

    Sector attackSectorTop = { posX, posY, SECTOR_RADIUS_ATTACK };
    return attackSectorTop;
}

Sector UsefulFunctions::getAttackSectorBottom(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkRight() + game.getRinkLeft());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? 1.0 : -1.0) * DELTA_SECTOR_ATTACK_X;
    double posY = game.getRinkBottom();
    posY -= DELTA_SECTOR_ATTACK_Y;

    Sector attackSectorBottom = { posX, posY, SECTOR_RADIUS_ATTACK };
    return attackSectorBottom;
}

Sector UsefulFunctions::getSupportSectorTop(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkRight() + game.getRinkLeft());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? 1.0 : -1.0) * DELTA_SECTOR_SUPPORT_X;
    double posY = game.getRinkTop() + DELTA_SECTOR_SUPPORT_Y;
    
    Sector supportSectorTop = { posX, posY, SECTOR_RADIUS_SUPPORT };
    return supportSectorTop;
}

Sector UsefulFunctions::getSupportSectorBottom (Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkRight() + game.getRinkLeft());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? 1.0 : -1.0) * DELTA_SECTOR_SUPPORT_X;
    double posY = game.getRinkBottom() - DELTA_SECTOR_SUPPORT_Y;
    
    Sector supportSectorBottom = { posX, posY, SECTOR_RADIUS_SUPPORT };
    return supportSectorBottom;
}


Sector UsefulFunctions::getOpponentAttackSectorTop(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkRight() + game.getRinkLeft());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? -1.0 : 1.0) * DELTA_SECTOR_OPP_ATTACK_X;
    double posY = game.getRinkTop();
    posY += DELTA_SECTOR_OPP_ATTACK_Y;

    Sector oppAttackSectorTop = { posX, posY, SECTOR_RADIUS_OPPONENT_ATTACK };
    return oppAttackSectorTop;
}

Sector UsefulFunctions::getOpponentAttackSectorBottom(Information& information, AllSystem& system) {
    const Game& game = information.game;

    double posX = 0.5 * (game.getRinkRight() + game.getRinkLeft());
    posX += (system.getPlayerSide() == PlayerSideType::LEFT ? -1.0 : 1.0) * DELTA_SECTOR_OPP_ATTACK_X;
    double posY = game.getRinkBottom();
    posY -= DELTA_SECTOR_OPP_ATTACK_Y;

    Sector oppAttackSectorBottom = { posX, posY, SECTOR_RADIUS_OPPONENT_ATTACK };
    return oppAttackSectorBottom;
}


bool UsefulFunctions::isInSector(Information& information, Sector sector) {
    const Hockeyist& self = information.self;
    if (self.getDistanceTo(sector.x, sector.y) <= sector.radius) {
        return true;
    }
    return false;
}

void UsefulFunctions::checkAttack(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    Move& move = information.move;
    int selfId = information.self.getTeammateIndex();
    
    bool wasInAttackSector = system.getHockeyistsInAttack(selfId)[0];
    bool wasInStrikeSector = system.getHockeyistsInAttack(selfId)[1];
    bool wasInStrike = system.isHockeyistsInStrike(selfId);

    if (self.getState() == HockeyistState::SWINGING) {
        if (isInStrikeZone(information, system, self) == false) {
            move.setAction(ActionType::CANCEL_STRIKE);
            system.resetHockeyistState(information, system, selfId);            
            return;
        }
    }

    const int attackZone = 0;
    const int strikeZone = 1;


    if (wasInAttackSector) {
        if (wasInStrikeSector) {
            if (wasInStrike) {
                Sector strikeSector = getStrikeSector(information, system);
                strikeSector.radius += 50;
                if (isInSector(information, strikeSector) == false) {
                    system.setHockeyistsInAttack(selfId, attackZone, false);
                    system.setHockeyistsInAttack(selfId, strikeZone, false);
                    system.setHockeyistsInStrike(selfId, false);
                    move.setAction(ActionType::CANCEL_STRIKE);
                }
            }
            else {
                Sector strikeSector = getStrikeSector(information, system);
                double angleToOppNet = self.getAngleTo(strikeSector.x, strikeSector.y);
                if (abs(angleToOppNet) <= information.game.getStrikeAngleDeviation()) {
                    system.setHockeyistsInStrike(selfId, true);
                }
            }
        }
        else {
            if (isInSector(information, system.getMovingSector(selfId))) {
                system.setMovingSectorChoosed(selfId, false);
                system.setHockeyistsInAttack(selfId, strikeZone, true);
            }
        }
    }
    else {
        if (isInSector(information, system.getMovingSector(selfId))) {
            system.setMovingSectorChoosed(selfId, false);
            system.setHockeyistsInAttack(selfId, attackZone, true);
        }
    }

    if (isInStrikeZone(information, system, self)) {
        double speedAngle = atan2(self.getSpeedY(), self.getSpeedX());
        Sector strikeSector = getStrikeSector(information, system);
        double angleToNet = atan2(strikeSector.y - self.getY(), strikeSector.x - self.getX());

        if (abs(angleToNet - speedAngle) < PI / 3) {
            if (system.getPlayerSide() == PlayerSideType::LEFT && self.getX() > (X_CENTER + X_LINE)) {
               system.setHockeyistsInAttack(selfId, attackZone, true);    
            }
            else if (system.getPlayerSide() == PlayerSideType::RIGHT && self.getX() < (X_CENTER - X_LINE)) {
               system.setHockeyistsInAttack(selfId, attackZone, true);    
            }
        }
    }
}

void UsefulFunctions::changeByRolesWith(Information& information, AllSystem& system, HockeyistRole changeRole) {
    Hockeyist* changeRoleHockeyist = getMyHockeyistByRole(information, system, changeRole);
    int changeRoleHockeyistId = changeRoleHockeyist->getTeammateIndex();
    int selfId = information.self.getTeammateIndex();

    HockeyistRole selfRole = system.getHockeyistsRole(selfId);
    system.setHockeyistsRole(selfId, changeRole);
    system.setHockeyistsRole(changeRoleHockeyistId, selfRole);

    system.setMovingSectorChoosed(selfId, false);
    system.setMovingSectorChoosed(changeRoleHockeyistId, false);
}

bool UsefulFunctions::isOpponentDangerousPass(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    const World& world = information.world;

    vector<Hockeyist> hockeyists = world.getHockeyists();

    for (size_t i = 0; i < hockeyists.size(); ++i) {
        if (hockeyists[i].isTeammate() == false && hockeyists[i].getType() != HockeyistType::GOALIE
            && hockeyists[i].getState() != HockeyistState::RESTING) {

            double angle = self.getAngleTo(hockeyists[i]);
            double range = self.getDistanceTo(hockeyists[i]);
            
            if (abs(angle) < PI / 2 && range < 100.0) {
                return true;
            }
            int selfId = self.getTeammateIndex();
            if (range < 150.0) {
                if (isInStrikeZone(information, system, self) == false) {
                    return true;
                }
                Sector movingSector = system.getMovingSector(selfId);
                movingSector.radius *= 2 / 3;
                if (movingSector.radius >= 200) {
                    movingSector.radius = 650;
                }
                if (isInSector(information, movingSector) == false) {
                    return true;
                }
            }
            if (range < 200.0) {
                if (system.getPlayerSide() == PlayerSideType::LEFT) {
                    if (self.getSpeedX() < -1.0) {
                        return true;
                    }
                }
                else {
                    if (self.getSpeedX() > 1.0) {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

bool UsefulFunctions::isOpponentInAttackDangerousAttacker(Information& information, AllSystem& system) {

    const Hockeyist& self = information.self;
    const World& world = information.world;
    const Game& game = information.game;

    Puck puck = world.getPuck();
    Hockeyist* puckOwner = getHockeyistById(puck.getOwnerHockeyistId(), world);
    
    if (puckOwner != NULL) {
        if (puckOwner->isTeammate() == false) {
            double puckOwnerX = puckOwner->getX();
            double puckOwnerY = puckOwner->getY();
            double puckOwnerNextX = puckOwnerX + puckOwner->getSpeedX();
            double puckOwnerNextY = puckOwnerY + puckOwner->getSpeedY();

            double range = self.getDistanceTo(*puckOwner);
            double nextRange = self.getDistanceTo(puckOwnerNextX, puckOwnerNextY);

            if (range < nextRange) {

                if (system.getPlayerSide() == PlayerSideType::LEFT) {
                    if (puckOwner->getSpeedX() < 3.0) {
                        return true;
                    }
                }
                if (system.getPlayerSide() == PlayerSideType::RIGHT) {
                    if (puckOwner->getSpeedX() > -3.0) {
                        return true;
                    }
                }

            }
        }
    }

    return false;
}

bool UsefulFunctions::isOpponentInAttackDangerousDefencer(Information& information, AllSystem& system) {
    const Hockeyist& self = information.self;
    const World& world = information.world;

    Sector defSector = getDefenceSector(information, system);
    Hockeyist* puckOwner = getHockeyistById(world.getPuck().getOwnerHockeyistId(), world);
    
    double range = puckOwner->getDistanceTo(defSector.x, defSector.y);

    if (range <= RANGE_OPP_DANGEROUS_DEF) {
        return true;
    }

    return false;
}


bool UsefulFunctions::isPuckDangerous(Information& information) {
    const Hockeyist& self = information.self;
    Puck puck = information.world.getPuck();

    // double speedPuck = hypot(self.getSpeedX(), self.getSpeedY());
    // баг века!
    double speedPuck = hypot(puck.getSpeedX(), puck.getSpeedY());

    if (speedPuck >= 18.0) {
        return true;
    }

    return false;
}


bool UsefulFunctions::isInStrikeZone(Information& information, AllSystem& system, const Hockeyist& hockeyist) {
    const World& world = information.world;
    const Game& game = information.game;

    const PlayerSideType playerSide = system.getPlayerSide();
    Player oppPlayer = world.getOpponentPlayer();
    
    double x = hockeyist.getX();
    double y = hockeyist.getY();

    if (abs(y - Y_CENTER) < MIN_DELTA_Y_STRIKE) {
        return false;
    }

    if (playerSide == PlayerSideType::LEFT) {

//        if (x )

        if (y < Y_CENTER) {
            double top1X = X_CENTER + DELTA_BEGIN_STRIKE_LINE1_X;
            double top1Y = game.getRinkTop() + DELTA_BEGIN_STRIKE_LINE1_Y;
            
            double top2X = X_CENTER + DELTA_BEGIN_STRIKE_LINE2_X;
            double top2Y = game.getRinkTop() + DELTA_BEGIN_STRIKE_LINE2_Y;

            double netX = 0.5 * (oppPlayer.getNetFront() + oppPlayer.getNetBack()) + DELTA_END_STRIKE_X;
            double netY = oppPlayer.getNetBottom() + DELTA_END_STRIKE_Y;

            double koefLine1 = (y - top1Y) * (netX - top1X) - (x - top1X) * (netY - top1Y);
            double koefLine2 = (y - top2Y) * (netX - top2X) - (x - top2X) * (netY - top2Y);

            if (koefLine1 < 0.0 && koefLine2 > 0.0) {
                return true;
            }
        }
        else { // self.getY() >= Y_CENTER
            double bot1X = X_CENTER + DELTA_BEGIN_STRIKE_LINE1_X;
            double bot1Y = game.getRinkBottom() - DELTA_BEGIN_STRIKE_LINE1_Y;

            double bot2X = X_CENTER + DELTA_BEGIN_STRIKE_LINE2_X;
            double bot2Y = game.getRinkBottom() - DELTA_BEGIN_STRIKE_LINE2_Y;

            double netX = 0.5 * (oppPlayer.getNetFront() + oppPlayer.getNetBack()) + DELTA_END_STRIKE_X;
            double netY = oppPlayer.getNetTop() - DELTA_END_STRIKE_Y;
            
            double koefLine1 = (y - bot1Y) * (netX - bot1X) - (x - bot1X) * (netY - bot1Y);
            double koefLine2 = (y - bot2Y) * (netX - bot2X) - (x - bot2X) * (netY - bot2Y);

            if (koefLine1 > 0.0 && koefLine2 < 0.0) {
                return true;
            }
        }
    }

    else if (playerSide == PlayerSideType::RIGHT) {
        if (y < Y_CENTER) {
            double top1X = X_CENTER - DELTA_BEGIN_STRIKE_LINE1_X;
            double top1Y = game.getRinkTop() + DELTA_BEGIN_STRIKE_LINE1_Y;
            
            double top2X = X_CENTER - DELTA_BEGIN_STRIKE_LINE2_X;
            double top2Y = game.getRinkTop() + DELTA_BEGIN_STRIKE_LINE2_Y;

            double netX = 0.5 * (oppPlayer.getNetFront() + oppPlayer.getNetBack()) - DELTA_END_STRIKE_X;
            double netY = oppPlayer.getNetBottom() + DELTA_END_STRIKE_Y;
            
            double koefLine1 = (y - top1Y) * (netX - top1X) - (x - top1X) * (netY - top1Y);
            double koefLine2 = (y - top2Y) * (netX - top2X) - (x - top2X) * (netY - top2Y);

            if (koefLine1 > 0.0 && koefLine2 < 0.0) {
                return true;
            }
        }
        else { // self.getY() >= Y_CENTER            
            double bot1X = X_CENTER - DELTA_BEGIN_STRIKE_LINE1_X;
            double bot1Y = game.getRinkBottom() - DELTA_BEGIN_STRIKE_LINE1_Y;
            
            double bot2X = X_CENTER - DELTA_BEGIN_STRIKE_LINE2_X;
            double bot2Y = game.getRinkBottom() - DELTA_BEGIN_STRIKE_LINE2_Y;

            double netX = 0.5 * (oppPlayer.getNetFront() + oppPlayer.getNetBack()) - DELTA_END_STRIKE_X;
            double netY = oppPlayer.getNetTop() - DELTA_END_STRIKE_Y;
            
            double koefLine1 = (y - bot1Y) * (netX - bot1X) - (x - bot1X) * (netY - bot1Y);
            double koefLine2 = (y - bot2Y) * (netX - bot2X) - (x - bot2X) * (netY - bot2Y);

            if (koefLine1 < 0.0 && koefLine2 > 0.0) {
                return true;
            }
        }
    }
    
    return false;
}


bool UsefulFunctions::isChangeByRolesNeed(Information& information, AllSystem& system) {
    
    const Hockeyist& self = information.self; 
    const World& world = information.world; 
    const Game& game = information.game;
    Move& move = information.move;

    Puck puck = world.getPuck();

    if (puck.getOwnerPlayerId() != world.getMyPlayer().getId()) {
        Hockeyist* defencer = getMyHockeyistByRole(information, system, DEFENCER);
        Hockeyist* attacker = getMyHockeyistByRole(information, system, ATTACKER);

        Sector defNearestSector = getPrognosedSectorUnit(information, *defencer, puck, true);
        Sector attNearestSector = getPrognosedSectorUnit(information, *attacker, puck, true);

        double rangeFromDef = defencer->getDistanceTo(defNearestSector.x, defNearestSector.y);
        double rangeFromAtt = attacker->getDistanceTo(attNearestSector.x, attNearestSector.y);

        double rangeBetweenThem = defencer->getDistanceTo(*attacker);
        
        if (rangeFromDef < rangeFromAtt && rangeBetweenThem >= 120.0) {
            return true;
        }

    }
    return false;
}

void UsefulFunctions::mayBeChangeStrikeSector(Information& information, AllSystem& system, Sector& strikeSector) {
    const Hockeyist& self = information.self;
    
    double angleToSector = self.getAngleTo(strikeSector.x, strikeSector.y);
    double absoluteSpeedSelf = hypot(self.getSpeedX(), self.getSpeedY());

    double realSpeedX = self.getSpeedX();
    
    double neededSpeedX;
    if (system.getPlayerSide() == PlayerSideType::LEFT) {
        neededSpeedX = absoluteSpeedSelf * cos(angleToSector);
    }
    else {
        neededSpeedX = absoluteSpeedSelf * (-cos(angleToSector));
    }

    double deltaXOneTick = neededSpeedX - realSpeedX;

    strikeSector.x += 0.8 * deltaXOneTick * getEffectiveSwingingTicks(information, system);
    //double realSpeedY = self.getSpeedY();
    //double neededSpeedY = absoluteSpeedSelf * cos(angleToSector);
    //double deltaYOneTick = neededSpeedY - realSpeedY;

    //strikeSector.y += deltaYOneTick * getEffectiveSwingingTicks(information, system);    
}
