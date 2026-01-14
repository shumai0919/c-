#ifndef _NEWCALC_H_
#define _NEWCALC_H_
#include "fundcalc.h"
using namespace std;

class NewCalc:public FundCalc{
public:
    double mul();
    double div();
};

#endif