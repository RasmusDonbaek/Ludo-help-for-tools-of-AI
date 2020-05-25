#include "ai_player.h"
#include <iostream>
#include <exception>

AI_player::AI_player(Q_Table& table) : AI_player(table, 0.1)
{

}

AI_player::AI_player(Q_Table &table, double learning_rate)
{
    q_table = &table;
    set_learning_rate(learning_rate);

    std::random_device rd;
    generator = std::mt19937(rd());
}

void AI_player::debug_set_q_table(int index, double value)
{
    q_table->set_value(index, value);
}

double AI_player::debug_get_q_table(int index)
{
    return q_table->get_value(index);
}

void AI_player::learning_on()
{
    do_learning = true;
}

void AI_player::learning_off()
{
    do_learning = false;
}

void AI_player::set_learning_rate(double value)
{
    if(value < 0.0 || value > 1.0)
        throw std::exception();

    alpha = value;
}

void AI_player::post_game_analysis(bool you_won)
{
    if(do_learning)
    {
        int last = move_count - 1;
        if(you_won == false)
        {
            int state = history[last];
            double q_value = q_table->get_value(state);
            double d_q = alpha * (0 - q_value);
            q_table->set_value(state, q_value + d_q);
        }

        last--;
        for(int i = last; i >= 0; i--)
        {
            int state = history[i];
            int next_state = history[i+1];

            double q_value = q_table->get_value(state);
            double q_next  = q_table->get_value(next_state);

            double d_q = alpha * (q_next - q_value);

            q_table->set_value(state, q_value + d_q);
        }
    }

    move_count = 0;
}

int AI_player::make_decision()
{
//***************************************************************
//      Identify all valid moves in the current position        *
//***************************************************************
    int valid_moves[4];
    int valid_count = 0;

    for(int i = 0; i < 4; i++)
    {
        if(is_valid_move(i))
        {
            valid_moves[valid_count] = i;
            valid_count++;
        }
    }


//***************************************************************
//      If forced move, then no choice                          *
//***************************************************************
    if(valid_count == 0) return -1;
    if(valid_count == 1) return valid_moves[0];


//***************************************************************
//      Find the best moves of out multiple options             *
//***************************************************************
    double max_val = -1;
    int max_count = 0;
    int max_moves[4];
    for(int i = 0; i < valid_count; i++)
    {
        int piece = valid_moves[i];
        calc_post_move_position(piece);
        int q_index = get_q_index();
        double q_value = q_table->get_value(q_index);

        if(q_value > max_val)
        {
            max_val = q_value;
            max_moves[0] = piece;
            max_count = 1;
        }
        else if(q_value == max_val)
        {
            max_moves[max_count] = piece;
            max_count++;
        }
    }


//***************************************************************
//      If one move is better than the rest                     *
//***************************************************************
    if(max_count < 1) throw std::exception();
    if(max_count == 1) return max_moves[0];


//***************************************************************
//      Randomly selecet among multiple equally good moves      *
//***************************************************************
    distribution = std::uniform_int_distribution<int>(0, max_count - 1);
    return max_moves[distribution(generator)];
}

void AI_player::post_move_analysis()
{
    for(int i = 0; i < 16; i++)
        post_move_position[i] = position[i];

    if(do_learning)
    {
        int q_index = get_q_index();
        q_table->record_visit(q_index);
        history[move_count] = q_index;
    }

    move_count++;
}

void AI_player::calc_post_move_position(int move_piece)
{
    for(int i = 0; i < 16; i++)
        post_move_position[i] = position[i];

    if(move_piece == -1)
        return;

    int square = position[move_piece];

    if(square == -1)
        move_start(move_piece);
    else
    {
        int target = square + dice;
        target += is_star(target);

        if(target == 56)
            post_move_position[move_piece] = 99;
        else if(target > 50 && target < 56)
            post_move_position[move_piece] = target;
        else if(target > 55)
            post_move_position[move_piece] = 112 - target;
        else if (target > 0 && target < 51)
        {
            int opponents = count_opponents(target);

            if(opponents == 0)
                post_move_position[move_piece] = target;
            else if(opponents == 1)
            {
                if(is_globe(target))
                    post_move_position[move_piece] = -1;
                else
                {
                    post_move_position[move_piece] = target;
                    send_them_home(target);
                }
            }
            else
                post_move_position[move_piece] = -1;
        }
        else
            throw std::exception();
    }
}

