/*-----------------------------------------
Author: Liang Tang
Email:  liang@auburn.edu
Web:

Copyright(c) Liang Tang 2012
-----------------------------------------*/

#ifndef socialNetwork_defined
#define socialNetwork_defined
#define Lenovo

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
class socialNetwork
{
    public:
        socialNetwork(){}
        socialNetwork(string& a_filename_group, string&  a_file_node, string&  a_file_write):
        filename_group(a_filename_group), file_node(a_file_node), file_write(a_file_write), count_node(0)
        {};
        ~socialNetwork();
        void init();
        void analyzeNodeFile();
        void analyzeGroupFile();
        void calProb(vector<vector<double> >& );
        void calProb_2(vector<vector<double> >& );
        void calProb_3(vector<vector<double> >& );

    public:
        string filename_group;
        string file_node;
        string file_write;
        long count_node;
        long *nodeMap;
        struct Vnode *node;
        map_long group_member;

    private:
        long jishu;
        long iter;

};


#endif
