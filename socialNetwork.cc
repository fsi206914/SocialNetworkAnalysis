#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "socialNetwork.h"

#ifdef Lenovo
#define Num_Node 80000
#define Num_ID 100000
#define Num_Friends 400
#endif

#ifndef Lenovo
#define Num_Node 80000000
#define Num_ID 100000000
#define Num_Friends 400000
#endif

using namespace std;

const bool SNdebug = false;
long total = 0; long succeed = 0;

char *remove_newline(char *s)
{
    int len = strlen(s);

    if (len > 0 && s[len-1] == '\n')  // if there's a newline
        s[len-1] = '\0';          // truncate the string

    return s;
}


void socialNetwork::init()
{
    nodeMap = (long *)malloc(sizeof(long)*Num_ID);
    memset(nodeMap, 0, (sizeof(long)*Num_ID));

    node = (struct Vnode *)malloc(sizeof(Vnode) * Num_Node);
    memset(node, 0, (sizeof(Vnode)*Num_Node));

}

void socialNetwork::analyzeNodeFile()
{
    ifstream ifs(file_node.c_str());
    long *p=nodeMap;
    char large_input[Num_Friends];
    jishu = 0, iter = 1;
    while(ifs.getline(large_input, Num_Friends))
    {
        jishu++;
        if(jishu == 1000 * iter)
        {
            cout<<"current round = "<<jishu<<endl;
            iter++;
        }
        if(!large_input[0]) break;
        remove_newline(large_input);
        char * pch = strtok(large_input," #");
        long i=0;
        long first = 0;
        string currentNum = string(pch);
        long temp_num = atol(currentNum.c_str());
        if(*(p+temp_num)==0) continue;
        first = *(p+temp_num); pch = strtok(NULL, " #");i++;
        while(pch != NULL)
        {
            string currentNum = string(pch);
            long following = atol(currentNum.c_str());
            if(*(p+following)>0)
            {
            (node+first)->friends.push_back( *(p+following) );
            (node+ *(p+following))->friends.push_back( first );
            }
            pch = strtok(NULL, " #");i++;
        }
    }
    ifs.close();
}

void socialNetwork::analyzeGroupFile()
{
   FILE *fp;
   long *p=nodeMap;
    if((fp=fopen(filename_group.c_str(),"r"))==NULL)
        printf("cannot write file!\n");
    vector<string> originaldecrpt(10);
    char input[100];

    while(fgets(input, 100, fp))
    {
        if(!input[0]) break;
        remove_newline(input);
        string temp = input;
        if(temp[temp.size()-1] == '#') continue;
        char * pch = strtok(input,"#");
        int i=0;
        while(pch != NULL)
        {
            originaldecrpt[i] = string(pch);
            pch = strtok(NULL, "#");i++;
        }
        string::size_type found = originaldecrpt[4].find('|');
        string::size_type j = 0;
        long MemberID = atol(originaldecrpt[0].c_str());

        if(*(p+MemberID)==0)//如果这里没数
        {
        count_node++;
        *(p+MemberID)=count_node;
        (node+count_node)->my_ID = count_node;
        }

        MemberID = *(nodeMap+MemberID);
        if(found == std::string::npos )
        {
            long groupID = atol(originaldecrpt[4].c_str());
            if(group_member.count(groupID) <= 0) // first time when create a group
            {
                vector<long> temp; temp.push_back(MemberID);
                group_member.insert(pair<long, vector<long> >(groupID, temp));
            }
            else
                group_member[groupID].push_back(MemberID);
        }
        else{
            vector<string> multipleGroup;
            multipleGroup.push_back( string(originaldecrpt[4], j, found));
            j = found+1;
            found = originaldecrpt[4].find("|", found+1);
            while(found != std::string::npos)
            {
                 multipleGroup.push_back( string(originaldecrpt[4], j, found-j));
                 j = found+1;
                 found = originaldecrpt[4].find("|", found+1);
            }
            multipleGroup.push_back( string(originaldecrpt[4], j, originaldecrpt[4].size()-j));
            for(unsigned int j=0; j<multipleGroup.size();j++)
            {
                long groupID = atol(multipleGroup[j].c_str());
                if(group_member.count(groupID) <= 0) // first time when create a group
                {
                    vector<long> temp; temp.push_back(MemberID);
                    group_member.insert(pair<long, vector<long> >(groupID, temp));
                }
                else
                    group_member[groupID].push_back(MemberID);
            }
        }
    }
    fclose(fp);
}

