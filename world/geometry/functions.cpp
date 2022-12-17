

#include <limits>
#include<vector>
#include"geometry.h"
#include"functions.h"

using namespace std;

namespace gpp
{

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

void ClosestPtPointSegment(const vector3d& c, const vector3d& a, const vector3d& b, float &t, vector3d &d)
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
                  d = ((a + t) * ab);
            }
      }
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

void calculateBoundingBox(const std::vector<vector3d>& points, AABB& b)
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
b.min=m1;
b.max=m2;
}
}