int AI_player::get_q_index() const
{
    //Defining the 6 Q-index variables
    int in_goal_level     = 0;
    int pip_count_level   = 0;
    int unprotected_level = 0;
    int threat_level      = 0;
    int best_opp_level    = 0;
    int second_opp_level  = 0;

    //Helper variables used in calculating the value of the 6 Q-index variables
    int in_goal = count_own_pieces_in_goal();
    if(in_goal == 4)
        return Q_Table::WON_STATE;

    int pip_count = calc_own_pip_count();
    int pip_divider = get_pip_divider(in_goal_level);
    int unprotected_count = calc_unprotected_pips();
    if(unprotected_count > (248 - pip_count))
        throw std::exception();
    int unprotected_divider = get_unprotected_divider(in_goal);
    int pips_under_attack = calc_pips_under_attack();
    if(pips_under_attack > unprotected_count)
    {
        //Used for debugging
        std::cout << "pips_under_attack > unprotected_count" << std::endl;
        std::cout << "pips_under_attack: " << pips_under_attack << std::endl;
        std::cout << "unprotected_count: " << unprotected_count << std::endl;
        for(int i = 0; i < 16; i++)
            std::cout << post_move_position[i] << ", ";
        std::cout << std::endl;
        throw std::exception();
    }
    int opp_1_pips = calc_opponent_pips(1);
    int opp_2_pips = calc_opponent_pips(2);
    int opp_3_pips = calc_opponent_pips(3);
    int best_opp = get_lowest(opp_1_pips, opp_2_pips, opp_3_pips);
    int second_opp = get_middle(opp_1_pips, opp_2_pips, opp_3_pips);
    int pip_diff = second_opp - best_opp;
    int diff_divider = 7;

    //Assigning the 6 Q-index variables
    in_goal_level = in_goal;

    pip_count_level = pip_count / pip_divider; //yes, integer division
    if(pip_count_level > 9)
        pip_count_level = 9;

    unprotected_level = unprotected_count / unprotected_divider; //yes, integer division
    if(unprotected_level > 7)
        unprotected_level = 7;

    if(unprotected_count == 0 || pips_under_attack == 0)
        threat_level = 0;
    else
    {
        threat_level = 1 + (3*pips_under_attack)/unprotected_count;
        if(threat_level > 3)
            threat_level = 3;
    }

    best_opp_level = calc_best_opp_level(pip_count, best_opp);

    second_opp_level = pip_diff / diff_divider; //yes integer division
    if(second_opp_level > 7)
        second_opp_level = 7;

    //Convert to and return Q-index
    return q_table->get_index(in_goal_level, pip_count_level, unprotected_level, threat_level, best_opp_level, second_opp_level);
}


void AI_player::move_start(int piece)
{
    if(post_move_position[piece] == -1 && dice == 6)
    {
        post_move_position[piece] = 0;
        send_them_home(0);
    }
    else
        throw std::exception();
}

void AI_player::send_them_home(int square)
{
    for(int i = 4; i < 16; i++)
        if(post_move_position[i] == square)
            post_move_position[i] = -1;
}

int AI_player::count_opponents(int square) const
{
    int count = 0;

    for(int i = 4; i < 16; i++)
        if(post_move_position[i] == square)
            count++;

    return count;
}

bool AI_player::is_globe(int square) const
{
    return (square % 13) == 0 ||
           (square % 13) == 8;
}

int AI_player::count_own_pieces_in_goal() const
{
    int in_goal = 0;
    for(int i = 0; i < 4; i++)
        if(post_move_position[i] == 99)
            in_goal++;

    return in_goal;
}

int AI_player::calc_own_pip_count() const
{
    int total = 0;

    for(int i = 0; i < 4; i++)
        total += pips_to_goal(post_move_position[i]);

    return total;
}

int AI_player::calc_opponent_pips(int opponent) const
{
    if(opponent < 1 || opponent > 3) throw std::exception();

    int total = 0;

    int piece_begin = 4 * opponent;
    int piece_end = piece_begin + 4;
    int goal_stretch_begin = 51 + 5 * opponent;
    int goal_stretch_end = goal_stretch_begin + 4;
    int dead = (13 * opponent) - 1;
    int pivoit = dead - 1;

    for(int i = piece_begin; i < piece_end; i++)
    {
        int square = post_move_position[i];

        if(square == -1)
            total += 62;
        else if(square == 99)
            continue;
        else if(square >= goal_stretch_begin && square <= goal_stretch_end)
            total += (1 + goal_stretch_end - square);
        else if (square == dead)
            throw std::exception();
        else if(square >= 0 && square <= 51)
        {
            int remaining = pivoit - square;
            if(remaining < 0)
                remaining += 52;

            total += remaining + 6;
        }
        else
            throw std::exception();
    }

    return total;
}

int AI_player::calc_unprotected_pips() const
{
    int total = 0;

    for(int i = 0; i < 4; i++)
    {
        int square = post_move_position[i];
        if(is_protected(square) == false)
            total += square + 6; //6 pips for getting out of home box
    }

    return total;
}

