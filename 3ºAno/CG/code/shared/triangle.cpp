#include<iostream>
#include <string>
using namespace std;

#include "triangle.hpp"
#include "point.hpp"

Triangle::Triangle(int indP1, int indP2, int indP3)
{
    this->indP1 = indP1;
    this->indP2 = indP2;
    this->indP3 = indP3;
}

int Triangle::getIndP1() 
{
    return this->indP1;
};

int Triangle::getIndP2() 
{ 
    return this->indP2;
};

int Triangle::getIndP3() 
{
    return this->indP3; 
};


string Triangle::toString() {
    return to_string(this->indP1) + " " + to_string(this->indP2) + " " + to_string(this->indP3);
}
