


#include"geometry.h"
#include"functions.h"

using namespace std;

namespace gpp
{

bool gpp_find_nearest_point(const vector<vector3d>& points, const vector3d& point, vector3d* outv)
{
if((points.size()==0)||(outv==NULL))
{
return false;
}
float mindist=-1;
int index=-1;
for(int i=0; i<static_cast<int>(points.size()); i++)
{
float d=points[i].get_squared_distance(point);
if((mindist<0)||(d<mindist))
{
mindist=d;
index=i;
}
}
if((mindist>-1)&&(index>-1))
{
*outv=points[index];
return true;
}
return false;
}
}