int AI_player::calc_pips_under_attack() const
{
    int total = 0;

    for(int i = 0; i < 4; i++)
    {
        int square = post_move_position[i];

        if(is_protected(square))
            continue;
        else if(is_globe_danger(square))
            total += square + 6; // +6 for getting out of home
        else
        {
            int min = square - 6;
            if(square % 13 == 5) { min -= 7; }       //they need to hit the star before me
            else if(square % 13 == 11) { min -= 6; } //they need to hit the star before me

            int rotation = 0;
            if(min < 0) { rotation = 13; }
            min += rotation;
            int max = min + 5;

            for(int i = 4; i < 16; i++)
            {
                int color = i / 4;
                int limit = 13 * color - 2;
                int opp_square = position[i];

                if(opp_square == -1) continue; //globes have already been accounted for
                if(opp_square  > 51) continue; //on its goal stretch or in its goal
                if(opp_square <= limit && square > limit) continue; //will walk up its goal stretch

                opp_square += rotation;
                if(opp_square > 51)
                    opp_square -= 52;

                if(opp_square >= min && opp_square <= max)
                {
                    total += square + 6; // +6 for getting out of home
                    break;               // can only be send home once
                }
            }
        }

    }

    return total;
}

int AI_player::calc_best_opp_level(int own_pip_count, int opp_pip_count) const
{
    int pip_count = std::min(179, std::max(50, own_pip_count)); //limit pip_count to rage [50, 179]
    int step_size = std::min(12, std::max(5, pip_count / 10));  //limit step_size to range [5, 12]

    if(opp_pip_count      > (pip_count + 4 * step_size))     return 0; //I'm way ahead
    else if(opp_pip_count > (pip_count + 2 * step_size))     return 1;
    else if(opp_pip_count > (pip_count + step_size))         return 2;
    else if(opp_pip_count > (pip_count - 1))                 return 3;
    else if(opp_pip_count > (pip_count - 2 - step_size))     return 4;
    else if(opp_pip_count > (pip_count - 2 - 2 * step_size)) return 5;
    else if(opp_pip_count > (pip_count - 2 - 4 * step_size)) return 6;
    else if(opp_pip_count > (pip_count - 2 - 7 * step_size)) return 7;
    else if(opp_pip_count > 0)                               return 8; //I'm way behind
    else throw std::exception();
}

int AI_player::get_pip_divider(int in_goal) const
{
    switch (in_goal)
    {
    case 0: return 24;
    case 1: return 18;
    case 2: return 12;
    case 3: return  6;
    default: throw std::exception();
    }
}

int AI_player::get_unprotected_divider(int in_goal) const
{
    switch (in_goal)
    {
    case 0: return 13;
    case 1: return 11;
    case 2: return  8;
    case 3: return  5;
    default: throw std::exception();
    }
}

int AI_player::get_lowest(int a, int b, int c) const
{
    return std::min(a, std::min(b, c));
}

int AI_player::get_middle(int a, int b, int c) const
{
    if( a >= b && a >= c)
        return std::max(b, c);  //if a is largest
    else if(b >= a && b >= c)
        return std::max(a, c);  //if b is largest
    else
        return std::max(a, b);  //if c is largest
}

int AI_player::pips_to_goal(int square) const
{
    if(square == -1) return 62;
    if(square == 99) return  0;

    if(square >= 0 && square <= 55)
        return 56 - square;

    throw std::exception();
}

bool AI_player::is_protected(int square) const
{
    switch (square)
    {
    case -1: //home
    case  0: //start globe
    case  8: //normal globe
    case 21: //normal globe
    case 34: //normal globe
    case 47: //normal globe
    case 99: //goal
        return true;

    case 13: //opponents home globe
    case 26: //opponents home globe
    case 39: //opponents home globe
        return false;
    }

    if( square >= 51 && square <= 55)
        return true; //home stretch

    //count own pieces on the square
    int count = 0;
    for(int i = 0; i < 4; i++)
    {
        if(post_move_position[i] == square)
            count++;
    }

    //protected if at least two on the square
    return (count > 1);
}

bool AI_player::is_globe_danger(int square) const
{
    switch (square)
    {
    case 13: return has_piece_home(1);
    case 26: return has_piece_home(2);
    case 39: return has_piece_home(3);
    default: return false;
    }
}

bool AI_player::has_piece_home(int opponent) const
{
    int begin = 4 * opponent;
    int end = begin + 4;

    for(int i = begin; i < end; i++)
        if(post_move_position[i] == -1)
            return true;

    return false;
}

int AI_player::is_star(int square) const
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

