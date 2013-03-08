#include "socialNetwork.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


int main()
{
    #ifdef Lenovo
    string a = "test.txt";
    string b = "node.txt";
    string c = "write.txt";
    #endif

    #ifndef Lenovo
    string a = "uni-nodeproperties-anonymized";
    string b = "uni-socialgraph-anonymized";
    string c = "write.txt";
    #endif

    vector<vector<double> >test(40000, vector<double>(4));
    for(int i=0;i<40000;i++)
        for(int j=0;j<4;j++)
            test[i][j] = 0;
    cout<<"------------------before constructor-------"<<endl;
    socialNetwork SN = socialNetwork(a,b,c);
    cout<<"------------------init-------------------- "<<endl;
    SN.init();
    cout<<"------------------analyzeNodeFile--------- "<<endl;
    SN.analyzeGroupFile();
    SN.analyzeNodeFile();
    SN.calProb(test);
    SN.calProb_2(test);
    SN.calProb_3(test);

    return 0;
}
