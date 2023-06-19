#ifndef TRIANGLE
#define TRIANGLE

#include <iostream>
#include "point.hpp"
using namespace std;


class Triangle
{
private:
    int indP1;
    int indP2;
    int indP3;

public:
    Triangle(int indP1, int indP2, int indP3);
    int getIndP1();
    int getIndP2();
    int getIndP3();
    string toString();
};

#endif