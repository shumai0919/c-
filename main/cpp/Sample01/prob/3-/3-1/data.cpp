#include "data.h"

void CData::init()
{
    number = 0;
    comment = "";
}

void CData::setNumber(int a){
    number=a;
}

void CData::setComment(string a){
    comment=a;
}

int CData::getNumber(){
    return number;
}

string CData::getComment(){
    return comment;
}