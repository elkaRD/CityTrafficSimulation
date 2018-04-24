#include "GameObject.h"

void GameObject::update()
{

}

Vec3::Vec3()
{

}

Vec3::Vec3(float a, float b, float c)
{
    x=a;
    y=b;
    z=c;
}

/*std::istream& operator >> (std::istream& in, Vec3& right)
{
    //in >> right.x >> right.y >> right.z;
    //in >> right.x;
    int tx;
    in >> tx;
    return in;
}
*/
