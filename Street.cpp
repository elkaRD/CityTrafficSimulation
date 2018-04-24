#include "Street.h"

Cross::Cross(Vec3 position)
{
    pos = position;
}

Street::Street(Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;
}
