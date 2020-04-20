# Unittest the C++ Ludo code from Black Board
The C++ Ludo code on Black Board contains game play bugs and bad code design.<br/><br/>
<strong>The bugs are:</strong>
<ol>
<li>The game::reset() function, does not reset the game.
<li>Incorrect absolute to relative transformation of a piece on absolute square 51
<li>Incorrect absolute to relative transformation of pieces on the goal stretch
<li>There is one of the opponents pieces which can't be send home.
<li>You send your own piece home if it lands on a globe square where you already have a piece.
<li>You send your own piece home if you land a third piece on the same square.
<li>You can avoid making a move even if you have legal move options.
</ol>
<strong>To see this for yourself:</strong>
<ol>
<li>Download the test_game.h and test_game.cpp files from this repository and add them to your project</li>
<li>Add #include "test_game.h" in main.cpp</li>
<li>In game.h changed form this
		<blockquote>
			class game : public QThread<br/> 
		    	{
		</blockquote>
	   into this instead
		<blockquote>
			class test_game;<br/> 
		    	class game : public QThread<br/> 
		    	{<br/> 
    			friend class test_game;
		</blockquote></li>
	<li>In main.cpp just before g.start(); add
		<blockquote>
		    test_game test;<br/> 
		    test.run_all_tests();
		</blockquote></li>
	<li>In game.cpp game::reset() change this:
		<blockquote>
		    for(auto i : player_positions){<br/> 
	        	i = -1; }<br/>
		</blockquote>
     into this instead
		<blockquote>
        		for(int i = 0; i < player_positions.size(); i++)<br/> 
        			player_positions[i] = -1;
		</blockquote></li>
	<li>Build and run program</li>
	</ol>
Then 6 of the unit tests in test_game.cpp should fail:<br/><br/>
<strong>A quick sanity check</strong> is to have 4 'ludo_player_ramdon' players play many games against each other.<br/> 
If they ecah take turn starting the game they are all expected to win 25% of the time. <br/> 
For me player_0 only wins ~ 18% and player_3 wins ~ 40% of the games, showing a significant bias.<br/> 
