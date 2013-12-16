/*-----------------------------------------
Author: Liang Tang
Email:  liang@auburn.edu
Web:

Copyright(c) Liang Tang 2012
-----------------------------------------*/

#ifndef infer_defined
#define infer_defined

#include <vector>
#include <map>
#include <string>
#include "tool.h"
#define num_samples 1000

using namespace std;
typedef std::vector<long> vec_long;
typedef std::map<long, vector<long> > map_long;
typedef std::vector<vector<bool> > vec_2dimension;
typedef std::map<long, int > map_int;

struct Vnode
{
    long my_ID;
    vec_long friends;
};
class tool;
class infer
{
    public:
        infer(){}
        infer::infer(map_long &socialMap):
        group_member(socialMap),count_true(0)
        {
            parse PAR;
        };

        ~infer();
        void init(strunt Vnode* node, long node_num, vec_long & FiveSet, long person_ID_1, long person_ID_2);


    public:
        map_long group_member;
        vec_2dimension vec_table_fixed;

    private:
        long jishu;
        long iter;
        long count_true;
        long person1;
        long person2;
        long capacity;
        parse PAR;

};


#endif
