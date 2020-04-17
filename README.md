# Ludo-help-for-tools-of-AI
The C++ Ludo code on Black Board contains game play bugs and bad code design.

To make the code build and run:
	1) Rename View.pro to Ludo.pro
	2) In Ludo.pro change Target = View to Target = Ludo
	3) In game.cpp method run() out comment emit close();
	4) In ludo_player_random.cpp add #include <random>

To expose the bugs: 
	1) Download the test_game.h and test_game.cpp files from this repository
	2) Add test_game.h and test_game.cpp to your Ludo.pro file
	3) Add #include "test_game.h" in main.cpp
	4) In game.h changed form this
		    class game : public QThread
		    {
	   into this instead
		    class test_game;
		    class game : public QThread
		    {
    			friend class test_game;
	5) In main.cpp just before g.start(); add
		    test_game test;
		    test.run_all_tests();
  6) In game.cpp game::reset() change this:
		    for(auto i : player_positions){
	        	i = -1; }
     into this instead
        for(int i = 0; i < player_positions.size(); i++)
        		player_positions[i] = -1;
  7) Build and run program

Then 6 of the unit tests in test_game.cpp should fail:
	1) 'test_rel_pos_outfield_special'
      Wrong absolute to relative position transformation of a piece on absolute square 51.
	2) 'test_rel_pos_goal_stretch'
      Wrong absolute to relative position transformation of the pieces on the goal stretch for the players before you.
      That is Player_0's pieces for player_1, Player_0 and Player_1's pieces for player_2,
      Player_0, Player_1 and Player_2's pieces for player_3
	3) 'test_move_piece_send_home'
		  For each player_0, player_1 and player_2, there is one of the opponents pieces you can't send home.
	4) 'test_move_piece_two_on_globe'
		  You send yourself home if you land a second piece on a globe square
	5) 'test_move_piece_three_on_square'
		  You send yourself home if you land a third piece on a normal square
	6) 'test_move_piece_enforce_legal_move'
		  You can avoid making a move even if you have legal move options

A quick sanity check is to have 4 'ludo_player_ramdon' players play many games against each other.
If they ecah take turn starting they are all expected to win 25% of the time. 
For me player_0 only wins ~ 18% and player_3 wins ~ 40% of the games, showing a significant bias.

Code fixes for the bugs and bad software design will be uploaded shortly.
