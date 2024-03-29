
#include"../common.h"
#include"math.h"

using namespace std;

namespace gpp{
float degrees_to_radians(float f)
{
return (f*GPP_PI)/180.0f;
}

float radians_to_degrees(float r)
{
return (r*180)/GPP_PI;
}

float gpp_clamp(float n, float min, float max) {
      if (n < min) return min;
      if (n > max) return max;
      return n;
}

// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
float ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,                                              float &s, float &t, vector3d& c1, vector3d& c2)
{
      vector3d d1 = q1 - p1; // Direction vector of segment S1
      vector3d d2 = q2 - p2; // Direction vector of segment S2
      vector3d r = p1 - p2;
      float a = vector3d::dotProduct(d1, d1); // Squared length of segment S1, always nonnegative
      float e = vector3d::dotProduct(d2, d2); // Squared length of segment S2, always nonnegative
      float f = vector3d::dotProduct(d2, r);
      // Check if either or both segments degenerate into points
      if (a <=GPP_EPSILON && e <=GPP_EPSILON) {
            // Both segments degenerate into points
            s = t = 0.0f;
            c1 = p1;
            c2 = p2;
                return vector3d::dotProduct(c1 - c2, c1 - c2);
          }
          if (a <=GPP_EPSILON) {
                // First segment degenerates into a point
                s = 0.0f;
                t = f / e; // s = 0 => t = (b*s + f) / e = f / e
                t = gpp_clamp(t, 0.0f, 1.0f);
          } else {
                float c = vector3d::dotProduct(d1, r);
                if (e <=GPP_EPSILON) {
                      // Second segment degenerates into a point
                      t = 0.0f;
                      s = gpp_clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
                } else {
                      // The general nondegenerate case starts here
                      float b = vector3d::dotProduct(d1, d2);
                      float denom = a*e-b*b; // Always nonnegative
                      // If segments not parallel, compute closest point on L1 to L2 and
                      // clamp to segment S1. Else pick arbitrary s (here 0)
                      if (denom != 0.0f) {
                            s = gpp_clamp((b*f - c*e) / denom, 0.0f, 1.0f);
                      } else s = 0.0f;
                      // Compute point on L2 closest to S1(s) using
                      // t = vector3d::dotProduct((P1 + D1*s) - P2,D2) / vector3d::dotProduct(D2,D2) = (b*s + f) / e
                      t = (b*s + f) / e;
                      // If t in [0,1] done. Else clamp t, recompute s for the new value
                      // of t using s = vector3d::dotProduct((P2 + D2*t) - P1,D1) / vector3d::dotProduct(D1,D1)= (t*b - c) / a
                      // and clamp s to [0, 1]
                      if (t < 0.0f) {
                            t = 0.0f;
                            s = gpp_clamp(-c / a, 0.0f, 1.0f);
                      } else if (t > 1.0f) {
                            t = 1.0f;
                            s = gpp_clamp((b - c) / a, 0.0f, 1.0f);
                      }
                }
          }
          c1 = p1 + d1 * s;
          c2 = p2 + d2 * t;
          return vector3d::dotProduct(c1 - c2, c1 - c2);
    }

float ClosestPtPointSegment(const vector3d& c, const vector3d& a, const vector3d& b, float &t, vector3d &d)
{
      vector3d ab = b - a;
      // Project c onto ab, but deferring divide by vector3d::dotProduct(ab, ab)
      t = vector3d::dotProduct(c - a, ab);
      if (t <= 0.0f) {

            // c projects outside the [a,b] interval, on the a side; clamp to a
            t = 0.0f;
            d = a;
      } else {
            float denom = vector3d::dotProduct(ab, ab); // Always nonnegative since denom = ||ab||2
            if (t >= denom) {

                  // c projects outside the [a,b] interval, on the b side; clamp to b
                  t = 1.0f;
                  d = b;
            } else {
                  // c projects inside the [a,b] interval; must do deferred divide now
                  t = t / denom;
                  d = a + t * ab;
            }
}
vector3d dist=d-c;
return dist*dist;
}

                 float SqDistPointSegment(const vector3d& a, const vector3d& b, const vector3d& c)
                 {
                       vector3d ab = b - a, ac = c - a, bc = c - b;
                       float e = vector3d::dotProduct(ac, ab);
                       // Handle cases where c projects outside ab
                       if (e <= 0.0f) return vector3d::dotProduct(ac, ac);
                       float f = vector3d::dotProduct(ab, ab);
                       if (e >= f) return vector3d::dotProduct(bc, bc);
                       // Handle cases where c projects onto ab
                       return vector3d::dotProduct(ac, ac) - e * e / f;
                 }

void calculateBoundingBox(const std::vector<vector3d>& points, vector3d& min,
vector3d& max)
{
vector3d m1, m2;
if(points.size()==0)
{
return;
}
m1=points[0];
m2=m1;
for(auto& it : points)
{
if(it.x<m1.x)
{
m1.x=it.x;
}
if(it.y<m1.y)
{
m1.y=it.y;
}
if(it.z<m1.z)
{
m1.z=it.z;
}
if(it.x>m2.x)
{
m2.x=it.x;
}
if(it.y>m2.y)
{
m2.y=it.y;
}
if(it.z>m2.z)
{
m2.z=it.z;
}
}
min=m1;
max=m2;
}

bool point_in_box(const vector3d& p, const vector3d& min, const vector3d& max){
for(uint32 i=0; i<3; i++){
if((p[i]<min[i])||(p[i]>max[i])) return false;
}
return true;
}
}
