#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

typedef std::vector<long> vec_long;
struct Vnode
{
    long my_ID;
    vec_long friends;
};

void HeapAdjust(long *array, long *array1, long i, long Length)
{
    long child,temp,temp1;
    for(temp=*(array+i),temp1=*(array1+i);2*i+1<Length;i=child)
    {
        child = 2*i+1;
        if(child<Length-1 && *(array+child+1)>*(array+child))
            child++;
        if (temp<*(array+child))
            {*(array+i)=*(array+child);
             *(array1+i)=*(array1+child);
            }
        else
            break;
        *(array+child)=temp;
        *(array1+child)=temp1;
    }
}

void Swap(long* a,long* b)
{
    *a=*a^*b;
    *b=*a^*b;
    *a=*a^*b;
}

void GetMax(long *array, long *array1, long Length,long k)
{
    long temp1;
    Swap(& *(array),& *(array+Length-1));
    Swap(& *(array1),& *(array1+Length-1));

    long child,temp;
    long i=0,j=k-1;
    for (temp=*(array), temp1=*(array1); j>0 && 2*i+1<Length; --j,i=child)
    {
        child = 2*i+1;
        if(child<Length-1 && *(array+child+1)>*(array+child))
            child++;
        if (temp<*(array+child))
        {
            *(array+i)=*(array+child);
            *(array1+i)=*(array1+child);
        }
        else
            break;
        *(array+child) = temp;
        *(array1+child) = temp1;
    }

}


class tool {
public:



static long TopK(long *array , long *array1, long Length , long k, vec_long &vec_degreeID)
{
    long i;
    for(i=Length/2-1;i>=0;--i)
        //初始建堆，时间复杂度为O(n)
        HeapAdjust(array, array1, i,Length);

    long j=Length;
    long k_backbone;
    //k次循环，每次循环的复杂度最多为k次交换，复杂度为o(k^2)
    for(i=k;i>0;--i,--j)
    {
//        printf( "%ld 's degree is %ld\r\n", *array1, *array/2);
        k_backbone=*array;
        vec_degreeID.push_back(*array1);
        GetMax(array,array1, j,i);
    }
    return k_backbone/2;
}

bool check_if_true(struct Vnode *node, vec_long &bigFive, int person1, int person2)
{
     long person1_ID = bigFive[person1];
     long person2_ID = bigFive[person2];
     vec_long *vec_check = &((node+person1_ID)->vec_test);
     if(binary_search (vec_check->begin(), vec_check->end(), person2_ID))
         return true;
     else
         return false;
}
double log_2(double a)
{
    return log(a)/log(2);
}

};
#endif /* TOOL_H
 */
