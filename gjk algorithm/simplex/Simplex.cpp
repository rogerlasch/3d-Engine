
#include"Simplex3d.h"

using namespace std;

    Simplex3d::Simplex3d()
{
vertices.clear();
}

Simplex3d::Simplex3d(const vector<vector3d>& vertices)
{
*this=vertices;
}

Simplex3d& Simplex3d::operator=(const vector<vector3d>& vertices)
{
this->vertices=vertices;
return *this;
}
Simplex3d::    Simplex3d(const Simplex3d& sp)
{
*this=sp;
}

    Simplex3d& Simplex3d::operator=(const Simplex3d& sp)
{
this->vertices=sp.vertices;
return *this;
}

    void Simplex3d::reset()
{
vertices.clear();
}

    vector3d Simplex3d::getVertex(uint32 index) const
{
return vertices[index];
}

    void Simplex3d::setVertex(uint32 index, const vector3d& pt)
{
vertices[index]=pt;
}

    void Simplex3d::addVertex(const vector3d& pt)
{
}

bool Simplex3d::containsPoint(const vector3d& pt) const {
    // find the closest vertex to the point
    double minDist = std::numeric_limits<double>::max();
    int closestVertex = -1;
    for (int i = 0; i < 4; i++) {
        double dist = (pt - vertices[i]).length();
        if (dist < minDist) {
            minDist = dist;
            closestVertex = i;
        }
    }

    // calculate vectors from the closest vertex to each other vertex
    vector<vector3d> edgeVectors(3);
    int idx = 0;
    for (int i = 0; i < 4; i++) {
        if (i == closestVertex) continue;
        edgeVectors[idx] = vertices[i] - vertices[closestVertex];
        idx++;
    }

    // calculate the product of the first two vectors with the third vector
    double mixedProduct = vector3d::dotProduct(edgeVectors[0], vector3d::crossProduct(edgeVectors[1], edgeVectors[2]));

    // check the sign of the mixed product
    if (mixedProduct < 0) {
        return false;
    } else {
        return true;
    }
}

/*
bool Simplex3d::containsPoint(const vector3d& pt) const {
    // calculate vectors from the point to each vertex
    vector<vector3d> edgeVectors(4);
    for (int i = 0; i < 4; i++) {
        edgeVectors[i] = vertices[i] - pt;
    }

    // calculate the product of the first three vectors with the fourth vector
double mixedProduct =vector3d::dotProduct(edgeVectors[0], vector3d::crossProduct(edgeVectors[1], vector3d::crossProduct(edgeVectors[2],  edgeVectors[3])));
    // check the sign of the mixed product
    if (mixedProduct < 0) {
        return false;
    } else {
        return true;
    }
}
*/
    vector3d Simplex3d::getSupportPoint(const vector3d& dir) const
{
return vector3d();
}

vector3d Simplex3d::getDirectionToOrigin() const {
    double minDist = std::numeric_limits<double>::max();
    int closestVertex = -1;

    // find the closest vertex to the origin
    for (int i = 0; i < 4; i++) {
        double dist = vertices[i].length();
        if (dist < minDist) {
            minDist = dist;
            closestVertex = i;
        }
    }

    // calculate the direction from the origin to the closest vertex
    vector3d dir = vector3d::inverse(vertices[closestVertex]);
    return dir.normalize();
}

    // Métodos auxiliares para o algoritmo GJK
vector3d Simplex3d::computeClosestPointToOrigin() const {
    // Initialize the closest point to the first vertex
    vector3d closestPoint = vertices[0];

    // Check distances from all simplex vertices to the origin
    double minDistanceSquared = closestPoint.lengthSquared();
    for (int i = 1; i < 4; i++) {
        double distSquared = vertices[i].lengthSquared();
        if (distSquared < minDistanceSquared) {
            closestPoint = vertices[i];
            minDistanceSquared = distSquared;
        }
    }

    // Check distances from all simplex edges to the origin
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            vector3d edgeVector = vertices[j] - vertices[i];
            vector3d pointOnEdge = vertices[i] + edgeVector * ((-1 * vertices[i].dotProduct(edgeVector)) / edgeVector.lengthSquared());
            double distSquared = pointOnEdge.lengthSquared();
            if (distSquared < minDistanceSquared) {
                closestPoint = pointOnEdge;
                minDistanceSquared = distSquared;
            }
        }
    }

    return closestPoint;
}

void Simplex3d::reduceToTriangle(vector3d& a, vector3d& b, vector3d& c) const {
    // Encontra o índice do vértice mais distante da origem
    int furthestVertex = -1;
    double maxDist = 0.0;
    for (int i = 0; i < 4; i++) {
        double dist = vertices[i].length();
        if (dist > maxDist) {
            maxDist = dist;
            furthestVertex = i;
        }
    }
    
    // Remove o vértice mais distante do simplex
    vertices[furthestVertex] = vertices[3];
    vertices.pop_back();

    // Calcula o ponto mais próximo da origem com o simplex de três vértices
    vector3d closestPoint = computeClosestPointToOrigin();

    // Atribui os três vértices restantes aos parâmetros passados por referência
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];
}

void Simplex3d::reduceToLine(vector3d& a, vector3d& b) const {
    // find the point farthest from the origin
    int farthestVertex = 0;
    double maxDist = (vertices[0] - vector3d(0, 0, 0)).lengthSquared();
    for (int i = 1; i < 3; i++) {
        double dist = (vertices[i] - vector3d(0, 0, 0)).lengthSquared();
        if (dist > maxDist) {
            farthestVertex = i;
            maxDist = dist;
        }
    }

    // set a to the point farthest from the origin
    a = vertices[farthestVertex];

    // set b to the closest point on the line formed by the other two vertices
    vector3d ab = vertices[(farthestVertex + 1) % 3] - a;
    vector3d ac = vertices[(farthestVertex + 2) % 3] - a;
    double t = -vector3d::dotProduct(ab, a) / ab.lengthSquared();
    b = a + ab * t;
}
