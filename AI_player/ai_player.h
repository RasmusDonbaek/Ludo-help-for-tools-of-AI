#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "iplayer.h"
#include "q_table.h"
#include "random"

class AI_player : public iplayer
{
private:
    Q_Table* q_table;
    int post_move_position[16];

    double alpha = 0.10;
    int history[400];
    int move_count = 0;

    bool do_learning = true;

    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;



public:
    AI_player(Q_Table& table);
    AI_player(Q_Table& table, double learning_rate);

    void debug_set_q_table(int index, double value);
    double debug_get_q_table(int index);
    void learning_on();
    void learning_off();
    void set_learning_rate(double value);

    void post_game_analysis(bool you_won);

private:
    int make_decision();
    void post_move_analysis();

    void calc_post_move_position(int move_piece);
    int get_q_index() const;

    //Helping functions for update_post_move_position
    void move_start(int piece);
    void send_them_home(int square);
    int count_opponents(int square) const;
    bool is_globe(int square) const;

    //Helping functions for get_q_index
    int count_own_pieces_in_goal() const;
    int calc_own_pip_count() const;
    int calc_opponent_pips(int opponent) const;
    int calc_unprotected_pips() const;
    int calc_pips_under_attack()  const;
    int calc_best_opp_level(int own_pip_count, int opp_pip_count) const;
    int get_pip_divider(int in_goal)  const;
    int get_unprotected_divider(int in_goal) const;
    int get_lowest(int a, int b, int c) const;
    int get_middle(int a, int b, int c) const;
    int pips_to_goal(int square) const;
    bool is_protected(int square) const;
    bool is_globe_danger(int square) const;
    bool has_piece_home(int opponent) const;

    //Helper for both functions
    int  is_star(int square) const;
};

#endif // AI_PLAYER_H