void socialNetwork::calProb(vector<vector<double> >&test)
{
    ofstream File_wirte;
    File_wirte.open(file_write.c_str());
    long i; vec_long vec_sort;
    jishu = 0, iter = 1;
    for ( i=1; i<count_node+1; i++) // sort node
    {
        vec_sort = (node+i)->friends;
        sort (vec_sort.begin(), vec_sort.end());
        vec_sort.erase( unique( vec_sort.begin(), vec_sort.end() ), vec_sort.end() );
        (node+i)->friends = vec_sort;
        if(SNdebug)
        {
            cout << (node+i)->my_ID;
            for(unsigned int m =0; m<(node+i)->friends.size();m++)
            {
                cout <<"    "<< (node+i)->friends.operator[](m);
            }
        }
        cout << endl;
    }

    map<long, vector<long> >::iterator it = group_member.begin();
    for(; it!=group_member.end(); ++it)
    {
        vec_sort = it->second;
        sort (vec_sort.begin(), vec_sort.end());
//        vec_sort.erase( unique( vec_sort.begin(), vec_sort.end() ), vec_sort.end() );
        it->second = vec_sort;

        File_wirte<< it->first;
        test[jishu][0] = it->first;
        vec_long group = vec_sort;
        for(unsigned long i=0; i<group.size(); i++)
        {
            long first = group[i];
            vec_long& first_friends = (node+first)->friends;
            for(unsigned long j = i+1; j<group.size(); j++)
            {
                long second = group[j];
                if(binary_search (first_friends.begin(), first_friends.end(), second))
                {
                    vec_long& second_friends = (node+second)->friends;
                    for(unsigned long k = 0; k<group.size(); k++)
                        if(k!=i && k!=j)
                        {
                            long third = group[k];
                            if(binary_search (second_friends.begin(), second_friends.end(), third))
                            {
                                total ++;
                                if(binary_search (first_friends.begin(), first_friends.end(), third))
                                    succeed++;
                            }
                        }
                }
            }
        }
        jishu++;
        if(jishu == 10 * iter)
        {
            cout<<"current round = "<<jishu<<endl;
            iter++;
        }
        double prob;
        double temp1= succeed;
        double temp2= total;
        if(temp2 ==0) prob=0;
        else prob= temp1/temp2;
        File_wirte<<" total: " <<total<<"     succeed: "<<succeed;
        File_wirte<<"  prob: " <<prob<<"  sizeofGroup =="<<group.size()<<endl;
        test[jishu][1] = prob;
        total = 0; succeed = 0;
    }
    File_wirte.close();
}

void socialNetwork::calProb_2(vector<vector<double> >&test)
{
    ofstream File_wirte;
    File_wirte.open("write_2.txt");
    jishu = 0, iter = 1;

    map<long, vector<long> >::iterator it = group_member.begin();
    for(; it!=group_member.end(); ++it)
    {
        File_wirte<< it->first;

        vec_long group = it->second;
        for(unsigned long i=0; i<group.size(); i++)
        {
            long first = group[i];
            vec_long& first_friends = (node+first)->friends;
            for(unsigned long j = i+1; j<group.size(); j++)
            {
                total++;
                long second = group[j];
                if(binary_search (first_friends.begin(), first_friends.end(), second))
                    succeed++;
            }
        }
        jishu++;
        if(jishu == 10 * iter)
        {
            cout<<"current round = "<<jishu<<endl;
            iter++;
        }
        double prob;
        double temp1= succeed;
        double temp2= total;
        if(temp2 ==0) prob=0;
        else prob= temp1/temp2;
        File_wirte<<" total: " <<total<<"     succeed: "<<succeed;
        File_wirte<<"  prob: " <<prob<<"  sizeofGroup =="<<group.size()<<endl;
        test[jishu][2] = prob;
        total = 0; succeed = 0;
    }
    File_wirte.close();
}


