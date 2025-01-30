#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace gpp;

class Polyedron{
public:
vector<vector3d> vertices;
decimal radius;
Polyedron(){
vertices.clear();
radius=1.5f;
}
void create(const std::vector<vector3d>& vertices){
this->vertices=vertices;
}
void create(uint32 numSides, decimal height, decimal radius){
this->radius=radius;
vertices.clear();
    // Calcular o ângulo entre cada lado do cilindro
    decimal angleIncrement = 2 * GPP_PI / numSides;
    // Gerar vértices em torno da base do cilindro
    for (int i = 0; i < numSides; ++i) {
        decimal angle = i * angleIncrement;
        vector3d vertex;
        vertex.x = radius * cos(angle);
        vertex.y = radius * sin(angle);
        vertex.z = 0.0f;  // Altura da base
vertices.push_back(vertex);
vertex.z=height;
vertices.push_back(vertex);
    }
}

vector3d getFarthestPointInDirection(const vector3d& dir)const {

uint32 bestIndex=0;
decimal d=numeric_limits<decimal>::min();

for(uint32 i=1; i<vertices.size(); i++){
decimal dd=vertices[i]*dir;
if(d<dd){
d=dd;
bestIndex=i;
}
}

return vertices[bestIndex];
}

string toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Radius: "<<radius<<endl;
for(auto& it : vertices){
ss<<it<<endl;
}
return ss.str();
}

void translate(const vector3d& v){
for(auto& it : vertices){
it+=v;
}
}
};

class GJK {
 public:
vector<vector3d> getSimplex()const{
return {b, c, d};
}
    bool intersect(const GeometricShape& shape1, const GeometricShape& shape2) {
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

    vector3d support(const GeometricShape& shape1, const GeometricShape& shape2, const vector3d& v) {
        vector3d p1 = shape1.getSupportPoint(v);
        vector3d p2 = shape2.getSupportPoint(vector3d::inverse(v));//negate v
        vector3d p3 = p1 - p2;
        return p3;
    }

 private:
    bool update(const vector3d& a) {
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

    vector3d tripleProduct(const vector3d& ab, const vector3d& c) {
        return vector3d::cross(vector3d::cross(ab, c), ab);
    }

    bool checkOneFaceAC(const vector3d& abc, const vector3d& ac, const vector3d& ao) {
        if (vector3d::dot(vector3d::cross(abc, ac), ao) > 0) {
            //origin is in the region of edge ac
            b = vector3d::inverse(ao);//b=a
            v = tripleProduct(ac, ao);
            n = 2;

            return false;
        }
        return true;
    }

    bool checkOneFaceAB(const vector3d& abc, const vector3d& ab, const vector3d& ao) {
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

    bool checkTwoFaces(vector3d& abc, vector3d& acd, vector3d& ac, vector3d& ab, vector3d& ad, const vector3d& ao) {
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

    vector3d v;
    vector3d b, c, d;
    unsigned int n; //simplex size

    const int MAX_ITERATIONS = 64;
};

class EPA {
public:
    struct Edge {
        vector3d normal;
        decimal distance;
        int indexA, indexB;
        Edge(const vector3d& normal, decimal distance, int indexA, int indexB)
            : normal(normal), distance(distance), indexA(indexA), indexB(indexB) {}
    };

    struct CollisionInfo {
        vector3d normal;
        decimal depth;
    };

    CollisionInfo expand(vector<vector3d>& simplex, const GeometricShape& shape1, const GeometricShape& shape2) {
        vector<Edge> edges;

        // Expand the simplex (triangle or tetrahedron) to find the closest face to the origin
        for (int i = 0; i < simplex.size(); ++i) {
            int next = (i + 1) % simplex.size();
            vector3d edge = simplex[next] - simplex[i];
            vector3d normal = vector3d::cross(edge, simplex[i]);
            normal.normalize();
            decimal distance = vector3d::dot(normal, simplex[i]);

            edges.emplace_back(normal, distance, i, next);
        }

        while (true) {
            // Find the edge closest to the origin
            auto closestEdge = min_element(edges.begin(), edges.end(),
                [](const Edge& a, const Edge& b) { return a.distance < b.distance; });

            vector3d support = supportFunction(shape1, shape2, closestEdge->normal);

            decimal distance = vector3d::dot(closestEdge->normal, support);
            if (distance - closestEdge->distance < EPA_TOLERANCE) {
                // Converged, return the collision information
                return {closestEdge->normal, distance};
            }

            // Add new triangle formed by the support point
            int indexA = closestEdge->indexA;
            int indexB = closestEdge->indexB;
            edges.erase(closestEdge);
            edges.emplace_back(closestEdge->normal, distance, indexA, (int32)simplex.size());
            simplex.push_back(support);
        }
    }

private:
    vector3d supportFunction(const GeometricShape& shape1, const GeometricShape& shape2, const vector3d& direction) {
        vector3d point1 = shape1.getSupportPoint(direction);
        vector3d point2 = shape2.getSupportPoint(vector3d::inverse(direction));
        return point1 - point2;
    }

    static constexpr decimal EPA_TOLERANCE = 0.0001f;
};
