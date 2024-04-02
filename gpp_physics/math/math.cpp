
#include"../common.h"
#include"math.h"
#include<gpp/debug_system.h>

using namespace std;

namespace gpp{
decimal degrees_to_radians(decimal f)
{
profiler_snap();
return (f*GPP_PI)/180.0f;
}

decimal radians_to_degrees(decimal r)
{
profiler_snap();
return (r*180)/GPP_PI;
}

decimal gpp_clamp(decimal n, decimal min, decimal max) {
profiler_snap();
      if (n < min) return min;
      if (n > max) return max;
      return n;
}

// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
decimal ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,                                              decimal &s, decimal &t, vector3d& c1, vector3d& c2)
{
profiler_snap();
      vector3d d1 = q1 - p1; // Direction vector of segment S1
      vector3d d2 = q2 - p2; // Direction vector of segment S2
      vector3d r = p1 - p2;
      decimal a = vector3d::dot(d1, d1); // Squared length of segment S1, always nonnegative
      decimal e = vector3d::dot(d2, d2); // Squared length of segment S2, always nonnegative
      decimal f = vector3d::dot(d2, r);
      // Check if either or both segments degenerate into points
      if (a <=GPP_EPSILON && e <=GPP_EPSILON) {
            // Both segments degenerate into points
            s = t = 0.0f;
            c1 = p1;
            c2 = p2;
                return vector3d::dot(c1 - c2, c1 - c2);
          }
          if (a <=GPP_EPSILON) {
                // First segment degenerates into a point
                s = 0.0f;
                t = f / e; // s = 0 => t = (b*s + f) / e = f / e
                t = gpp_clamp(t, 0.0f, 1.0f);
          } else {
                decimal c = vector3d::dot(d1, r);
                if (e <=GPP_EPSILON) {
                      // Second segment degenerates into a point
                      t = 0.0f;
                      s = gpp_clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
                } else {
                      // The general nondegenerate case starts here
                      decimal b = vector3d::dot(d1, d2);
                      decimal denom = a*e-b*b; // Always nonnegative
                      // If segments not parallel, compute closest point on L1 to L2 and
                      // clamp to segment S1. Else pick arbitrary s (here 0)
                      if (denom != 0.0f) {
                            s = gpp_clamp((b*f - c*e) / denom, 0.0f, 1.0f);
                      } else s = 0.0f;
                      // Compute point on L2 closest to S1(s) using
                      // t = vector3d::dot((P1 + D1*s) - P2,D2) / vector3d::dot(D2,D2) = (b*s + f) / e
                      t = (b*s + f) / e;
                      // If t in [0,1] done. Else clamp t, recompute s for the new value
                      // of t using s = vector3d::dot((P2 + D2*t) - P1,D1) / vector3d::dot(D1,D1)= (t*b - c) / a
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
          return vector3d::dot(c1 - c2, c1 - c2);
    }

decimal ClosestPtPointSegment(const vector3d& c, const vector3d& a, const vector3d& b, decimal &t, vector3d &d)
{
profiler_snap();
      vector3d ab = b - a;
      // Project c onto ab, but deferring divide by vector3d::dot(ab, ab)
      t = vector3d::dot(c - a, ab);
      if (t <= 0.0f) {

            // c projects outside the [a,b] interval, on the a side; clamp to a
            t = 0.0f;
            d = a;
      } else {
            decimal denom = vector3d::dot(ab, ab); // Always nonnegative since denom = ||ab||2
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

                 decimal SqDistPointSegment(const vector3d& a, const vector3d& b, const vector3d& c)
                 {
profiler_snap();
                       vector3d ab = b - a, ac = c - a, bc = c - b;
                       decimal e = vector3d::dot(ac, ab);
                       // Handle cases where c projects outside ab
                       if (e <= 0.0f) return vector3d::dot(ac, ac);
                       decimal f = vector3d::dot(ab, ab);
                       if (e >= f) return vector3d::dot(bc, bc);
                       // Handle cases where c projects onto ab
                       return vector3d::dot(ac, ac) - e * e / f;
                 }

void closestPointFromTriangle(const vector3d& p, const vector3d& a, const vector3d& b, const vector3d& c, vector3d& pt){
      // Check if P in vertex region outside A
      vector3d ab = b - a;
      vector3d ac = c - a;
      vector3d ap = p - a;
      decimal d1 = vector3d::dot(ab, ap);
      decimal d2 = vector3d::dot(ac, ap);
      if (d1 <= 0.0f && d2 <= 0.0f){
pt=a;
return;
}
      // Check if P in vertex region outside B
      vector3d bp = p - b;
      decimal d3 = vector3d::dot(ab, bp);
      decimal d4 = vector3d::dot(ac, bp);
      if (d3 >= 0.0f && d4 <= d3){
pt=b;
return;
}
      // Check if P in edge region of AB, if so return projection of P onto AB
      decimal vc = d1*d4 - d3*d2;
      if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
            decimal v = d1 / (d1 - d3);
pt=a + v * ab; // barycentric coordinates (1-v,v,0)
return;
      }

      // Check if P in vertex region outside C
      vector3d cp = p - c;
      decimal d5 = vector3d::dot(ab, cp);
      decimal d6 = vector3d::dot(ac, cp);
      if (d6 >= 0.0f && d5 <= d6){
pt=c;
return;
}

          // Check if P in edge region of AC, if so return projection of P onto AC
          decimal vb = d5*d2 - d1*d6;
          if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
                decimal w = d2 / (d2 - d6);
pt=a + w * ac; // barycentric coordinates (1-w,0,w)
return;
          }
          // Check if P in edge region of BC, if so return projection of P onto BC
          decimal va = d3*d6 - d5*d4;
          if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
                decimal w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
pt=b + w * (c - b); // barycentric coordinates (0,1-w,w)
return;
          }
          // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
          decimal denom = 1.0f / (va + vb + vc);
          decimal v = vb * denom;
          decimal w = vc * denom;
pt=a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
}

void calculateBoundingBox(const std::vector<vector3d>& points, vector3d& min, vector3d& max) {
profiler_snap();
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
profiler_snap();
for(uint32 i=0; i<3; i++){
if((p[i]<min[i])||(p[i]>max[i])) return false;
}
return true;
}

vector3d get_closest_point_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max) {
profiler_snap();
vector3d closestPoint;
                       for (int i = 0; i < 3; i++) {
                             decimal v = pt[i];
                             if (v < min[i]) v = min[i]; // v = max(v, min[i])
                             if (v > max[i]) v = max[i]; // v = min(v, max[i])
closestPoint[i] = v;
                       }
return closestPoint;
}

