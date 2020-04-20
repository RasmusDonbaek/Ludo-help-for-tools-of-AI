#ifndef TEST_GAME_H
#define TEST_GAME_H

#include "game.h"
#include <string>

class test_game
{
private:
    game g;

public:
    test_game();
    void run_all_tests();

private:
    bool test_reset();
    bool test_rel_pos_outfield_standard();
    bool test_rel_pos_outfield_special();
    bool test_rel_pos_goal_stretch();
    bool test_move_piece_standard();
    bool test_move_piece_square_51();
    bool test_move_piece_around_51();
    bool test_move_piece_send_home();
    bool test_move_piece_two_send_me_home();
    bool test_move_piece_globe_send_me_home();
    bool test_move_piece_start_is_globe();
    bool test_move_piece_start_send_home();
    bool test_move_piece_star_to_goal();
    bool test_move_piece_star_send_home();
    bool test_move_piece_to_goal();
    bool test_move_piece_beyond_goal();
    bool test_move_piece_star_around_51();
    bool test_move_piece_two_on_globe();
    bool test_move_piece_three_on_square();
    bool test_move_piece_move_to_globe();
    bool test_move_piece_move_enter_goal_stretch();
    bool test_move_piece_enforce_legal_move();

    //If adding more unit test classes, these methods should be moved to a abstract base class
    bool content_is_equal(const int expected[], const int actual[], int length);
    bool content_is_not_equal(const int expected[], const int actual[], int length);
    void print(const std::string& message,const int expected[], const int actual[], int length);
    void print_success(const std::string& method);
    void print_failed(const std::string& method);
};

#endif // TEST_GAME_H
