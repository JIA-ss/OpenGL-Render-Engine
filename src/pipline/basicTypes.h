#ifndef _BASIC_TYPES_
#define _BASIC_TYPES_

class Vector2
{
public:
    float x,y;
    float& operator[](int i);
};

class Vector3
{
public:
    float x,y,z;
    float& operator[](int i);
};

class Color
{
public:
    float r,g,b,a;
    float& operator[](int i);
};

#endif