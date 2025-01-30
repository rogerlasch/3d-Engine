

#ifndef GJK_H
#define GJK_H

namespace gpp{

class GJK{
private:
    vector3d v;
    vector3d b, c, d;
    unsigned int n; //simplex size
    const int MAX_ITERATIONS = 64;

public:
GJK()=default;
    bool isColliding(const GeometricShape* shape1, const GeometricShape* shape2, CollisionInfo* info=NULL);
private:
    vector3d support(const GeometricShape* shape1, const GeometricShape* shape2, const vector3d& v);
    bool update(const vector3d& a);
    vector3d tripleProduct(const vector3d& ab, const vector3d& c);
    bool checkOneFaceAC(const vector3d& abc, const vector3d& ac, const vector3d& ao);
    bool checkOneFaceAB(const vector3d& abc, const vector3d& ab, const vector3d& ao);
    bool checkTwoFaces(vector3d& abc, vector3d& acd, vector3d& ac, vector3d& ab, vector3d& ad, const vector3d& ao);
};
}
#endif
