#include "move_logic.h"

move_logic::move_logic()
{
    std::random_device rd;
    generator = std::mt19937(rd());
}

void move_logic::set(int dice, int* position)
{
    this->dice = dice;
    this->position = position;
}

int move_logic::count_valid_moves() const
{
    int move_count = 0;

    for(int i = 0; i < 4; i++)
        if(is_valid_move(i))
            move_count++;

    return move_count;
}

int move_logic::first_legal_move() const
{
    for(int i = 0; i < 4; i++)
        if(is_valid_move(i))
            return i;

    return NO_MOVE;
}

int move_logic::move_random_legal() const
{
    //Identify all legal moves
    std::vector<int> options;

    for(int i = 0; i < 4; i++)
        if(is_valid_move(i))
            options.push_back(i);

    if(options.size() == 0) return NO_MOVE;    //no legal moves
    if(options.size() == 1) return options[0]; //only one legal move

    //randomly select amongst multiple legal moves
    distribution = std::uniform_int_distribution<int>(0, options.size() - 1);
    return options[distribution(generator)];
}

int move_logic::move_random_safe() const
{
    //Identify all safe legal moves
    std::vector<int> options;
    for(int i = 0; i < 4; i++)
        if(is_valid_move(i) && is_safe_move(i))
            options.push_back(i);

    if(options.size() == 0) return NO_MOVE;    //no safe moves
    if(options.size() == 1) return options[0]; //only one safe move

    //randomly select amongst multiple safe moves.
    distribution = std::uniform_int_distribution<int>(0, options.size() - 1);
    return options[distribution(generator)];
}

int move_logic::move_start() const
{
    if(dice != 6)
        return NO_MOVE;

    for(int i = 0; i < 4; i++)
        if(position[i] < 0)
            return i;

    return NO_MOVE;
}

int move_logic::move_into_goal() const
{
    for(int i = 0; i < 4; i++)
        if(position[i]+dice == 50 || position[i]+dice == 56)
            return i;

    return NO_MOVE;
}

int move_logic::move_fast() const
{
    int piece = NO_MOVE;
    int square = -1;
    for(int i = 0; i < 4; i++)
        if(position[i] > square  && position[i] < 51)
        {
            square = position[i];
            piece = i;
        }

    return piece;
}

int move_logic::move_fast_safe() const
{
    int piece = NO_MOVE;
    int square = -1;
    for(int i = 0; i < 4; i++)
        if(position[i] > square  && position[i] < 51 && is_safe_move(i))
        {
            square = position[i];
            piece = i;
        }

    return piece;
}

int move_logic::move_early_outfield() const
{
    int piece = NO_MOVE;
    int square = 51;
    for(int i = 0; i < 4; i++)
        if(position[i] < square  && position[i] > -1)
        {
            square = position[i];
            piece = i;
        }

    return piece;
}

int move_logic::move_send_home() const
{
    int max_damage = 0;
    int piece = NO_MOVE;

    for(int i = 0; i < 4; i++)
    {
        int damage = 0;
        int target = target_square(i);
        int opponents = count_opponents(target);

        if(opponents != 0 && target == 0) //one (or more) opponents on our start square
        {
            switch (opponent_color(target))
            {
            case 1: damage = 39 * opponents; break;
            case 2: damage = 26 * opponents; break;
            case 3: damage = 13 * opponents; break;
            default: damage = 0; break;
            }
        }
        else if(opponents == 1 && is_globe(target) == false) //single opponent on a none globe square
        {
            damage = target - opponent_color(target) * 13;
            if(damage < 0)
                damage += 52;
        }

        if(damage > max_damage)
        {
            max_damage = damage;
            piece = i;
        }
    }

    return piece;
}

int move_logic::is_star(int square) const
{
    switch(square)
    {
    case 5:  return 6;
    case 18: return 6;
    case 31: return 6;
    case 44: return 6;

    case 11: return 7;
    case 24: return 7;
    case 37: return 7;

    case 50: return 6; //jump to goal

    default: return 0;
    }
}

bool move_logic::is_globe(int square) const
{
    return (square % 13) == 0 ||
           (square % 13) == 8;
}

bool move_logic::is_valid_move(int piece) const
{
    if(piece < 0 || piece > 3)
        return false;

    if(position[piece] >= 0 && position[piece] < 99)
        return true;

    if (position[piece] < 0 && dice == 6)
        return true;

    return false;
}

bool move_logic::is_safe_move(int piece) const
{
    int target = target_square(piece);

    if(target == 0 || target > 50) //always safe on start square and goal stretch
        return true;

    int opponents = count_opponents(target);

    if(opponents == 0)
        return true;

    if(opponents == 1 && is_globe(target) == false)
        return true;

    return false;
}

int move_logic::target_square(int piece) const
{
    if(position[piece] < 0)
    {
        if(dice == 6)
            return 0;
        else
            return -1;
    }

    if(position[piece] == 99)
        return 99;

    int square = position[piece] + dice;
    square += is_star(square);

    if(square > 56)            //overshot the goal
        square = 112 - square; //move back on goal stretch

    return square;
}

int move_logic::count_opponents(int square) const
{
    if(square < 0 || square > 50)
        return 0;

    int count = 0;
    for(int i = 4; i < 16; i++)
        if(position[i] == square)
            count++;

    return count;
}

int move_logic::opponent_color(int square) const
{
    for(int i = 4; i < 16; i++)
        if(position[i] == square)
            return (i / 4); //integer division

    return -1; //no opponent on this square
}
