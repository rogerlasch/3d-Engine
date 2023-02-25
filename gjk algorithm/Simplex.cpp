
#include"Simplex3d.h"

using namespace std;

    Simplex3d::Simplex3d()
{
vertices.clear();
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

    vector3d Simplex3d::getSupportPoint(const vector3d& dir) const
{
return vector3d();
}

    vector3d Simplex3d::getDirectionToOrigi() const
{
return vector3d();
}

    // Métodos auxiliares para o algoritmo GJK
    vector3d Simplex3d::computeClosestPointToOrigin() const
{
return vector3d();
}

    void Simplex3d::reduceToTriangle(vector3d& a, vector3d& b, vector3d& c) const
{
}

    void Simplex3d::reduceToLine(vector3d& a, vector3d& b) const
{
}
