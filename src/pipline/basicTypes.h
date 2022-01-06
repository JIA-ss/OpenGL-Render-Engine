#ifndef _BASIC_TYPES_
#define _BASIC_TYPES_

class Vector2
{
public:
    float x,y;
};

class Vector3
{
public:
    float x,y,z;
};

class Color
{
public:
    Color() {}
    Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
    float r,g,b,a;
};

#endif