#include "test_game.h"

test_game::test_game()
{ }

void test_game::run_all_tests()
{
    std::cout << "Run all tests of class 'game'' ..." << std::endl;

    //Test the positions
    test_reset()                     ? print_success("test_reset")                     : print_failed("test_reset");
    test_rel_pos_outfield_standard() ? print_success("test_rel_pos_outfield_standard") : print_failed("test_rel_pos_outfield_standard");
    test_rel_pos_outfield_special()  ? print_success("test_rel_pos_outfield_special")  : print_failed("test_rel_pos_outfield_special");
    test_rel_pos_goal_stretch()      ? print_success("test_rel_pos_goal_stretch")      : print_failed("test_rel_pos_goal_stretch");

    //Test piece movements
    test_move_piece_standard()       ? print_success("test_move_piece_standard")       : print_failed("test_move_piece_standard");
    test_move_piece_square_51()      ? print_success("test_move_piece_square_51")      : print_failed("test_move_piece_square_51");
    test_move_piece_around_51()      ? print_success("test_move_piece_around_51")      : print_failed("test_move_piece_around_51");
    test_move_piece_send_home()      ? print_success("test_move_piece_send_home")      : print_failed("test_move_piece_send_home");
    test_move_piece_two_send_me_home()   ? print_success("test_move_piece_two_send_me_home")   : print_failed("test_move_piece_two_send_me_home");
    test_move_piece_globe_send_me_home() ? print_success("test_move_piece_globe_send_me_home") : print_failed("test_move_piece_globe_send_me_home");
    test_move_piece_start_is_globe() ? print_success("test_move_piece_start_is_globe") : print_failed("test_move_piece_start_is_globe");
    test_move_piece_start_send_home()? print_success("test_move_pieve_start_send_home"): print_failed("test_move_pieve_start_send_home");
    test_move_piece_star_to_goal()   ? print_success("test_move_piece_star_to_goal")   : print_failed("test_move_piece_star_to_goal");
    test_move_piece_star_send_home() ? print_success("test_move_piece_star_send_home") : print_failed("test_move_piece_star_send_home");
    test_move_piece_to_goal()        ? print_success("test_move_piece_to_goal")        : print_failed("test_move_piece_to_goal");
    test_move_piece_beyond_goal()    ? print_success("test_move_piece_beyond_goal")    : print_failed("test_move_piece_beyond_goal");
    test_move_piece_star_around_51() ? print_success("test_move_piece_star_around_51") : print_failed("test_move_piece_star_around_51");
    test_move_piece_two_on_globe()   ? print_success("test_move_piece_two_on_globe")   : print_failed("test_move_piece_two_on_globe");
    test_move_piece_three_on_square()? print_success("test_move_piece_three_on_square"): print_failed("test_move_piece_three_on_square");
    test_move_piece_move_to_globe()  ? print_success("test_move_piece_move_to_globe")  : print_failed("test_move_piece_move_to_globe");
    test_move_piece_move_enter_goal_stretch() ? print_success("test_move_piece_move_enter_goal_stretch") : print_failed("test_move_piece_move_enter_goal_stretch");
    test_move_piece_enforce_legal_move()      ? print_success("test_move_piece_enforce_legal_move")      : print_failed("test_move_piece_enforce_legal_move");

    std::cout << "All tests of class 'game' done" << std::endl << std::endl;
}

