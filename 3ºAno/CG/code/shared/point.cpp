#include "point.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

using namespace std;

Point::Point()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}


Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float Point::getX()
{
    return this->x; 
}

float Point::getY() 
{
    return this->y;
}

float Point::getZ()
{ 
    return this->z; 
}

void Point::setX(float x) {
    this->x = x;
}

void Point::setY(float y) {
    this->y = y;
}

void Point::setZ(float z) {
    this->z = z;
}

void Point::setPoint(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point Point::crossProduct(Point v1, Point v2) {
    Point p;
    p.x = v1.getY()*v2.getZ() - v1.getZ()*v2.getY();
    p.y = v1.getZ()*v2.getX() - v1.getX()*v2.getZ();
    p.z = v1.getX()*v2.getY() - v1.getY()*v2.getX();
    return p;
}

float Point::getSize(){
    return (float) abs(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
}

void Point::normalize(){
    float size = this->getSize();
    this->x = this->x/size;
    this->y = this->y/size;
    this->z = this->z/size;
}

Point Point::scale(float factor){
    Point p(this->x * factor, this->y*factor, this->z*factor);
    return p;
}

void Point::sum(Point p){
    this->x += p.getX();
    this->y += p.getY();
    this->z += p.getZ();
}

string Point::toString()
{
    return to_string(this->x) + " " + to_string(this->y) + " " + to_string(this->z);
}

float Point::distanceTo(Point other) {
    Point vect = Point(this->getX()-other.getX(), this->getY()-other.getY(), this->getZ()-other.getZ());
    return vect.getSize();
}



bool Point::operator==(const Point& other) const {
    return (x == other.x) && (y == other.y) && (z == other.z);
}   


std::size_t Point::HashFunction::operator()(const Point& point) const {
    // Combinação de hashes das coordenadas x, y e z
    std::size_t h1 = std::hash<float>{}(point.x);
    std::size_t h2 = std::hash<float>{}(point.y);
    std::size_t h3 = std::hash<float>{}(point.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
}