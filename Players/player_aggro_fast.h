#ifndef PLAYER_AGGRO_FAST_H
#define PLAYER_AGGRO_FAST_H

#include "iplayer.h"
#include "move_logic.h"

//***************************************************************************************
//*     Strategy of 'player_aggro_fast'                                                 *
//*     If possible send an opponent home, if multiple options, goes for maximum damage *
//*     Else brings pieces into goal and into play when possible,                       *
//*     but else it will push the front most piece around the outfield,                 *
//*     while making sure it only sends its own pieces home when forced to do so.       *
//*     In 2-vs-2 play agains player_random      it wins approx. 92% of the time        *
//*     In 2-vs-2 play agains player_random_safe it wins approx. 89% of the time        *
//*     In 2-vs-2 play agains player_fast        it wins approx. 70% of the time        *
//***************************************************************************************

class player_aggro_fast : public iplayer
{
private:
    move_logic logic;

public:
    player_aggro_fast() { /* do nothing */ }

private:
    int make_decision()
    {
        int piece = -1;
        logic.set(dice, position);

        if(logic.count_valid_moves() < 2)    // If no choice
            return logic.first_legal_move(); // Don't waste time analysing

        //This makes us aggressive
        piece = logic.move_send_home();    if(piece != -1) return piece; // Send an opponent home, go for maximum damage if multiple options

        //Else do almost like 'player_fast'
        piece = logic.move_into_goal();    if(piece != -1) return piece; // Bring a piece into goal
        piece = logic.move_start();        if(piece != -1) return piece; // Bring a piece into play
        piece = logic.move_fast_safe();    if(piece != -1) return piece; // Move the most advanced outfield piece posible without sending itself home
        piece = logic.move_random_safe();  if(piece != -1) return piece; // Make any move which don't send ourself home.
        piece = logic.move_early_outfield();if(piece != -1) return piece;// Forced to send ourself home, we choose the least advanced one.

        return -1; //Should never reach this line. Provided for a sense of completeness
    }
};

#endif // PLAYER_AGGRO_FAST_H