bool test_game::test_reset()
{
    g.position[0] = -1;  g.position[1] = 0;   g.position[2] = 10;  g.position[3] = 11;
    g.position[4] = 20;  g.position[5] = 21;  g.position[6] = 22;  g.position[7] = 23;
    g.position[8] = -1;  g.position[9] = 31;  g.position[10] = 32; g.position[11] = 99;
    g.position[12] = 99; g.position[13] = -1; g.position[14] = 40; g.position[15] = 41;

    g.reset();

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_reset", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_rel_pos_outfield_standard()
{
    //Set up position with pieces on outfield squares
    g.position[0] = 0;   g.position[1] = 24; g.position[2] = 30;  g.position[3] = 48;
    g.position[4] = 19;  g.position[5] = 26; g.position[6] = 45;  g.position[7] = 4;
    g.position[8] = 33;  g.position[9] = 46; g.position[10] = 2;  g.position[11] = 15;
    g.position[12] = 40; g.position[13] = 5; g.position[14] = 14; g.position[15] = 29;

    //Test relative position from the perspective of player 0
    g.color = 0;
    g.update_relative_position();
    int* actual = g.rel_pos_and_dice.position;
    int expected[] = {0, 24, 30, 48, 19, 26, 45, 4, 33, 46, 2, 15, 40, 5, 14, 29};
    //print("test_rel_pos_outfield_standard", expected, actual, game::piece_count);
    if(content_is_not_equal(expected, actual, game::piece_count))
        return false;

    //Test relative position from the perspective of player 2
    g.color = 2;
    g.update_relative_position();
    actual = g.rel_pos_and_dice.position;
    int expected2[] = {7, 20, 28, 41, 14, 31, 40, 3, 26, 50, 4, 22, 45, 0, 19, 30};
    //print("test_rel_pos_outfield_standard", expected, actual, game::piece_count);
    return content_is_equal(expected2, actual, game::piece_count);
}

bool test_game::test_rel_pos_outfield_special()
{
    g.reset();

    g.position[0] = 12;
    g.position[4] = 25;
    g.position[8] = 38;
    g.position[12] = 51;

    g.color = 1;
    g.update_relative_position();
    int* actual = g.rel_pos_and_dice.position;
    int expected[] = {12, -1, -1, -1, 25, -1, -1, -1, 38, -1, -1, -1, 51, -1, -1, -1};
    //print("test_rel_pos_outfield_special", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_rel_pos_goal_stretch()
{
    //Set up position with pieces on goal stretch
    g.position[0] = 50;  g.position[1] = 52;  g.position[2] = 53;  g.position[3] = 56;
    g.position[4] = 11;  g.position[5] = 57;  g.position[6] = 58;  g.position[7] = 61;
    g.position[8] = 24;  g.position[9] = 62;  g.position[10] = 63; g.position[11] = 66;
    g.position[12] = 37; g.position[13] = 67; g.position[14] = 68; g.position[15] = 71;

    g.color = 2;
    g.update_relative_position();
    int* actual = g.rel_pos_and_dice.position;
    int expected[] = {50, 51, 52, 55, 11, 56, 57, 60, 24, 61, 62, 65, 37, 66, 67, 70};
    //print("test_rel_pos_goal_stretch", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_standard()
{
    g.reset();

    g.color = 1;
    g.position[5] = 28;
    g.game_dice.set(4);

    g.move_piece(1);

    int expected[] = {-1, -1, -1, -1, -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_standard", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_square_51()
{
    g.reset();

    g.color = 2;
    g.position[10] = 49;
    g.game_dice.set(2);

    g.move_piece(2);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 51, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_square_51", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_around_51()
{
    g.reset();

    g.color = 3;
    g.position[15] = 47;
    g.game_dice.set(6);

    g.move_piece(3);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1};
    int* actual = g.position;
    //print("test_move_piece_around_51", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_send_home()
{
    g.reset();

    g.position[0] = 0;
    g.position[1] = 0;
    g.position[2] = 0;
    g.position[3] = 0;
    g.position[4] = 1;
    g.position[5] = 2;
    g.position[6] = 3;
    g.position[7] = 4;

    g.color = 0; g.game_dice.set(1); g.move_piece(0);
    g.color = 0; g.game_dice.set(2); g.move_piece(1);
    g.color = 0; g.game_dice.set(3); g.move_piece(2);
    g.color = 0; g.game_dice.set(4); g.move_piece(3);

    int expected[] = {1, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_send_home", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_two_send_me_home()
{
    g.reset();

    g.color = 1;
    g.position[5] = 20;
    g.position[10] = 22;
    g.position[11] = 22;
    g.game_dice.set(2);

    g.move_piece(1);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 22, 22, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_two_send_me_home", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_globe_send_me_home()
{
    g.reset();

    g.color = 2;
    g.position[11] = 4;
    g.position[14] = 8;
    g.game_dice.set(4);

    g.move_piece(3);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, -1};
    int* actual = g.position;
    //print("test_move_piece_globe_send_me_home", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_start_is_globe()
{
    g.reset();

    g.color = 3;
    g.position[0] = 13;
    g.position[13] = 12;
    g.game_dice.set(1);

    g.move_piece(1);

    int expected[] = {13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_start_is_globe", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_start_send_home()
{
    g.reset();

    g.position[8] = 0;
    g.position[9] = 0;

    g.color = 0;
    g.game_dice.set(6);
    g.move_piece(1);

    int expected[] =  {-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_start_send_home", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_star_to_goal()
{
    g.reset();
    g.position[0] = 47;
    g.position[4] = 8;
    g.position[8] = 21;
    g.position[12] = 34;

    g.color = 0; g.game_dice.set(3); g.move_piece(0);
    g.color = 1; g.game_dice.set(3); g.move_piece(0);
    g.color = 2; g.game_dice.set(3); g.move_piece(0);
    g.color = 3; g.game_dice.set(3); g.move_piece(0);

    int expected[] = {99, -1, -1, -1, 99, -1, -1, -1, 99, -1, -1, -1, 99, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_star_to_goal", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_star_send_home()
{
    g.reset();

    g.position[6] = 50;
    g.position[10] = 43;
    g.position[14] = 44;

    g.color = 2;
    g.game_dice.set(1);
    g.move_piece(2);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 50, -1, -1, -1, 44, -1};
    int* actual = g.position;
    //print("test_move_piece_star_send_home", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_to_goal()
{
    g.reset();

    g.position[0] = 55;
    g.position[5] = 60;
    g.position[10] = 65;
    g.position[15] = 70;

    g.color = 0; g.game_dice.set(2); g.move_piece(0);
    g.color = 1; g.game_dice.set(2); g.move_piece(1);
    g.color = 2; g.game_dice.set(2); g.move_piece(2);
    g.color = 3; g.game_dice.set(2); g.move_piece(3);

    int expected[] = {99, -1, -1, -1, -1, 99, -1, -1, -1, -1, 99, -1, -1, -1, -1, 99};
    int* actual = g.position;
    //print("test_move_piece_to_goal", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_beyond_goal()
{
    g.reset();

    g.position[0] = 55;
    g.position[5] = 60;
    g.position[10] = 65;
    g.position[15] = 70;

    g.color = 0; g.game_dice.set(6); g.move_piece(0);
    g.color = 1; g.game_dice.set(6); g.move_piece(1);
    g.color = 2; g.game_dice.set(6); g.move_piece(2);
    g.color = 3; g.game_dice.set(6); g.move_piece(3);

    int expected[] = {53, -1, -1, -1, -1, 58, -1, -1, -1, -1, 63, -1, -1, -1, -1, 68};
    int* actual = g.position;
    //print("test_move_piece_beyond_goal", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_star_around_51()
{
    g.reset();

    g.position[10] = 48;
    g.color = 2;
    g.game_dice.set(2);
    g.move_piece(2);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_star_around_51", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

//Don't send yourself home on a globe
bool test_game::test_move_piece_two_on_globe()
{
    g.reset();

    g.position[4] = 8;
    g.position[5] = 6;

    g.color = 1;
    g.game_dice.set(2);
    g.move_piece(1);

    int expected[] = {-1, -1, -1, -1, 8, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_two_on_globe", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

//Don't send yourself home just because you already have two pieces on the square
bool test_game::test_move_piece_three_on_square()
{
    g.reset();

    g.position[13] = 46;
    g.position[14] = 46;
    g.position[15] = 45;

    g.color = 3;
    g.game_dice.set(1);
    g.move_piece(3);

    int expected[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 46, 46, 46};
    int* actual = g.position;
    //print("test_move_piece_three_on_square", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

//Should be safe to land on a globe
bool test_game::test_move_piece_move_to_globe()
{
    g.reset();

    g.position[3] = 19;
    g.color = 0;
    g.game_dice.set(2);
    g.move_piece(3);

    int expected[] = {-1, -1, -1, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_move_to_globe", expected, actua, game::piece_countl);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_move_enter_goal_stretch()
{
    g.reset();

    g.position[0] = 48;
    g.position[4] = 9;
    g.position[8] = 22;
    g.position[12] = 35;

    g.color = 0; g.game_dice.set(3); g.move_piece(0);
    g.color = 1; g.game_dice.set(3); g.move_piece(0);
    g.color = 2; g.game_dice.set(3); g.move_piece(0);
    g.color = 3; g.game_dice.set(3); g.move_piece(0);

    int expected[] = {52, -1, -1, -1, 57, -1, -1, -1, 62, -1, -1, -1, 67, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_move_enter_goal_stretch", expected, actual, game::piece_count);

    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::test_move_piece_enforce_legal_move()
{
    g.reset();

    g.position[4] = 99; //rel. piece 0
    g.position[6] = 20; //rel. piece 2

    g.color = 1;
    g.game_dice.set(2);
    g.move_piece(3); //trying an invalid move

    int expected[] = {-1, -1, -1, -1, 99, -1, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int* actual = g.position;
    //print("test_move_piece_enforce_legal_move", expected, actual, game::piece_count);


    return content_is_equal(expected, actual, game::piece_count);
}

bool test_game::content_is_equal(const int* expected, const int actual[], int length)
{
    for(int i = 0; i < length; i++)
        if(actual[i] != expected[i])
            return false;

    return true;
}

bool test_game::content_is_not_equal(const int expected[], const int actual[], int length)
{
    return ! content_is_equal(expected, actual, length);
}

void test_game::print(const std::string& message, const int expected[], const int actual[], int length)
{
    std::cout << std::endl << message << std::endl;

    std::cout << "Expected: ";
    for(int i = 0; i < length; i++)
        std::cout << expected[i] << ", ";
    std::cout << std::endl;

    std::cout << "Actual:   ";
    for(int i = 0; i < length; i++)
        std::cout << actual[i] << ", ";
    std::cout << std::endl << std::endl;
}

void test_game::print_success(const std::string& method)
{
    std::cout << "Passed: " << method << std::endl;
}

void test_game::print_failed(const std::string& method)
{
    std::cout << std::endl << std::endl << "FAILED !!! " << method << std::endl << std::endl;
}


