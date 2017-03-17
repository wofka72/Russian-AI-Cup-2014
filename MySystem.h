#ifndef MY_SYSTEM_H
#define MY_SYSTEM_H

#include "model\Hockeyist.h"

using namespace model;

struct Information;
class AllSystem;

namespace MySystem {
    enum TacticsType {
        DEFAULT, EXTENDED, PRESSING, MISSED_GOAL, SCORED_GOAL,
        CATCH_PUCK, ATTACK, PASSING, DEFENCE, CONTRATTACK, HOLD_PUCK
    };
    enum HockeyistRole {
        NONE_ROLE, SUPPORT, DEFENCER, ATTACKER
    };
    enum TacticsHockeyistType {
        NONE_TACTICS,

        PASS_TO_SUPPORT,
        PASS_TO_ATTACKER,
        PASS_TO_DEFENCER,

        MOVE_TO_SUPPORT,
        MOVE_TO_ATTACK,
        MOVE_TO_STRIKE,
        MOVE_TO_NET,

        GO_PRESSING,
        TURN_TO_PUCK

        // ...
    };
    enum PlayerSideType {
        LEFT, RIGHT
    };

    struct Sector {
        double x;
        double y;
        double radius;
    };

    enum AttackSideType {
        TOP = 1,
        BOTTOM = 2
    };

    enum OpponentAttackSideType {
        TOP_OPP = 1,
        BOTTOM_OPP = 2
    };

    enum DefenceSideType {
        DEFENCE_CENTER = 1,
        DEFENCE_TOP = 2,
        DEFENCE_BOTTOM = 3
    };

    // Class with functions which define global types of hockeyist's behavior
    class Tactics {
    private:
        void executeTacticsDefault(Information& information, AllSystem& system);
        void executeTacticsExtended(Information& information, AllSystem& system);
        void executeTacticsPressing(Information& information, AllSystem& system);
        void executeTacticsMissedGoal(Information& information, AllSystem& system);
        void executeTacticsScoredGoal(Information& information, AllSystem& system);

        void executeTacticsCatchPuck(Information& information, AllSystem& system);
        void executeTacticsAttack(Information& information, AllSystem& system);
        void executeTacticsPassing(Information& information, AllSystem& system);
        void executeTacticsDefence(Information& information, AllSystem& system);
        void executeTacticsContrattack(Information& information, AllSystem& system);
        void executeTacticsHoldPuck(Information& information, AllSystem& system);

    public:
        void computeOptimalTactics(Information& information, AllSystem& system);
        void executeTactics(Information& information, AllSystem& system);

    };

    // Class with functions where are described hockeyists actions
    class TacticsHockeyists {
    public:

        void moveApproxToSector(Information& information, AllSystem& system, const Sector& sector);
        void moveQuickToSector(Information& information, AllSystem& system, const Sector& sector);
        void moveToEnemy(Information& information, AllSystem& system, Hockeyist* hockeyist);
        void turnToSector(Information& information, const Sector& sector);
        
        void mayBeContinueAttack(Information& information, AllSystem& system);

        void moveToAttack(Information& information, AllSystem& system);
        void moveToAttackSector(Information& information, AllSystem& system);
        void moveToStrikeSector(Information& information, AllSystem& system);
        void strikePuckAttacker(Information& information, AllSystem& system);
        void strikePuckDefencer(Information& information, AllSystem& system);

        void strikePuckAndOwner(Information& information, AllSystem& system);

        void moveToPuck(Information& information, AllSystem& system);
        void moveToPuckDefencer(Information& information, AllSystem& system);
        
        void turnToPuck(Information& information, AllSystem& system);
        void passToOther(Information& information, AllSystem& system);
        void moveToMyNet(Information& information, AllSystem& system);
        void defenceMyNetDefencer(Information& information, AllSystem& system);
        void moveToOppAttackSector(Information& information, AllSystem& system);
        void moveApproxToSectorDefence(Information& information, AllSystem& system, Sector defenceSector);

        void pushEnemy(Information& information, AllSystem& system, Hockeyist* enemy);
        void strikeUnit(Information& information, const Unit& unit);

        void catchPuck(Information& information, AllSystem& system);
        void moveToSupportSector(Information& information, AllSystem& system);
        void moveToMovingSector(Information& information, AllSystem& system);

    };

}

#endif
