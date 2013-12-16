#include "infer.h"
#include <math.h>
#include <iostream>
using namespace std;

#define random(x) (rand()%x)


void infer::init(strunt Vnode* node, long node_num, vec_long & FiveSet, long person_ID_1, long person_ID_2)
{
    capacity = FiveSet.size();
    vec_long::iterator it = find(FiveSet.begin(), FiveSet.end(), person_ID_1);
    person1 = it - FiveSet.begin();

    it = find(FiveSet.begin(), FiveSet.end(), person_ID_2);
    person2 = it - FiveSet.begin();

    vec_table_fixed = vec_2dimension(capacity, vector<bool>(capacity));
    for(i=0;i<capacity;i++)
        for(j=i+1;j<capacity;j++)
        {
        vec_table_fixed[i][j]=TOOL.check_if_true(node, FiveSet, i , j);
        vec_table_fixed[j][i]=vec_table_fixed[i][j];
        if(vec_table_fixed[i][j]==true)
        {count_true++;count_true++;}
        }

    // each person knows himself
    for(i=0;i<capacity;i++)
        vec_table_fixed[i][i]=true;
}

bool infer::JustifyTrue_firstRule(long threeVariable, long group, vec_2dimension&table_iter, bool reverse = false)
{
    vector<int> person3 = parse3(threeVariable);
    vec_long& group_A = group_member[group];
//---------------------check if the three variables exist in the group
//---------------------group_A needs to be sorted before
    for(int i=0 ; i<3; i++)
    {
        if(!binary_search (group_A.begin(), group_A.end(), person3[i]) )
            return 1;
    }
    vec_long& X_Friends = (node+person3[0])->friends;
    if(!binary_search (X_Friends.begin(), X_Friends.end(), person3[1]) )
        return 1;

    vec_long& Y_Friends = (node+person3[1])->friends;
    if(!binary_search (Y_Friends.begin(), Y_Friends.end(), person3[2]) )
        return 1;

//--------------------------when the rule's proabbility is less than 0.5, we need to reverse it.
    if(reverse == false)
    {
        if(!binary_search (X_Friends.begin(), X_Friends.end(), person3[2]) )
            return 0;
        else return 1;
    }
    else
    {
        if(!binary_search (X_Friends.begin(), X_Friends.end(), person3[2]) )
            return 1;
        else return 0;
    }

}

void infer::LinkInfer()
{

    if(vec_table_fixed[person1][person2]==true){cout<<"they already know each other"<<endl; return;}

//------------------MC-SAT---------------------------
    parse par;
    vec_2dimension table_iter;
    vec_long combination_threeVariable;
    vec_long combination_twoVariable;


    vector<class clause> ClauseSet;

    par.comb(capacity,3,combination_threeVariable);
    par.comb(capacity,2,combination_twoVariable);

    long num_formulas_three = combination_threeVariable.size();
    long num_formulas_two = combination_twoVariable.size();

    for(int i=0; i<num_samples; i++)
    {
        srand((int)time(NULL));
        long_reverse.clear();
        TOOL.assignRandomValue(vec_table_fixed,table_iter);
        //-----------------groupID needs to be resorted
        for(long group =0; group < group_member.size();group++)
        {
//------------------------select first rule clause---------
            for(long j =0; j<num_formulas_three; j++)
            {
                if(test[group][1] == 0) continue;
                if(test[group][1] > 0.5)
                {
                    if(JustifyTrue_first(combination_threeVariable[j], group, table_iter))
                    if(double(random(100))/100 < log_2(test[group][1]))
                    {
                        clause temp(3,group,1,false);
                        vector<int> vec_three_Variable = parse3(combination_threeVariable[j]);
                        temp.predicate = vec_three_Variable;
                        ClauseSet.push_back(temp);
                    }

                }
                else{
                    if(JustifyTrue_first(combination_threeVariable[j], group, table_iter, true))
                    if(double(random(100))/100 < log_2(1-test[group][1]))
                    {
                        clause temp(3,group,1,true);
                        vector<int> vec_three_Variable = parse3(combination_threeVariable[j]);
                        temp.predicate = vec_three_Variable;
                        ClauseSet.push_back(temp);
                    }


                }
            }

//------------------------select second rule clause---------

            for(long j =0; j<num_formulas_two; j++)
            {
                if(test[group][2] == 0) continue;
                if(test[group][2] > 0.5)
                {
                    if(JustifyTrue_second(combination_twoVariable[j], group, table_iter))
                    if(double(random(100))/100 < log_2(test[group][2]))
                    {
                        clause temp(2,group,2,false);
                        vector<int> vec_two_Variable = parse2(combination_twoVariable[j]);
                        temp.predicate = vec_two_Variable;
                        ClauseSet.push_back(temp);
                    }

                }
                else{
                    if(JustifyTrue_second(combination_twoVariable[j], group, table_iter, true))
                    if(double(random(100))/100 < log_2(1-test[group][2]))
                    {
                        clause temp(2,group,2,true);
                        vector<int> vec_two_Variable = parse2(combination_twoVariable[j]);
                        temp.predicate = vec_two_Variable;
                        ClauseSet.push_back(temp);
                    }


                }
            }
//------------------------select third rule clause---------

            for(long j =0; j<num_formulas_two; j++)
            {
                if(test[group][3] == 0) continue;
                if(test[group][3] > 0.5)
                {
                    if(JustifyTrue_third(combination_twoVariable[j], group, table_iter))
                    if(double(random(100))/100 < log_2(test[group][2]))
                    {
                        clause temp(2,group,3,false);
                        vector<int> vec_two_Variable = parse2(combination_twoVariable[j]);
                        temp.predicate = vec_two_Variable;
                        ClauseSet.push_back(temp);
                    }

                }
                else{
                    if(JustifyTrue_third(combination_twoVariable[j], group, table_iter, true))
                    if(double(random(100))/100 < log_2(1-test[group][2]))
                    {
                        clause temp(2,group,3,true);
                        vector<int> vec_two_Variable = parse2(combination_twoVariable[j]);
                        temp.predicate = vec_two_Variable;
                        ClauseSet.push_back(temp);
                    }


                }
            }






        }


//        WalkSAT(ClauseSet,vec_table_fixed,table_iter);

        if(table_iter[person1][person2] == true)
        {

        }

    }
}