void socialNetwork::calProb_3(vector<vector<double> >&test)
{
    ofstream File_wirte;
    File_wirte.open("write_3.txt");
    ofstream Rule_1;
    Rule_1.open("flickr_rule_1.txt");
    ofstream Rule_2;
    Rule_2.open("flickr_rule_2.txt");
    ofstream Rule_3;
    Rule_3.open("flickr_rule_3.txt");

    jishu = 0, iter = 1;
    map<long, vector<long> >::iterator it = group_member.begin();
    for(; it!=group_member.end(); ++it)
    {
        File_wirte<< it->first;

        vec_long group = it->second;
        for(unsigned long i=0; i<group.size(); i++)
        {
            long first = group[i];
            vec_long& first_friends = (node+first)->friends;
            for(unsigned long j = 0; j<first_friends.size(); j++)
                if(first_friends[j]>first)
                {
                total++;
                long second = first_friends[j];
                if(binary_search (group.begin(), group.end(), second))
                    succeed++;
                }
        }
        jishu++;
        if(jishu == 10 * iter)
        {
            cout<<"current round = "<<jishu<<endl;
            iter++;
        }
        double prob;
        double temp1= succeed;
        double temp2= total;
        if(temp2 == 0) prob=0;
        else prob= temp1/temp2;
//        File_wirte<<" total: " <<total<<"     succeed: "<<succeed;
//        File_wirte<<"  prob: " <<prob<<"  sizeofGroup =="<<group.size()<<endl;
        test[jishu][3] = prob;

        File_wirte<<" "<<test[jishu][1]<<" "<<test[jishu][2]<<" "<<test[jishu][3]<<endl;
        Rule_1<<it->first<<"   "<<test[jishu][1]<<endl;
        Rule_2<<it->first<<"   "<<test[jishu][2]<<endl;
        Rule_3<<it->first<<"   "<<test[jishu][3]<<endl;

        total = 0; succeed = 0;
    }
    File_wirte.close();
    Rule_1.close();
    Rule_2.close();
    Rule_3.close();
    vector<double> FirstRule(jishu), SecondRule(jishu), ThirdRule(jishu);
    for(int i=0; i<jishu; i++)
    {
        FirstRule[i] =test[i+1][1];
        SecondRule[i] =test[i+1][2];
        ThirdRule[i] =test[i+1][3];
    }
    sort (FirstRule.begin(), FirstRule.end());
    sort (SecondRule.begin(), SecondRule.end());
    sort (ThirdRule.begin(), ThirdRule.end());

    ofstream graph_rule_1;
    graph_rule_1.open("graph_rule_1.txt");
    ofstream graph_rule_2;
    graph_rule_2.open("graph_rule_2.txt");
    ofstream graph_rule_3;
    graph_rule_3.open("graph_rule_3.txt");

    for(int i=0; i<jishu; i++)
    {
        graph_rule_1<<i+1<<" "<<FirstRule[i]<<endl;
        graph_rule_2<<i+1<<" "<<SecondRule[i]<<endl;
        graph_rule_3<<i+1<<" "<<ThirdRule[i]<<endl;
    }

    graph_rule_1.close();
    graph_rule_2.close();
    graph_rule_3.close();
}



socialNetwork::~socialNetwork()
{
    free(nodeMap);
    free(node);
}
