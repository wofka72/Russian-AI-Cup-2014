#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PI 3.14159265358979323846
#define GRADUS (PI / 180)

// Moving constants
const double MIN_ANGLE_ANGLE_TO_SPEED = (PI / 6);
const double MIN_RANGE_RANGE_TO_SPEED = -50.0;
const double MAX_RANGE_RANGE_TO_SPEED = 100.0;

const double MIN_RANGE_MOVE_NET_FRONT = 100.0;
const double SOME_RANGE_MOVE_NET_FRONT = 150.0;

// Sector constants
const double SECTOR_RADIUS_SUPPORT = 50.0;
const double SECTOR_RADIUS_ATTACK = 150.0;
const double SECTOR_RADIUS_STRIKE = 550.0;
const double SECTOR_RADIUS_DEFENCE = 70.0;
const double SECTOR_RADIUS_OPPONENT_ATTACK = 200.0;

const double DELTA_SECTOR_ATTACK_X = -20.0;
const double DELTA_SECTOR_ATTACK_Y = 69.0;
const double DELTA_SECTOR_STRIKE_NET_X = 35.0;

const double DELTA_SECTOR_SUPPORT_X = 125.0;
const double DELTA_SECTOR_SUPPORT_Y = 60.0;

const double DELTA_SECTOR_DEFENCE_X = 400.0;
const double DELTA_SECTOR_DEFENCE_Y = 80.0;
const double DELTA_SECTOR_DEFENCE_TOP_BOT = 50.0;

const double DELTA_SECTOR_OPP_ATTACK_X = 360.0;
const double DELTA_SECTOR_OPP_ATTACK_Y = 300.0;

//const double CHANGE_SECTOR_STRIKE_X = 40.0;



const double RANGE_MAX_PASS_POWER = 700.0;
const double RANGE_MIN_PASS_POWER = 100.0;
const double MAX_PASS_POWER = 0.8;
const double MIN_PASS_POWER = 0.25;


// Initial state
const double Y_CENTER               = 460;
const double X_CENTER               = 600;

const double FROM_CENTER_TO_FIRST        = 150;
const double FROM_CENTER_TO_SECOND       = 300;
const double FROM_CENTER_TO_GOALIE       = 505;


// Attack zone
const double MIN_DELTA_Y_STRIKE         = 90.0;
const double DELTA_BEGIN_STRIKE_LINE1_X = 0.0;
const double DELTA_BEGIN_STRIKE_LINE1_Y = 250.0;

const double DELTA_BEGIN_STRIKE_LINE2_X = 250.0;
const double DELTA_BEGIN_STRIKE_LINE2_Y = 0.0;

const double DELTA_END_STRIKE_X         = 0.0;
const double DELTA_END_STRIKE_Y         = 0.0;

// other

const double X_LINE                     = 20.0;
const int TICKS_PROGNOSE                = 50;
const double RANGE_OPP_DANGEROUS_DEF    = 10000.0;

#endif
