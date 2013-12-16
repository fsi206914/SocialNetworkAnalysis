/*-----------------------------------------
Author: Liang Tang
Email:  liang@auburn.edu
Web:

Copyright(c) Liang Tang 2012
-----------------------------------------*/

#ifndef clause_defined
#define clause_defined

#include <vector>
#include <map>
#include <string>

using namespace std;
typedef std::vector<long> vec_long;
typedef std::map<long, vector<long> > map_long;
struct Vnode
{
    long my_ID;
    vec_long friends;
};
class clause
{
    public:
        clause(){}
        clause(string& a_num_predicates, int  a_group_ID, int  a_rule, int a_need_Reverse):
        num_predicates(a_num_predicates), group_ID(a_group_ID), rule(a_rule), need_Reverse(a_need_Reverse)
        {};
        ~clause();

    public:
        int num_predicates;
        vec_long predicate;
        int group_ID;
        int rule;
        bool need_Reverse;
};


#endif
