#ifndef USEFUL_FUNCTIONS_H
#define USEFUL_FUNCTIONS_H

#include "MyStrategy.h"
#include "Information.h"
#include "AllSystem.h"

using namespace std;
using namespace model;

namespace UsefulFunctions {

    Hockeyist* getNearestOpponent(const double x, const double y, const World& world);
    Hockeyist* getNearestTeammate(const double x, const double y, const World& world);
    Hockeyist* getMyOtherHockeyist(const Hockeyist& thisHockeyist, const World& world);
    Hockeyist* getHockeyistById(const int hockeyistId, const World& world);
    Hockeyist* getMyHockeyistByRole(const Information& information, const AllSystem& system, const HockeyistRole role);

    Sector getPrognosedSectorUnit(Information& information, const Hockeyist& hockeyist,
                                  const Unit& unit, const bool isPuck);

    Sector getPuckSector(Information& information);
    Sector getAttackSector(Information& information, AllSystem& system);
    Sector getStrikeSector(Information& information, AllSystem& system);
    Sector getSupportSector(Information& information, AllSystem& system);
    Sector getDefenceSector(Information& information, AllSystem& system);
    Sector getOpponentAttackSector(Information& information, AllSystem& system);
    
    Sector getAttackSectorBottom(Information& information, AllSystem& system);
    Sector getAttackSectorTop(Information& information, AllSystem& system);
    
    Sector getSupportSectorBottom(Information& information, AllSystem& system);
    Sector getSupportSectorTop(Information& information, AllSystem& system);
    
    Sector getDefenceSectorCenter(Information& information, AllSystem& system);
    Sector getDefenceSectorBottom(Information& information, AllSystem& system);
    Sector getDefenceSectorTop(Information& information, AllSystem& system);

    Sector getOpponentAttackSectorTop(Information& information, AllSystem& system);
    Sector getOpponentAttackSectorBottom(Information& information, AllSystem& system);


    double getEffectiveSpeedByAngle(const double angle);
    double getEffectiveSpeedByRange(const double range);

    double getEffectiveAngleToPuck(Information& information);
    double getEffectivePassAngle(Information& information, const Hockeyist& hockeyist);
    double getEffectivePassPower(Information& information, const Hockeyist& hockeyist);


    int getEffectiveSwingingTicks(Information& information, AllSystem& system);
    AttackSideType getEffectiveAttackSide(Information& information, AllSystem& system);
    DefenceSideType getEffectiveDefenceSide(Information& information, AllSystem& system);
    OpponentAttackSideType getEffectiveOpponentAttackSide(Information& information);


    bool isInSector(Information& information, Sector sector);
    void checkAttack(Information& information, AllSystem& system);
    void checkPassingNeed(Information& infromation, AllSystem& system);

    void changeByRolesWith(Information& information, AllSystem& system, HockeyistRole changeRole);

    
    bool isOpponentDangerousPass(Information& information, AllSystem& system);
    bool isOpponentInAttackDangerousAttacker(Information& information, AllSystem& system);
    bool isOpponentInAttackDangerousDefencer(Information& information, AllSystem& system);
    bool isPuckDangerous(Information& information);
    bool isInStrikeZone(Information& information, AllSystem& system, const Hockeyist& hockeyist);

    bool isChangeByRolesNeed(Information& information, AllSystem& system);
    void mayBeChangeStrikeSector(Information& information, AllSystem& system, Sector& strikeSector);


    // Very very bad functions
    // TODO: DELETE THEM ALL
    bool isInStrikePosition(Information& information, AllSystem& system, const Hockeyist& hoskeyist);
    double getEffectiveAngleTo(const Hockeyist& self, const Unit& unit);
    double getEffectiveSpeedTo(const double angle);
    void setStrikePosition(Information& information, AllSystem& system, double& coordX, double& coordY);
    int getDanger(Information& information, const Unit& unit);
    // NO!
    // A3A3A :P
};

#endif
