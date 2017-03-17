#ifndef ALL_SYSTEM_H
#define ALL_SYSTEM_H

#include "MyStrategy.h"
#include "MySystem.h"
#include "Information.h"

using namespace std;
using namespace model;
using namespace MySystem;


class AllSystem {
private:

    TacticsType currentTactics;
    TacticsType previousTactics;
    HockeyistRole hockeyistsRoles[3];
    TacticsHockeyistType currentTacticsHockeyists[3];
    PlayerSideType playerSide;
    Sector movingSectors[3];

    Tactics tactics;
    TacticsHockeyists tacticsHockeyists;

    bool tacticsChoosed;
    bool rolesAreBad;

    bool movingSectorChoosed[3];
    bool hockeyistsInAttack[3][2];
    bool hockeyistsInStrike[3];

    bool justPassed;
    bool passingNeed;

    void setPlayerSide(const World& world) {
        if (world.getMyPlayer().getNetBack() < world.getMyPlayer().getNetFront()) {
            playerSide = LEFT;
        }
        else {
            playerSide = RIGHT;
        }
    }

public:

    AllSystem(const World& _world) {
        tacticsChoosed = false;
        passingNeed = false;
        
        previousTactics = DEFAULT;
        currentTactics = DEFAULT;
        setPlayerSide(_world);
        justPassed = false;
        rolesAreBad = true;

        for (int i = 0; i < 3; ++i) {
            hockeyistsRoles[i] = NONE_ROLE;
            currentTacticsHockeyists[i] = NONE_TACTICS;
            hockeyistsInAttack[i][0] = false;
            hockeyistsInAttack[i][1] = false;
            hockeyistsInStrike[i] = false;
            movingSectorChoosed[i] = false;
        }
    }


    TacticsType getCurrentTactics() { return currentTactics; }
    TacticsType getPreviousTactics() { return previousTactics; }
    HockeyistRole getHockeyistsRole(int id) { return hockeyistsRoles[id]; }
    HockeyistRole* getHockeyistsRole() { return hockeyistsRoles; }
    TacticsHockeyistType getCurrentTacticsHockeyists(int id) { return currentTacticsHockeyists[id]; }
    TacticsHockeyistType* getCurrentTacticsHockeyists() { return currentTacticsHockeyists; }
    PlayerSideType getPlayerSide() { return playerSide; }
    
    void setCurrentTactics(TacticsType tact) { previousTactics = currentTactics;
                                               currentTactics = tact; }
    void setHockeyistsRole(int id, HockeyistRole role) { hockeyistsRoles[id] = role; }
    void setCurrentTacticsHockeyists(int id, TacticsHockeyistType tactHock) { currentTacticsHockeyists[id] = tactHock; }
    void setPlayerSide(PlayerSideType side) { playerSide = side; }

    Tactics getTactics() { return tactics; };
    TacticsHockeyists getTacticsHockeyists() { return tacticsHockeyists; }
   
    void setTacticsChoosed(bool choise) { tacticsChoosed = choise; }
    bool isTacticsChoosed() { return tacticsChoosed; }
    void setRolesAreBad(bool value) { rolesAreBad = value; }
    bool areRolesBad() { return rolesAreBad; }

    bool* getHockeyistsInAttack(int id) { return ((bool*) hockeyistsInAttack[id]); }
    bool** getHockeyistsInAttack() { return ((bool**) hockeyistsInAttack); }
    void setHockeyistsInAttack(int id, int zone, bool value) { hockeyistsInAttack[id][zone] = value; }
    bool isHockeyistsInStrike(int id) { return hockeyistsInStrike[id]; }
    void setHockeyistsInStrike(int id, bool value) { hockeyistsInStrike[id] = value; }
    bool isMovingSectorChoosed(int id) { return movingSectorChoosed[id]; }
    void setMovingSectorChoosed(int id, bool value) { movingSectorChoosed[id] = value; }

    void setPassingNeed(bool value) { passingNeed = value; }
    bool isPassingNeed() { return passingNeed; }
    void setJustPassed(bool value) { justPassed = value; }
    bool isJustPassed() { return justPassed; }

    void setMovingSector(int index, Sector sector) { movingSectors[index] = sector; }
    Sector getMovingSector(int index) { return movingSectors[index]; }




    void resetHockeyistState(Information& information, AllSystem& system, int teamId) {
        setHockeyistsInAttack(teamId, 0, false);
        setHockeyistsInAttack(teamId, 1, false);
        setHockeyistsInStrike(teamId, false);
        setMovingSectorChoosed(teamId, false);
        Sector invalidateSector = { -100, -100, 0 };
        setMovingSector(teamId, invalidateSector);
    }

    void resetMyHockeyistsState(Information& information, AllSystem& system) {
        for (int id = 0; id < 3; ++id) {
            resetHockeyistState(information, system, id);
        }
        
        setJustPassed(false);
        setPassingNeed(false);
    }

};

#endif
