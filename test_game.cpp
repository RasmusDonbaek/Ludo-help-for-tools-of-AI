#include "test_game.h"


test_game::test_game()
{ }

void test_game::run_all_tests()
{
    std::cout << "Run all tests of class 'game'' ..." << std::endl;

    //Test the positions
    test_reset()                        ? print_success("test_reset")                     : print_failed("test_reset");
    test_rel_pos_outfield_standard()    ? print_success("test_rel_pos_outfield_standard") : print_failed("test_rel_pos_outfield_standard");
    test_rel_pos_outfield_special()     ? print_success("test_rel_pos_outfield_special")  : print_failed("test_rel_pos_outfield_special");
    test_rel_pos_goal_stretch()         ? print_success("test_rel_pos_goal_stretch")      : print_failed("test_rel_pos_goal_stretch");

    //Test piece movements
    test_move_piece_standard()                  ? print_success("test_move_piece_standard")                 : print_failed("test_move_piece_standard");
    test_move_piece_square_51()                 ? print_success("test_move_piece_square_51")                : print_failed("test_move_piece_square_51");
    test_move_piece_around_51()                 ? print_success("test_move_piece_around_51")                : print_failed("test_move_piece_around_51");
    test_move_piece_send_home()                 ? print_success("test_move_piece_send_home")                : print_failed("test_move_piece_send_home");
    test_move_piece_two_send_me_home()          ? print_success("test_move_piece_two_send_me_home")         : print_failed("test_move_piece_two_send_me_home");
    test_move_piece_globe_send_me_home()        ? print_success("test_move_piece_globe_send_me_home")       : print_failed("test_move_piece_globe_send_me_home");
    test_move_piece_start_is_globe()            ? print_success("test_move_piece_start_is_globe")           : print_failed("test_move_piece_start_is_globe");
    test_move_piece_start_send_home()           ? print_success("test_move_pieve_start_send_home")          : print_failed("test_move_pieve_start_send_home");
    test_move_piece_star_to_goal()              ? print_success("test_move_piece_star_to_goal")             : print_failed("test_move_piece_star_to_goal");
    test_move_piece_star_send_home()            ? print_success("test_move_piece_star_send_home")           : print_failed("test_move_piece_star_send_home");
    test_move_piece_to_goal()                   ? print_success("test_move_piece_to_goal")                  : print_failed("test_move_piece_to_goal");
    test_move_piece_beyond_goal()               ? print_success("test_move_piece_beyond_goal")              : print_failed("test_move_piece_beyond_goal");
    test_move_piece_star_around_51()            ? print_success("test_move_piece_star_around_51")           : print_failed("test_move_piece_star_around_51");
    test_move_piece_two_on_globe()              ? print_success("test_move_piece_two_on_globe")             : print_failed("test_move_piece_two_on_globe");
    test_move_piece_three_on_square()           ? print_success("test_move_piece_three_on_square")          : print_failed("test_move_piece_three_on_square");
    test_move_piece_move_to_globe()             ? print_success("test_move_piece_move_to_globe")            : print_failed("test_move_piece_move_to_globe");
    test_move_piece_move_enter_goal_stretch()   ? print_success("test_move_piece_move_enter_goal_stretch")  : print_failed("test_move_piece_move_enter_goal_stretch");
    test_move_piece_enforce_legal_move() ? print_success("test_move_piece_enforce_legal_move")              : print_failed("test_move_piece_enforce_legal_move");

    std::cout << "All tests of class 'game' done" << std::endl << std::endl;
}

bool test_game::test_reset()
{
    g.turn_complete = false;
    g.game_complete = true;

    g.player_positions[0] = -1;  g.player_positions[1] = 0;   g.player_positions[2] = 10;  g.player_positions[3] = 11;
    g.player_positions[4] = 20;  g.player_positions[5] = 21;  g.player_positions[6] = 22;  g.player_positions[7] = 23;
    g.player_positions[8] = -1;  g.player_positions[9] = 31;  g.player_positions[10] = 32; g.player_positions[11] = 99;
    g.player_positions[12] = 99; g.player_positions[13] = -1; g.player_positions[14] = 40; g.player_positions[15] = 41;

    g.reset();

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_reset", expected, actual);

    return
            (g.turn_complete == true) &&
            (g.game_complete == false) &&
            content_is_equal(expected, actual);
}

bool test_game::test_rel_pos_outfield_standard()
{
    //Set up position with pieces on outfield squares
    g.player_positions[0] = 0;   g.player_positions[1] = 24; g.player_positions[2] = 30;  g.player_positions[3] = 48;
    g.player_positions[4] = 19;  g.player_positions[5] = 26; g.player_positions[6] = 45;  g.player_positions[7] = 4;
    g.player_positions[8] = 33;  g.player_positions[9] = 46; g.player_positions[10] = 2;  g.player_positions[11] = 15;
    g.player_positions[12] = 40; g.player_positions[13] = 5; g.player_positions[14] = 14; g.player_positions[15] = 29;

    //Test relative position from the perspective of player 0
    g.color = 0;
    std::vector<int> expected = {0, 24, 30, 48, 19, 26, 45, 4, 33, 46, 2, 15, 40, 5, 14, 29};
    std::vector<int> actual = g.relativePosition();
    //print("test_rel_pos_outfield_standard", expected, actual);
    if(content_is_not_equal(expected, actual))
        return false;

    //Test relative position from the perspective of player 2
    g.color = 2;
    expected = {7, 20, 28, 41, 14, 31, 40, 3, 26, 50, 4, 22, 45, 0, 19, 30};
    actual = g.relativePosition();
    //print("test_rel_pos_outfield_standard", expected, actual);
    return content_is_equal(expected, actual);
}

