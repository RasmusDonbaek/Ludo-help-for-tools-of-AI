#include <iostream>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"

using namespace std;

int main()
{
    //Run the unit tests
    test_game tester;
    tester.run_all_tests();

    //Create players
    player_random player_0;
    player_random player_1;
    player_random player_2;
    player_random player_3;

    //Play a game of Ludo
    game g(&player_0, &player_1, &player_2, &player_3);
    g.play_game();
    cout << "Player " << g.get_winner() << " won the game!" << endl << endl;

    //Play many games of Ludo
    int wins[] = {0, 0, 0, 0};
    for(int i = 0; i < 1000; i++)
    {
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        wins[g.get_winner()]++;
    }
    for(int i = 0; i < 4; i++)
        cout << "Player " << i << " won " << wins[i] << " games." << endl;


    cout << "End of main" << endl;
    return 0;
}


