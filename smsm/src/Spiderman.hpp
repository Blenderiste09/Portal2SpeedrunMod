#pragma once

#include "Command.hpp"
#include "Variable.hpp"

class Spiderman {
private:
    int lastTickBase = 0;
    Vector oldPos;
    Vector tetherPoint;
    float tetherLength;
    bool isDucking;

public:
    bool wantsToSwing;

public:
    Spiderman();
    void ThrowWeb();
    void ProcessMovement(void* pPlayer, CMoveData* pMove);
    void Swing(void* pPlayer, CMoveData* pMove, float ft);
};

extern Command test;

extern Spiderman spiderman;
