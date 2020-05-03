#ifndef PLAYER_RANDOM_SAFE_H
#define PLAYER_RANDOM_SAFE_H

#include "iplayer.h"
#include "move_logic.h"

//***************************************************************************************
//*     Strategy of 'player_random_safe'                                                *
//*     If possible : Randomly selecet a legal move which does not send yourself home   *
//*     Else        : Randomly selecet a legal move                                     *
//*     In 2-vs-2 play agains player_random it wins approx. 57% of the time             *
//***************************************************************************************

class player_random_safe : public iplayer
{
private:
    move_logic logic;

public:
    player_random_safe() { /* do nothing */ }

private:
    int make_decision()
    {
        int piece = -1;
        logic.set(dice, position);

        piece = logic.move_random_safe();  if(piece != -1) return piece;
        piece = logic.move_random_legal(); if(piece != -1) return piece;

        return -1; //No moves available
    }
};

#endif // PLAYER_RANDOM_SAFE_H
