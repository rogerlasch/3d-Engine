

#include<vector>

#include"vector3d.h"
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

void gpp_rotate_points(vector<vector3d>& points, float angle, const vector3d& origin)
{
if(points.size()==0)
{
return;
}
float mcos=cos((angle*GPP_PI)/180.0f);
float msin=sin((angle*GPP_PI)/180.0f);
for(auto& it : points)
{
it.x=(((it.x-origin.x)*mcos)-((origin.y-it.y)*msin)+origin.x);
it.y=(((it.y-origin.y)*mcos)+((it.x-origin.x)*msin))+origin.y;
}
}

void gpp_rotate_single_point(vector3d& point, float angle, const vector3d& origin)
{
float mcos=cos((angle*GPP_PI)/180.0f);
float msin=sin((angle*GPP_PI)/180.0f);
point.x=(((point.x-origin.x)*mcos)-((origin.y-point.y)*msin)+origin.x);
point.y=(((point.y-origin.y)*mcos)+((point.x-origin.x)*msin))+origin.y;
}
}
