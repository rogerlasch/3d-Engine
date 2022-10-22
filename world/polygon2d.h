

#ifndef POLYGON2D_H
#define POLYGON2D_H

namespace gpp
{

class polygon2d
{
public:
std::vector<vector3d> vertices;
polygon();
polygon(const polygon& p);
polygon& operator=(const polygon& p);
virtual ~polygon();
void setVertices(const std::vector<vector3d>& vertices);
vector3d getCenter()const;
std::vector<vector3d>& getVertices();
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
virtual bool isColiding(shape2d* sh);
};
}
#endif
