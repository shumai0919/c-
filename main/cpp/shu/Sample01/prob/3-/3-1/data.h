#ifndef _CDATA_H_
#define _CDATA_H_

#include <iostream>
#include <string>

using namespace std;

class CData{
private:
    int number;
    string comment;
public:
    void init();
    void setNumber(int a);
    void setComment(string a);
    int getNumber();
    string getComment();
};

#endif