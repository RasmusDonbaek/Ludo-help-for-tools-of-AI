#include "game.h"

game::game()
{
    reset();

    std::random_device rd;
    generator = std::mt19937(rd());
}

game::game(iplayer* p0, iplayer* p1, iplayer* p2, iplayer* p3)
{
    reset();

    players[0] = p0;
    players[1] = p1;
    players[2] = p2;
    players[3] = p3;

    std::random_device rd;
    generator = std::mt19937(rd());
}

void game::set_players(iplayer *p0, iplayer *p1, iplayer *p2, iplayer *p3)
{
    players[0] = p0;
    players[1] = p1;
    players[2] = p2;
    players[3] = p3;
}

void game::reset()
{
    winner = -1;
    color = 3;

    for(int i = 0; i < piece_count; i++)
        position[i] = -1;
}

void game::set_first(int first)
{
    first -= 1;
    if(first < 0)
        first = player_count;

    color = first % player_count;
}

int game::get_winner()
{
    return winner;
}

void game::play_game()
{
    while(has_won() == false)
        next_turn();
}

bool game::has_won()
{
    int begin = color * pieces_per_player;
    int end = begin + pieces_per_player;

    for(int i = begin; i < end; i++)
    {
        if(position[i] != 99)
            return false;
    }

    winner = color;
    return true;
}

void game::next_turn()
{
    color = (color + 1) % 4;

    update_dice();
    update_relative_position();

    int rel_piece = players[color]->make_decision(rel_pos_and_dice);
    move_piece(rel_piece);
}

void game::update_dice()
{
    game_dice.roll();
    rel_pos_and_dice.dice = game_dice.getValue();
}

void game::update_relative_position()
{
    int count = 0;
    int start = color * pieces_per_player;

    for(int i = start; i < piece_count; i++, count++)
        rel_pos_and_dice.position[count] = absolute_to_relative(position[i]);

    for(int i = 0; i < start; i++, count++)
        rel_pos_and_dice.position[count] = absolute_to_relative(position[i]);
}

int game::absolute_to_relative(int square)
{
    if(square == -1) //home square
        return -1;
    if(square == 99) //goal square
        return 99;

    if(square >= 0 && square <= 51) //On a normal outfield square
    {
        square -= 13 * color;

        if(square < 0)
            square += 52;
    }
    else if (square >= 52 && square <= 71) //On a goal stretch square
    {
        square -= (color * 5) + 1;

        if(square < 51)
            square += 20;
    }
    else
    {
        std::cout << "Can't convert to relative square." << std::endl;
        std::cout << "Piece on abs. square: " << square << " not allowed!" << std::endl;
    }

    return square;
}

void game::move_piece(int relative_piece)
{
    if(is_valid_move(relative_piece))
       trusted_move_piece(relative_piece);
    else
        enforce_valid_move(); //you are not allowed to cheat
}

bool game::is_valid_move(int relative_piece)
{
    if(relative_piece < 0 || relative_piece >= pieces_per_player)
        return false;

    int abs_square = position[abs_piece_index(relative_piece)];

    if(abs_square >= 0 && abs_square < 99)
        return true;

    if(abs_square < 0 && game_dice.getValue() == 6)
        return true;

    return false;
}

void game::trusted_move_piece(int relative_piece)
{
    int piece_index = abs_piece_index(relative_piece);
    int abs_square = position[piece_index];

    if(abs_square == -1)
        move_start(piece_index);
    else
    {
        int rel_square = absolute_to_relative(abs_square);
        rel_square += game_dice.getValue();

        if(rel_square == 56 || rel_square == 50)                  // relative square 50 is a star sending you to goal
            position[piece_index] = 99;
        else if (rel_square > 56)                                 // Overshooting the goal square
            position[piece_index] = 113 - rel_square + 5 * color; // Move back + convert to absolute square: 56+56-target+1+5*color.
        else if (rel_square >= 51)                                // On the goal stretch
            position[piece_index] = rel_square + 1 + 5 * color;   // Convert goal stretch to absolute square
        else                                                      // Still on the outfield
        {
            rel_square += is_star(rel_square);                    // Jump if landed on a star

            abs_square = rel_square + 13 * color;                 // Convert back to absolute square
            if(abs_square > 51)                                   // wrap around absolute square 51
                abs_square -= 52;

            int opp = count_opponents(abs_square);
            if(opp == 0)                                          // Free square, place the moving piece here
                position[piece_index] = abs_square;
            else if(opp > 1)                                      // Protected square, send the moving piece home
                position[piece_index] = -1;
            else                                                  // Exactly one opponent
            {
                if(abs_square % 13 == 0 || abs_square % 13 == 8)  // Opponent on globe, send the moving piece home
                    position[piece_index] = -1;
                else                                              // On normal square, send the opponent home
                {
                    send_them_home(abs_square);
                    position[piece_index] = abs_square;
                }
            }
        }
    }
}

void game::enforce_valid_move()
{
    //Count how many legal moves the player has
    int validCount = 0;
    bool validMoves[pieces_per_player];
    for(int i = 0; i < pieces_per_player; i++)
    {
        if(is_valid_move(i))
        {
            validMoves[i] = true;
            validCount++;
        }
        else
            validMoves[i] = false;
    }

    if(validCount == 0)
        return; //Do nothing, the player don't have any legal moves

    //choose and execute a randomly choosen legal move
    distribution = std::uniform_int_distribution<int>(1, validCount);
    int moveNumber = distribution(generator);

    int count = 0;
    for(int i = 0; i < pieces_per_player; i++)
    {
        if(validMoves[i])
            count++;

        if(count == moveNumber)
        {
            trusted_move_piece(i);
            break;
        }
    }
}

int game::abs_piece_index(int relative_piece_index)
{
    return relative_piece_index + color * pieces_per_player;
}

void game::move_start(int abs_piece)
{
    if(game_dice.getValue() == 6 && position[abs_piece] < 0)
    {
        position[abs_piece] = color * 13; //move me to start
        send_them_home(color * 13);       //send pieces home if they are on our start
    }
}

void game::send_them_home(int square)
{
    int first = color * pieces_per_player;
    int last = first + pieces_per_player - 1;

    for(int i = 0; i < piece_count; i++) // loop all pieces
        if(i < first || i > last)        // piece of different color
            if(position[i] == square)    // on the square
                position[i] = -1;        // send it home
}

int game::is_star(int square)
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
    case 50: return 7;

    default: return 0;
    }
}

int game::count_opponents(int square)
{
    int first = color * pieces_per_player;
    int last = first + pieces_per_player - 1;
    int count = 0;

    for(int i = 0; i < piece_count; i++) // loop all pieces
        if(i < first || i > last)        // piece of different color
            if(position[i] == square)    // on the square
                count++;

    return count;
}
