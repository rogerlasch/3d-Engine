
#include"collision.h"
#include"GJK.h"

using namespace std;

namespace gpp{

    bool GJK::isColliding(const GeometricShape* shape1, const GeometricShape* shape2, CollisionInfo* info) {
        v = vector3d(1,0,0);//initial vector
        n = 0;//set simplex size 0

        c = support(shape1, shape2, v);

        if (vector3d::vector3d::dot(c, v) < 0) {
            return false;
        }
        v = -c;
        b = support(shape1, shape2, v);

        if (vector3d::vector3d::dot(b, v) < 0) {
            return false;
        }
        v = tripleProduct(c - b, -b);
        n = 2;

        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            vector3d a = support(shape1, shape2, v);
            if (vector3d::vector3d::dot(a, v) < 0) {
                // no intersection
                return false;
            }

            if (update(a)) {
                return true;
            }
        }
        return true;
    }

    vector3d GJK::support(const GeometricShape* shape1, const GeometricShape* shape2, const vector3d& v) {
        vector3d p1 = shape1->getSupportPoint(v);
        vector3d p2 = shape2->getSupportPoint(vector3d::inverse(v));//negate v
        vector3d p3 = p1 - p2;
        return p3;
    }

    bool GJK::update(const vector3d& a) {
        if (n == 2) {
            //handling triangle
            vector3d ao = vector3d::inverse(a);
            vector3d ab = b - a;
            vector3d ac = c - a;

            vector3d abc = vector3d::cross(ab, ac);//normal of triangle abc

            // plane test on edge ab
            vector3d abp = vector3d::cross(ab, abc);//direction vector pointing inside triangle abc from ab
            if (vector3d::vector3d::dot(abp, ao) > 0) {
                //origin lies outside the triangle abc, near the edge ab
                c = b;
                b = a;
                v = tripleProduct(ab, ao);
                return false;
            }

            //plane test on edge ac

            //direction vector pointing inside triangle abc from ac
            //note that different than abp, the result of acp is abc cross ac, while abp is ab cross abc.
            //The order does matter. Based on the right-handed rule, we want the vector pointing inside the triangle.
            vector3d acp = vector3d::cross(abc, ac);
            if (vector3d::dot(acp, ao) > 0) {
                //origin lies outside the triangle abc, near the edge ac
                b = a;
                v = tripleProduct(ac, ao);
                return false;
            }

            // Now the origin is within the triangle abc, either above or below it.
            if (vector3d::dot(abc, ao) > 0) {
                //origin is above the triangle
                d = c;
                c = b;
                b = a;
                v = abc;
            } else {
                //origin is below the triangle
                d = b;
                b = a;
                v = -abc;
            }

            n = 3;

            return false;

        }

        if (n == 3) {
            vector3d ao = vector3d::inverse(a);
            vector3d ab = b - a;
            vector3d ac = c - a;
            vector3d ad = d - a;

            vector3d abc = vector3d::cross(ab, ac);
            vector3d acd = vector3d::cross(ac, ad);
            vector3d adb = vector3d::cross(ad, ab);

            vector3d tmp;

            const int over_abc = 0x1;
            const int over_acd = 0x2;
            const int over_adb = 0x4;

            int plane_tests =
                (vector3d::dot(abc, ao) > 0 ? over_abc : 0) |
                (vector3d::dot(acd, ao) > 0 ? over_acd : 0) |
                (vector3d::dot(adb, ao) > 0 ? over_adb : 0);

            switch(plane_tests) {
            case 0:
                {
                    //inside the tetrahedron
                    return true;
                }
            case over_abc:
                {
                    if (!checkOneFaceAC(abc, ac, ao)) {
                        //in the region of AC
                        return false;
                    }
                    if (!checkOneFaceAB(abc, ab, ao)) {
                        //in the region of AB
                        return false;
                    }

                    //otherwise, in the region of ABC
                    d = c;
                    c = b;
                    b = a;
                    v = abc;
                    n = 3;
                    return false;
                }
            case over_acd:
                {
                    //rotate acd to abc, and perform the same procedure
                    b = c;
                    c = d;

                    ab = ac;
                    ac = ad;

                    abc = acd;

                    if (!checkOneFaceAC(abc, ac, ao)) {
                        //in the region of AC (actually is ad)
                        return false;
                    }
                    if (!checkOneFaceAB(abc, ab, ao)) {
                        //in the region of AB (actually is ac)
                        return false;
                    }

                    //otherwise, in the region of "ABC" (which is actually acd)
                    d = c;
                    c = b;
                    b = a;
                    v = abc;
                    n = 3;
                    return false;

                }
            case over_adb:
                {
                    //rotate adb to abc, and perform the same procedure
                    c = b;
                    b = d;

                    ac = ab;
                    ab = ad;

                    abc = adb;
                    if (!checkOneFaceAC(abc, ac, ao)) {
                        //in the region of "AC" (actually is AB)
                        return false;
                    }
                    if (!checkOneFaceAB(abc, ab, ao)) {
                        //in the region of AB (actually is AD)
                        return false;
                    }

                    //otherwise, in the region of "ABC" (which is actually acd)
                    d = c;
                    c = b;
                    b = a;
                    v = abc;
                    n = 3;
                    return false;
                }
            case over_abc | over_acd:
                {
                    if (!checkTwoFaces(abc, acd, ac, ab, ad, ao)) {
                        if (!checkOneFaceAC(abc, ac, ao)) {
                            //in the region of "AC" (actually is AB)
                            return false;
                        }
                        if (!checkOneFaceAB(abc, ab, ao)) {
                            //in the region of AB (actually is AD)
                            return false;
                        }
                        //otherwise, in the region of "ABC" (which is actually acd)
                        d = c;
                        c = b;
                        b = a;
                        v = abc;
                        n = 3;
                        return false;
                    } else {
                        if (!checkOneFaceAB(abc, ab, ao)) {
                            return false;
                        }
                        d = c;
                        c = b;
                        b = a;
                        v = abc;
                        n = 3;
                        return false;
                    }
                }
            case over_acd | over_adb:
                {
                    //rotate ACD, ADB into ABC, ACD
                    tmp = b;
                    b = c;
                    c = d;
                    d = tmp;
            
                    tmp = ab;
                    ab = ac;
                    ac = ad;
                    ad = tmp;
            
                    abc = acd;
                    acd = adb;
                    if (!checkTwoFaces(abc, acd, ac, ab, ad, ao)) {
                        if (!checkOneFaceAC(abc, ac, ao)) {
                            //in the region of "AC" (actually is AB)
                            return false;
                        }
                        if (!checkOneFaceAB(abc, ab, ao)) {
                            //in the region of AB (actually is AD)
                            return false;
                        }
                        //otherwise, in the region of "ABC" (which is actually acd)
                        d = c;
                        c = b;
                        b = a;
                        v = abc;
                        n = 3;
                        return false;
                    } else {
                        if (!checkOneFaceAB(abc, ab, ao)) {
                            return false;
                        }
                        d = c;
                        c = b;
                        b = a;
                        v = abc;
                        n = 3;
                        return false;
                    }
                }
            case over_adb | over_abc:
                {
                    //rotate ADB, ABC into ABC, ACD
                    tmp = c;
                    c = b;
                    b = d;
                    d = tmp;
            
                    tmp = ac;
                    ac = ab;
                    ab = ad;
                    ad = tmp;
            
                    acd = abc;
                    abc = adb;
            
                    if (!checkTwoFaces(abc, acd, ac, ab, ad, ao)) {
                        if (!checkOneFaceAC(abc, ac, ao)) {
                            //in the region of "AC" (actually is AB)
                            return false;
                        }
                        if (!checkOneFaceAB(abc, ab, ao)) {
                            //in the region of AB (actually is AD)
                            return false;
                        }
                        //otherwise, in the region of "ABC" (which is actually acd)
                        d = c;
                        c = b;
                        b = a;
                        v = abc;
                        n = 3;
                        return false;
                    } else {
                        if (!checkOneFaceAB(abc, ab, ao)) {
                            return false;
                        }
                        d = c;
                        c = b;
                        b = a;
                        v = abc;
                        n = 3;
                        return false;
                    }
                }
            default:
                return true;
            }
        }
        return true;
    }

    vector3d GJK::tripleProduct(const vector3d& ab, const vector3d& c) {
        return vector3d::cross(vector3d::cross(ab, c), ab);
    }

    bool GJK::checkOneFaceAC(const vector3d& abc, const vector3d& ac, const vector3d& ao) {
        if (vector3d::dot(vector3d::cross(abc, ac), ao) > 0) {
            //origin is in the region of edge ac
            b = vector3d::inverse(ao);//b=a
            v = tripleProduct(ac, ao);
            n = 2;

            return false;
        }
        return true;
    }

    bool GJK::checkOneFaceAB(const vector3d& abc, const vector3d& ab, const vector3d& ao) {
        if (vector3d::dot(vector3d::cross(ab, abc), ao) > 0) {
            //origin in the region of edge ab
            c = b;
            b = vector3d::inverse(ao);//b=a
            v = tripleProduct(ab, ao);
            n = 2;

            return false;
        }
        return true;
    }

    bool GJK::checkTwoFaces(vector3d& abc, vector3d& acd, vector3d& ac, vector3d& ab, vector3d& ad, const vector3d& ao) {
        if (vector3d::dot(vector3d::cross(abc, ac), ao) > 0) {
            b = c;
            c = d;
            ab = ac;
            ac = ad;

            abc = acd;
            return false;
        }
        return true;
    }
}
