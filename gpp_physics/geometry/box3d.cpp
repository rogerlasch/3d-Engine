

#include"geometry.h"

using namespace std;

namespace gpp
{
box3d::box3d(const vector3d& min, const vector3d& measures):
GeometricShape(GTYPE_BOX)
{
this->min=min;
this->measures=measures;
}

box3d::box3d(const box3d& b):
GeometricShape(GTYPE_BOX)
{
*this=b;
}

box3d& box3d::operator=(const box3d& b)
{
this->min=b.min;
this->measures=b.measures;
return *this;
}

box3d:: ~box3d()
{
}

 vector3d box3d::GetCenter()const
{
return min+(measures*0.5f);
}

void box3d::GetVertices(vector<vector3d>& vertices)
{
vertices.resize(0);
vertices.push_back(min);
vertices.push_back({min.x, min.y+measures.y, min.z});
vertices.push_back({min.x+measures.x, min.y+measures.y, min.z});
vertices.push_back({min.x+measures.x, min.y, min.z});
vertices.push_back({min.x, min.y, min.z+measures.z});
vertices.push_back({min.x, min.y+measures.y, min.z+measures.z});
vertices.push_back({min.x+measures.x, min.y+measures.y, min.z+measures.z});
vertices.push_back({min.x+measures.x, min.y, min.z+measures.z});
}

   bool box3d::Contains(const vector3d& point) const
{
vector3d max=(min+measures);
return ((min.x<=point.x)&&(max.x>=point.x)&&(min.y<=point.y)&&(max.y>=point.y)&&(min.z<=point.z)&&(max.z>=point.z));
}

   void box3d::Translate(const vector3d& translation)
{
min+=translation;
}

   void box3d::Scale(float scale)
{
  vector3d center = (min + measures) * 0.5f;
  min = center + (min - center) * scale;
measures=measures*scale;
}

 void box3d::Scale(const vector3d& scale)
{
  vector3d center = (min + measures) * 0.5f;
  min = center + (min - center) * scale;
measures=measures*scale;
}

void box3d::Rotate(const quaternion& orientation)
{
  // rotate each vertex of the box
vector<vector3d> vertices;
  GetVertices(vertices);
  for (int i = 0; i < 8; i++)
  {
    vertices[i] = quaternion_vector_rotate(orientation, vertices[i]);
  }

  // Update the min and measures properties based on the rotated vertices
  min = vertices[0];
  measures = vector3d(0, 0, 0);
  for (int i = 1; i < 8; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (vertices[i][j] < min[j])
      {
        min[j] = vertices[i][j];
      }
      if (vertices[i][j] > min[j] + measures[j])
      {
        measures[j] = vertices[i][j] - min[j];
      }
    }
  }
}

   vector3d box3d::ClosestPointOnSurface(const vector3d& point) const
{
  vector3d closestPoint = point;

  // Verifica se o ponto está fora da caixa no eixo X
  if (point.x < min.x) {
    closestPoint.x = min.x;
  } else if (point.x > min.x + measures.x) {
    closestPoint.x = min.x + measures.x;
  }

  // Verifica se o ponto está fora da caixa no eixo Y
  if (point.y < min.y) {
    closestPoint.y = min.y;
  } else if (point.y > min.y + measures.y) {
    closestPoint.y = min.y + measures.y;
  }

  // Verifica se o ponto está fora da caixa no eixo Z
  if (point.z < min.z) {
    closestPoint.z = min.z;
  } else if (point.z > min.z + measures.z) {
    closestPoint.z = min.z + measures.z;
  }
  return closestPoint;
}

   float box3d::Volume() const
{
return (measures.x*measures.y)*measures.z;
}

matrix3x3 box3d::GetInertiaTensor(float mass) const
{
    const float one_twelfth = 1.0f / 12.0f;
    matrix3x3 inertiaTensor;
    float x = measures.x, y = measures.y, z = measures.z;
    vector3d v;
    v.x = mass * (y*y + z*z) * one_twelfth;
    v.y = mass * (x*x + z*z) * one_twelfth;
    v.z = mass * (x*x + y*y) * one_twelfth;
    inertiaTensor.setDiagonal(v);
    return inertiaTensor;
}
}
