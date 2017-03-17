#ifndef INFORMATION_H
#define INFORMATION_H

#include "MyStrategy.h"

using namespace std;
using namespace model;

struct Information {

    const Hockeyist& self;
    const World& world;
    const Game& game;
    Move& move;

    Information(const Hockeyist& _self, const World& _world, const Game& _game, Move& _move):
        self(_self), world(_world), game(_game), move(_move) {}
    
};

#endif