decimal get_distance_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max) {
profiler_snap();
                       decimal sqDist = 0.0f;
                       for (int i = 0; i < 3; i++) {
                             // For each axis count any excess distance outside box extents
                             decimal v = pt[i];
                             if (v < min[i]) sqDist += (min[i] - v) * (min[i] - v);
                             if (v > max[i]) sqDist += (v - max[i]) * (v - max[i]);
                       }
                       return sqDist;
}

bool point_in_aabb(const vector3d& pt, const vector3d& min, const vector3d& max) {
profiler_snap();
for(uint32 i=0; i<3; i++)
{
if((pt[i]<min[i])||(pt[i]>max[i])) return false;
}
return true;
}

bool aabbOverlap(const vector3d& v1, decimal r1, const vector3d& v2, decimal r2) {
profiler_snap();
    vector3d m1 = v1 - r1;
    vector3d m2 = v1 + r1;
    vector3d m3 = v2 - r2;
    vector3d m4 = v2 + r2;

    // Testa a interseção nas três dimensões
    bool overlapX = (m1.x <= m4.x) && (m2.x >= m3.x);
    bool overlapY = (m1.y <= m4.y) && (m2.y >= m3.y);
    bool overlapZ = (m1.z <= m4.z) && (m2.z >= m3.z);

    // Retorna verdadeiro se houver interseção em todas as dimensões
    return overlapX && overlapY && overlapZ;
}

bool aabbOverlap(const vector3d& center, decimal r, const vector3d& min, const vector3d& max) {
profiler_snap();
    vector3d m1 = center - r;
    vector3d m2 = center + r;

    // Testa a interseção nas três dimensões
    bool overlapX = (m1.x <= max.x) && (m2.x >= min.x);
    bool overlapY = (m1.y <= max.y) && (m2.y >= min.y);
    bool overlapZ = (m1.z <= max.z) && (m2.z >= min.z);

    // Retorna verdadeiro se houver interseção em todas as dimensões
    return overlapX && overlapY && overlapZ;
}

bool aabbOverlap(const vector3d& v1, const vector3d& v2, const vector3d& v3, const vector3d& v4) {
profiler_snap();
    // Testa a interseção nas três dimensões
    bool overlapX = (v1.x <= v4.x) && (v2.x >= v3.x);
    bool overlapY = (v1.y <= v4.y) && (v2.y >= v3.y);
    bool overlapZ = (v1.z <= v4.z) && (v2.z >= v3.z);

    // Retorna verdadeiro se houver interseção em todas as dimensões
    return overlapX && overlapY && overlapZ;
}

bool aabbInsideAll(const vector3d& v1, decimal r1, const vector3d& v2, decimal r2) {
profiler_snap();
    vector3d m1 = v1 - r1;
    vector3d m2 = v1 + r1;
    vector3d m3 = v2 - r2;
    vector3d m4 = v2 + r2;

    // Testa a contenção nas três dimensões
    bool insideX = (m1.x >= m3.x) && (m2.x <= m4.x);
    bool insideY = (m1.y >= m3.y) && (m2.y <= m4.y);
    bool insideZ = (m1.z >= m3.z) && (m2.z <= m4.z);

    // Retorna verdadeiro se a AABB da esfera está contida na AABB especificada em todas as dimensões
    return insideX && insideY && insideZ;
}

bool aabbInsideAll(const vector3d& center, decimal r, const vector3d& min, const vector3d& max) {
profiler_snap();
    vector3d m1 = center - r;
    vector3d m2 = center + r;
    // Testa a contenção nas três dimensões
    bool insideX = (m1.x <= min.x) && (m2.x >= max.x);
    bool insideY = (m1.y <= min.y) && (m2.y >= max.y);
    bool insideZ = (m1.z <= min.z) && (m2.z >= max.z);

    // Retorna verdadeiro se a AABB da esfera está contida na AABB especificada em todas as dimensões
    return insideX && insideY && insideZ;
}

bool aabbInsideAll(const vector3d& v1, const vector3d& v2, const vector3d& v3, const vector3d& v4) {
profiler_snap();
    // Testa a contenção nas três dimensões
    bool insideX = (v1.x >= v3.x) && (v2.x <= v4.x);
    bool insideY = (v1.y >= v3.y) && (v2.y <= v4.y);
    bool insideZ = (v1.z >= v3.z) && (v2.z <= v4.z);

    // Retorna verdadeiro se o primeiro AABB está contido no segundo AABB em todas as dimensões
    return insideX && insideY && insideZ;
}
}
