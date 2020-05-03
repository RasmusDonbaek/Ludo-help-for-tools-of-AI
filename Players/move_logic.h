#ifndef MOVE_LOGIC_H
#define MOVE_LOGIC_H

#include <random>
#include <vector>
#include <iostream>

class move_logic
{
private:
    static const int NO_MOVE = -1;

    int dice;
    int* position;

    mutable std::mt19937 generator;
    mutable std::uniform_int_distribution<int> distribution;

public:
    move_logic();
    void set(int dice, int* position);

    // The below functions are marked as 'const' on purpose!
    // 'const' functions are not allowed to alter member variables, ie. dice and position
    // This way we can call set(dice, position) just once,
    // and still call multiple of the below helping functions in a row,
    // knowing that the previous function calls have not messed up the position or dice value.
    int count_valid_moves() const;
    int first_legal_move() const;

    int move_random_legal() const;
    int move_random_safe() const;
    int move_start() const;
    int move_into_goal() const;
    int move_fast() const;
    int move_fast_safe() const;
    int move_early_outfield() const;
    int move_send_home() const;

    int  is_star(int square) const;
    bool is_globe(int square) const;
    bool is_valid_move(int piece) const;
    bool is_safe_move(int piece) const; //precondition: is legal to move 'piece'

    int target_square(int piece) const; //precondition: piece [0,3]
    int count_opponents(int square) const;
    int opponent_color(int square) const;
};

#endif // MOVE_LOGIC_H
