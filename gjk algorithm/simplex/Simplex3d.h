


#ifndef SIMPLEX3D_H
#define SIMPLEX3D_H

#include<vector>
#include<gpp_physics/gpp_physics.h>
using namespace gpp;

class Simplex3d {
public:
    std::vector<vector3d> vertices;
    Simplex3d();
Simplex3d(const std::vector<vector3d>& vertices);
    Simplex3d(const Simplex3d& sp);
    Simplex3d& operator=(const std::vector<vector3d>& vertices);
    Simplex3d& operator=(const Simplex3d& sp);
    void reset();
    vector3d getVertex(uint32 index) const;
    void setVertex(uint32 index, const vector3d& vertex);
    void addVertex(const vector3d& pt);
    bool containsPoint(const vector3d& pt) const;
    vector3d getSupportPoint(const vector3d& dir) const;
    vector3d getDirectionToOrigi() const;
private:
    // Métodos auxiliares para o algoritmo GJK
    vector3d computeClosestPointToOrigin() const;
    void reduceToTriangle(vector3d& a, vector3d& b, vector3d& c) const;
    void reduceToLine(vector3d& a, vector3d& b) const;
};
#endif
