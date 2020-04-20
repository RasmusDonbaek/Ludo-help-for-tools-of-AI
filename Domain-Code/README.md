# Ludo C++ Domain Code

The C++ Ludo code on Black Board contained multiple bugs.<br/>
Those bugs has been corrected in the code in this folder, the software design has also been improved.<br/>
Once the square indexes of the absolute and relative position are understood the GUI is no longer needed and only slows down the program, thus the GUI and the QT (slot/signal) dependencies have been removed. The two .png files shows the indexes of the squares in absolute and relative positions.<br/><br/>

The main.cpp file, shows how to create players and have them play a game of Ludo.<br/><br/>

To create your own Ludo AI players you have to create a new class and let it inherit the player interface from the 'iplayer' class just like the player_random class does. You can then implement your own custom AI logic in the 'make_decision' function.<br/><br/>

Enjoy
