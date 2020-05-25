#include "q_table.h"
#include <iostream>
#include <exception>

#include <iostream>
#include <fstream>

using namespace std;

Q_Table::Q_Table()
{ 
    for(int i = 0; i < STATE_COUNT; i++)
        q_table[i] = 0.25;

    q_table[WON_STATE] = 1.0;
}

int Q_Table::get_index(int in_goal, int pip, int unprotected, int threat, int best_opp, int second_opp)
{
    //Input validation
    if(in_goal < 0     || in_goal > 3)      throw std::exception();
    if(pip < 0         || pip > 9 )         throw std::exception();
    if(unprotected < 0 || unprotected > 7)  throw std::exception();
    if(threat < 0      || threat > 3)       throw std::exception();
    if(best_opp < 0    || best_opp > 8)     throw std::exception();
    if(second_opp < 0  || second_opp > 7)   throw std::exception();

    //Q Index calculation
    int index = in_goal * GOAL_OFFSET;
    index += pip * PIP_OFFSET;
    index += unprotected * UNPROTECTED_OFFSET;
    index += threat * THREAT_OFFSET;
    index += best_opp * BEST_OPP_OFFSET;
    index += second_opp;

    //Output validation
    if(index < 0 || index >= STATE_COUNT)   throw std::exception();

    return index;
}

double Q_Table::get_value(int index)
{
    return q_table[index];
}

void Q_Table::set_value(int index, double value)
{
    q_table[index] = value;
}

void Q_Table::save(const std::string &name)
{
    ofstream myfile;
    myfile.open(name);
    if(myfile.is_open() == false)
        throw exception();

    for(int i = 0; i < STATE_COUNT; i++)
        myfile << q_table[i] << "\n";
    myfile << 1.0 << "\n"; //ie. won state

    myfile.close();
}

bool Q_Table::load(const string &name)
{
    ifstream myfile;
    myfile.open(name);
    if(myfile.is_open() == false)
        std::cout << "Could not open file" << std::endl;

    double num = 0.0;
    for(int i = 0; i < STATE_COUNT; i++)
    {
        myfile >> num;
        q_table[i] = num;
    }
    myfile.close();

    if(q_table[WON_STATE] != 1.0)
        throw exception();

    return true;
}

void Q_Table::record_visit(int index)
{
    usage_table[index]++;
    total_visits++;
}

void Q_Table::report_visits()
{
    int one_time = 0;
    int two_times = 0;
    int teen_times = 0;
    for(int i = 0; i < STATE_COUNT; i++)
    {
        if(usage_table[i] >= 1)  one_time++;
        if(usage_table[i] >= 2)  two_times++;
        if(usage_table[i] >= 10) teen_times++;
    }

    std::cout << one_time << "; " << two_times << "; " << teen_times << "; " << total_visits << std::endl;
}

void Q_Table::report_usage()
{
    int no_visit = 0;

    int unused_goal[4] = {0};
    int unused_pip[10] = {0};
    int unused_unprotected[8] = {0};
    int unused_threat[4] = {0};
    int unused_best_opp[9] = {0};
    int unused_second_opp[8] = {0};

    int goal_pip[4][10] = {0};

    for(int i = 0; i < STATE_COUNT; i++)
    {
        if(usage_table[i] == 0)
        {
            no_visit++;
            int state = i;
            int in_goal = state / GOAL_OFFSET;
            state -= in_goal * GOAL_OFFSET;
            int pip = state / PIP_OFFSET;
            state -= pip * PIP_OFFSET;
            int unprotected = state / UNPROTECTED_OFFSET;
            state -= unprotected * UNPROTECTED_OFFSET;
            int threat = state / THREAT_OFFSET;
            state -= threat * THREAT_OFFSET;
            int best_opp = state / BEST_OPP_OFFSET;
            state -= best_opp * BEST_OPP_OFFSET;
            int second_opp = state;

            unused_goal[in_goal]++;
            unused_pip[pip]++;
            unused_unprotected[unprotected]++;
            unused_threat[threat]++;
            unused_best_opp[best_opp]++;
            unused_second_opp[second_opp]++;

            goal_pip[in_goal][pip]++;
        }
    }

    std::cout << no_visit << " states was never visited" << std::endl;

    std::cout << "unused_goal ";
    for(int i = 0; i < 4; i++)
        std::cout << unused_goal[i] << ", ";
    std::cout << std::endl;

    std::cout << "unused_pip ";
    for(int i = 0; i < 10; i++)
        std::cout << unused_pip[i] << ", ";
    std::cout << std::endl;

    std::cout << "unused_unprotected ";
    for(int i = 0; i < 8; i++)
        std::cout << unused_unprotected[i] << ", ";
    std::cout << std::endl;

    std::cout << "unused_threat ";
    for(int i = 0; i < 4; i++)
        std::cout << unused_threat[i] << ", ";
    std::cout << std::endl;

    std::cout << "unused_best_opp ";
    for(int i = 0; i < 9; i++)
        std::cout << unused_best_opp[i] << ", ";
    std::cout << std::endl;

    std::cout << "unused_second_opp ";
    for(int i = 0; i < 8; i++)
        std::cout << unused_second_opp[i] << ", ";
    std::cout << std::endl;

//    std::cout << "goal_pip: " << std::endl;
//    for(int i = 0; i < 4; i++)
//    {
//        for(int j = 0; j < 10; j++)
//            std::cout << goal_pip[i][j] << ", ";
//            std::cout << std::endl;
//    }
//    std::cout << std::endl;
 }

void Q_Table::copy_values_to(Q_Table& destination)
{
    for(int i = 0; i < STATE_COUNT; i++)
        destination.q_table[i] = this->q_table[i];

    destination.q_table[WON_STATE] = 1.0;
}

double Q_Table::difference(Q_Table& other_table)
{
    double dif = 0;

    for(int i = 0; i < STATE_COUNT; i++)
        dif += abs(other_table.q_table[i] - this->q_table[i]);

    return dif;
}










