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

void MySystem::Tactics::executeTacticsContrattack(Information& information, AllSystem& system) {

    information.move.setSpeedUp(-1.0);
    information.move.setTurn(PI);
    information.move.setAction(STRIKE);

}
