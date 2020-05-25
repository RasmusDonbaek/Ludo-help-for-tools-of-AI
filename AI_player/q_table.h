#ifndef Q_TABLE_H
#define Q_TABLE_H

#include <string>


class Q_Table
{
public:
    static const int STATE_COUNT = 92160;
    static const int WON_STATE = STATE_COUNT;

private:
    static const int GOAL_OFFSET = 23040;
    static const int PIP_OFFSET = 2304;
    static const int UNPROTECTED_OFFSET = 288;
    static const int THREAT_OFFSET = 72;
    static const int BEST_OPP_OFFSET = 8;
    //no off set for second opponent level

    double q_table[STATE_COUNT + 1];  //+1 --> a dummy state for won game;
    int usage_table[STATE_COUNT + 1] = {0}; //keeping track of which states is being reached
    int total_visits = 0;

public:
    Q_Table();

    int get_index(int in_goal, int pip, int unprotected, int threat, int best_opp, int second_opp);

    double get_value(int index);
    void   set_value(int index, double value);

    void save(const std::string& name);
    bool load(const std::string& name);

    void record_visit(int index);
    void report_visits();
    void report_usage();

    void copy_values_to(Q_Table& destination);
    double difference(Q_Table& other_table);



};

#endif // Q_TABLE_H
