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

void printPuckParameters(const Puck& puck);

void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {

    static AllSystem system(world);
    Information information(self, world, game, move);

    if (system.isTacticsChoosed() == false) {
        system.getTactics().computeOptimalTactics(information, system);
    }
    Tactics tact = system.getTactics();
    tact.executeTactics(information, system);

    system.setTacticsChoosed(false);
}

MyStrategy::MyStrategy() { }

#ifdef IOSTREAM

void printPuckParameters(const Puck& puck) {
    double puckSpeed = hypot(puck.getSpeedX(), puck.getSpeedY());

    double catchA = (160 - 60) / (0 - 20);
    double catchB = 160;
    double strikeA = (175 - 75) / (0 - 20);
    double strikeB = 175;

    double catchChance = catchA * puckSpeed + catchB;
    double strikeChance = strikeA * puckSpeed + strikeB;

    
    if (catchChance > 95.0) {
        catchChance = 95.0;
    }
    if (catchChance < 5.0) {
        catchChance = 5.0;
    }
    if (strikeChance > 95.0) {
        strikeChance = 95.0;
    }
    if (strikeChance < 5.0) {
        strikeChance = 5.0;
    }


    cout << fixed << setprecision(3)
    //    << "sX = " << setw(7) << puck.getSpeedX()
    //    << " sY = " << setw(7) << puck.getSpeedY()
        << " absS = " << setw(7) << puckSpeed
        << "  catchChance = " << setprecision(2) << setw(5) << catchChance << "% "
        << "strikeChance = " << setprecision(2) << setw(5) << strikeChance << "%" << endl;
}

#endif
