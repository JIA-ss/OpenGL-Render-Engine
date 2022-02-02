#include "basicTypes.h"
#include "assert.h"
float& Vector2::operator[](int i)
 {
    switch (i)
    {
        case 0:
            return x;
        case 1:
            return y;
        default:
            assert(false);
    };
}

float& Vector3::operator[](int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        assert(false);
    }
}

float& Color::operator[](int i)
{
    switch (i)
    {
    case 0:
        return r;
    case 1:
        return g;
    case 2:
        return b;
    case 3:
        return a;
    default:
        assert(false);
    }
}