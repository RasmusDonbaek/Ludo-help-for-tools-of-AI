#ifndef DICE
#define DICE

#include "random"

class dice
{
    friend class test_game; //to allow direct access in the unit tests

private:
    int dice_result = 0;

    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

public:
    dice()
    {
        std::random_device rd;
        generator = std::mt19937(rd());
        distribution = std::uniform_int_distribution<int>(1,6);
    }

    void roll()   { dice_result = distribution(generator); }
    int getValue() { return dice_result; }

private:
    void set(int value) {dice_result = value; } //used in the unit tests

};

#endif // DICE