//relative position of absolute square index 12, 25 , 38 and 51
bool test_game::test_rel_pos_outfield_special()
{
    g.reset();

    g.player_positions[0] = 12;
    g.player_positions[4] = 25;
    g.player_positions[8] = 38;
    g.player_positions[12] = 51;

    g.color = 1;
    std::vector<int> expected = {12, -1, -1, -1, 25, -1, -1, -1, 38, -1, -1, -1, 51, -1, -1, -1};
    std::vector<int> actual = g.relativePosition();
    //print("test_rel_pos_outfield_special", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_rel_pos_goal_stretch()
{
    //Set up position with pieces on goal stretch
    g.player_positions[0] = 50;  g.player_positions[1] = 52;  g.player_positions[2] = 53;  g.player_positions[3] = 56;
    g.player_positions[4] = 11;  g.player_positions[5] = 57;  g.player_positions[6] = 58;  g.player_positions[7] = 61;
    g.player_positions[8] = 24;  g.player_positions[9] = 62;  g.player_positions[10] = 63; g.player_positions[11] = 66;
    g.player_positions[12] = 37; g.player_positions[13] = 67; g.player_positions[14] = 68; g.player_positions[15] = 71;

    g.color = 2;
    std::vector<int> expected = {50, 51, 52, 55, 11, 56, 57, 60, 24, 61, 62, 65, 37, 66, 67, 70};
    std::vector<int> actual = g.relativePosition();
    //print("test_rel_pos_goal_stretch", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_standard()
{
    g.reset();

    g.color = 1;
    g.player_positions[5] = 28;
    g.dice_result = 4;

    g.movePiece(1);

    std::vector<int> expected = {-1, -1, -1, -1, -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_standard", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_square_51()
{
    g.reset();

    g.color = 2;
    g.player_positions[10] = 49;
    g.dice_result = 2;

    g.movePiece(2);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 51, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_square_51", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_around_51()
{
    g.reset();

    g.color = 3;
    g.player_positions[15] = 47;
    g.dice_result = 6;

    g.movePiece(3);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_around_51", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_send_home()
{
    g.reset();

    g.player_positions[0] = 0;
    g.player_positions[1] = 0;
    g.player_positions[2] = 0;
    g.player_positions[3] = 0;
    g.player_positions[4] = 1;
    g.player_positions[5] = 2;
    g.player_positions[6] = 3;
    g.player_positions[7] = 4;

    g.color = 0; g.dice_result = 1; g.movePiece(0);
    g.color = 0; g.dice_result = 2; g.movePiece(1);
    g.color = 0; g.dice_result = 3; g.movePiece(2);
    g.color = 0; g.dice_result = 4; g.movePiece(3);

    std::vector<int> expected = {1, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_send_home", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_two_send_me_home()
{
    g.reset();

    g.color = 1;
    g.player_positions[5] = 20;
    g.player_positions[10] = 22;
    g.player_positions[11] = 22;
    g.dice_result = 2;

    g.movePiece(1);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 22, 22, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_two_send_me_home", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_globe_send_me_home()
{
    g.reset();

    g.color = 2;
    g.player_positions[11] = 4;
    g.player_positions[14] = 8;
    g.dice_result = 4;

    g.movePiece(3);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_globe_send_me_home", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_start_is_globe()
{
    g.reset();

    g.color = 3;
    g.player_positions[0] = 13;
    g.player_positions[13] = 12;
    g.dice_result = 1;

    g.movePiece(1);

    std::vector<int> expected = {13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_start_is_globe", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_start_send_home()
{
    g.reset();

    g.player_positions[8] = 0;
    g.player_positions[9] = 0;

    g.color = 0;
    g.dice_result = 6;
    g.movePiece(1);

    std::vector<int> expected =  {-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_start_send_home", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_star_to_goal()
{
    g.reset();
    g.player_positions[0] = 47;
    g.player_positions[4] = 8;
    g.player_positions[8] = 21;
    g.player_positions[12] = 34;

    g.color = 0; g.dice_result = 3; g.movePiece(0);
    g.color = 1; g.dice_result = 3; g.movePiece(0);
    g.color = 2; g.dice_result = 3; g.movePiece(0);
    g.color = 3; g.dice_result = 3; g.movePiece(0);

    std::vector<int> expected = {99, -1, -1, -1, 99, -1, -1, -1, 99, -1, -1, -1, 99, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_star_to_goal", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_star_send_home()
{
    g.reset();

    g.player_positions[6] = 50;
    g.player_positions[10] = 43;
    g.player_positions[14] = 44;

    g.color = 2;
    g.dice_result = 1;
    g.movePiece(2);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 50, -1, -1, -1, 44, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_star_send_home", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_to_goal()
{
    g.reset();

    g.player_positions[0] = 55;
    g.player_positions[5] = 60;
    g.player_positions[10] = 65;
    g.player_positions[15] = 70;

    g.color = 0; g.dice_result = 2;
    g.movePiece(0);
    g.color = 1; g.dice_result = 2;
    g.movePiece(1);
    g.color = 2; g.dice_result = 2; g.movePiece(2);
    g.color = 3; g.dice_result = 2; g.movePiece(3);

    std::vector<int> expected = {99, -1, -1, -1, -1, 99, -1, -1, -1, -1, 99, -1, -1, -1, -1, 99};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_to_goal", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_beyond_goal()
{
    g.reset();

    g.player_positions[0] = 55;
    g.player_positions[5] = 60;
    g.player_positions[10] = 65;
    g.player_positions[15] = 70;

    g.color = 0; g.dice_result = 6; g.movePiece(0);
    g.color = 1; g.dice_result = 6; g.movePiece(1);
    g.color = 2; g.dice_result = 6; g.movePiece(2);
    g.color = 3; g.dice_result = 6; g.movePiece(3);

    std::vector<int> expected = {53, -1, -1, -1, -1, 58, -1, -1, -1, -1, 63, -1, -1, -1, -1, 68};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_beyond_goal", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_star_around_51()
{
    g.reset();

    g.player_positions[10] = 48;
    g.color = 2;
    g.dice_result = 2;
    g.movePiece(2);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_star_around_51", expected, actual);

    return content_is_equal(expected, actual);
}

//Don't send yourself home on a globe
bool test_game::test_move_piece_two_on_globe()
{
    g.reset();

    g.player_positions[4] = 8;
    g.player_positions[5] = 6;

    g.color = 1;
    g.dice_result = 2;
    g.movePiece(1);

    std::vector<int> expected = {-1, -1, -1, -1, 8, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_two_on_globe", expected, actual);

    return content_is_equal(expected, actual);
}

//Don't send yourself home just because you already have two pieces on the square
bool test_game::test_move_piece_three_on_square()
{
    g.reset();

    g.player_positions[13] = 46;
    g.player_positions[14] = 46;
    g.player_positions[15] = 45;

    g.color = 3;
    g.dice_result = 1;
    g.movePiece(3);

    std::vector<int> expected = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 46, 46, 46};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_three_on_square", expected, actual);

    return content_is_equal(expected, actual);
}

//Should be safe to land on a globe
bool test_game::test_move_piece_move_to_globe()
{
    g.reset();

    g.player_positions[3] = 19;
    g.color = 0;
    g.dice_result = 2;
    g.movePiece(3);

    std::vector<int> expected = {-1, -1, -1, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_move_to_globe", expected, actual);

    return content_is_equal(expected, actual);
}

bool test_game::test_move_piece_move_enter_goal_stretch()
{
    g.reset();

    g.player_positions[0] = 48; g.player_positions[4] = 9; g.player_positions[8] = 22; g.player_positions[12] = 35;

    g.color = 0; g.dice_result = 3; g.movePiece(0);
    g.color = 1; g.dice_result = 3; g.movePiece(0);
    g.color = 2; g.dice_result = 3; g.movePiece(0);
    g.color = 3; g.dice_result = 3; g.movePiece(0);

    std::vector<int> expected = {52, -1, -1, -1, 57, -1, -1, -1, 62, -1, -1, -1, 67, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_move_enter_goal_stretch", expected, actual);

    return content_is_equal(expected, actual);
}

//If you have a legal move, you should not be allowed to pass your turn
bool test_game::test_move_piece_enforce_legal_move()
{
    g.reset();

    g.player_positions[4] = 99; //rel. piece 0, already in goal
    g.player_positions[6] = 20; //rel. piece 2, on board ready to move

    g.color = 1;
    g.dice_result= 2;
    g.movePiece(3); //trying an invalid move

    std::vector<int> expected = {-1, -1, -1, -1, 99, -1, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<int> actual = g.player_positions;
    //print("test_move_piece_enforce_legal_move", expected, actual);


    return content_is_equal(expected, actual);
}


bool test_game::content_is_equal(const std::vector<int>& expected, const std::vector<int>& actual)
{
    if(expected.size() != actual.size())
        return false;

    for(int i = 0; i < actual.size(); i++)
        if(actual[i] != expected[i])
            return false;

    return true;
}

bool test_game::content_is_not_equal(const std::vector<int>& expected, const std::vector<int>& actual)
{
    return ! content_is_equal(expected, actual);
}

void test_game::print(const std::string& message, const std::vector<int>& expected, const std::vector<int>& actual)
{
    std::cout << std::endl << message << std::endl;

    std::cout << "Expected: ";
    for(int i = 0; i < expected.size(); i++)
        std::cout << expected[i] << ", ";
    std::cout << std::endl;

    std::cout << "Actual:   ";
    for(int i = 0; i < actual.size(); i++)
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


