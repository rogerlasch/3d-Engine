#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

class CollisionInfo
{
public:
vector3d point;
vector3d normal;
float depth;
CollisionInfo()
{
depth=0.0f;
}
};
class Sphere {
public:
    vector3d center;
    double radius;

    Sphere(const vector3d& center, double radius) :
        center(center), radius(radius) {}

    bool collidesWith(const Sphere& other, CollisionInfo& info) const {
        vector3d vecDist =center-other.center;
        double distanceSquared = vector3d::dotProduct(vecDist, vecDist);
        double radiusSum = radius + other.radius;
        double radiusSumSquared = radiusSum * radiusSum;
        if (distanceSquared <= radiusSumSquared) {
            double distance = sqrt(distanceSquared);
vector3d normal = distanceSquared == 0 ? vector3d() : vecDist* (1/distance) * (radius - other.radius) * vecDist;

            double depth = radiusSum - distance;
            info.point = center + vecDist * radius / vecDist;
            info.normal = normal;
            info.depth = depth;
            return true;
        }
        return false;
    }
};

int main() {
    Sphere sphere1(vector3d(5,5,5), 5);
    Sphere sphere2(vector3d(10,5,5), 5);

    CollisionInfo info;
    if (sphere1.collidesWith(sphere2, info)) {
        cout << "Collision detected at point " << info.point.toString() << endl;
        cout << "Collision normal: " << info.normal.toString() << endl;
        cout << "Collision depth: " << info.depth << endl;
    } else {
        cout << "No collision detected." << endl;
    }
    return 0;
}
