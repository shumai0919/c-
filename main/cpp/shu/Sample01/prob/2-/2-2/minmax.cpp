#include "minmax.h"

int MinMax::max(int n1,int n2,int n3){
    int max=n1;
    if(max<n2){
        max=n2;
    }
    if(max<n3){
        max=n3;
    }
    return max;
}

int MinMax::min(int n1,int n2,int n3){
    int min=n1;
    if(min>n2){
        min=n2;
    }
    if(min>n3){
        min=n3;
    }
    return min;
}


