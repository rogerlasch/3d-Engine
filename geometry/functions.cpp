

#include <limits>
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
float d=vector3d::get_squared_distance(points[i],point);
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

// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
float ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,  float& s, float& t, vector3d& c1, vector3d& c2)
{
struct clamp
{
static float Clamp(float n, float min, float max) {
      if (n < min) return min;
      if (n > max) return max;
      return n;
}
};
      vector3d d1 = q1 - p1; // Direction vector of segment S1
      vector3d d2 = q2 - p2; // Direction vector of segment S2
      vector3d r = p1 - p2;
      float a = vector3d::dotProduct(d1, d1); // Squared length of segment S1, always nonnegative
      float e =vector3d::dotProduct(d2, d2); // Squared length of segment S2, always nonnegative
      float f = vector3d::dotProduct(d2, r);
      // Check if either or both segments degenerate into points
      if (a <= numeric_limits<float>::epsilon() && e <= numeric_limits<float>::epsilon()) {
            // Both segments degenerate into points
            s = t = 0.0f;
            c1 = p1;
            c2 = p2;
                return vector3d::dotProduct(c1 - c2, c1 - c2);
          }
          if (a <= numeric_limits<float>::epsilon()) {
                // First segment degenerates into a point
                s = 0.0f;
                t = f / e; // s = 0 => t = (b*s + f) / e = f / e
                t = clamp::Clamp(t, 0.0f, 1.0f);
          } else {
                float c = vector3d::dotProduct(d1, r);
                if (e <= numeric_limits<float>::epsilon()) {
                      // Second segment degenerates into a point
                      t = 0.0f;
                      s = clamp::Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
                } else {
                      // The general nondegenerate case starts here
                      float b = vector3d::dotProduct(d1, d2);
                      float denom = a*e-b*b; // Always nonnegative
                      // If segments not parallel, compute closest point on L1 to L2 and
                      // clamp to segment S1. Else pick arbitrary s (here 0)
                      if (denom != 0.0f) {
                            s = clamp::Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
                      } else s = 0.0f;
                      // Compute point on L2 closest to S1(s) using
                      // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
                      t = (b*s + f) / e;
                      // If t in [0,1] done. Else clamp t, recompute s for the new value
                      // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
                      // and clamp s to [0, 1]
                      if (t < 0.0f) {
                            t = 0.0f;
                            s = clamp::Clamp(-c / a, 0.0f, 1.0f);
                      } else if (t > 1.0f) {
                            t = 1.0f;
                            s = clamp::Clamp((b - c) / a, 0.0f, 1.0f);
                      }
                }
          }
s=vector3d::floatClean(s);
t=vector3d::floatClean(t);
          c1 =(p1 + d1)*s;
          c2 = p2 + d2 * t;
          return vector3d::dotProduct(c1 - c2, c1 - c2);
    }
}
