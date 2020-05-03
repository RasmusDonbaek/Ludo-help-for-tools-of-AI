#ifndef PLAYER_FAST_H
#define PLAYER_FAST_H

#include "iplayer.h"
#include "move_logic.h"

//***************************************************************************************
//*     Strategy of 'player_fast'                                                       *
//*     Brings pieces into goal and into play when possible,                            *
//*     but else it will push the front most piece around the outfield,                 *
//*     even if it sends it own piece home                                              *
//*     In 2-vs-2 play agains player_random it wins approx. 85% of the time             *
//*     In 2-vs-2 play agains player_random_safe it wins approx. 77% of the time        *
//***************************************************************************************

class player_fast : public iplayer
{
private:
    move_logic logic;

public:
    player_fast() { /* do nothing */ }

private:
    int  make_decision()
    {
        int piece = -1;
        logic.set(dice, position);

        if(logic.count_valid_moves() < 2)    // If no choice
            return logic.first_legal_move(); // Don't waste time analysing

        piece = logic.move_into_goal();    if(piece != -1) return piece; // Bring a piece into goal
        piece = logic.move_start();        if(piece != -1) return piece; // Bring a piece into play
        piece = logic.move_fast();         if(piece != -1) return piece; // Move piece furthes on the outfiels, even if we send ourself home
        piece = logic.move_random_legal(); if(piece != -1) return piece; // Just make a legal move

        return -1; //Should never reach this line. Provided for a sense of completeness
    }
};

#endif // PLAYER_FAST_H
