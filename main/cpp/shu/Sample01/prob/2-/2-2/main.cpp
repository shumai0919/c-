#include <iostream>
#include "minmax.h"

using namespace std;

int main(void){
    MinMax m;
    int a=4;
    int b=2;
    int c=7;
    cout << a << "と" << b << "のうち、最大のものは" << m.max(a,b) << endl;
    cout << a << "と" << b << "のうち、最小のものは" << m.min(a,b) << endl;
    return 0;
}