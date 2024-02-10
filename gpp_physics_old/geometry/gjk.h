


#ifndef GPP_GJK_H
#define GPP_GJK_H

#include<vector>
namespace gpp
{
class GJK
{
public:
static bool gjk(const std::vector<vector3d>& p1, const std::vector<vector3d>& p2, std::vector<vector3d>& simplex);
static vector3d support(const std::vector<vector3d>& poly, const vector3d& dir);
static vector3d findCenterSimplex(const std::vector<vector3d>& simplex);
static bool isCenterInSimplex(const std::vector<vector3d>& simplex, const vector3d& center);
static void removeFarthestPoint(std::vector<vector3d>& poly, const vector3d& dir);
};
}
#endif
