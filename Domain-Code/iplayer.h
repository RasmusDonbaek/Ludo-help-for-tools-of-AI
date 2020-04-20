#ifndef IPLAYER
#define IPLAYER

#include <iostream>
#include "positions_and_dice.h"

// Let your own AI player implementations inheriet from this player interface.
class iplayer
{
protected:
    int dice;
    int* position;

public:
    int make_decision(positions_and_dice& relative)
    {
        dice = relative.dice;
        position = relative.position;

        return make_decision();
    }

protected:
    virtual int make_decision() = 0; //This is the function you need to implement in you own player classes

    //A helping function, which might be usefull
    bool is_valid_move(int piece)
    {
        if(piece < 0 || piece > 3)
            return false;

        if(position[piece] >= 0 && position[piece] < 99)
            return true;

        if (position[piece] < 0 && dice == 6)
            return true;

        return false;
    }
};

#endif // IPLAYER

