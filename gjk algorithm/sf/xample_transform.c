#include "gjk.h"

static void
transform(float *r3, const float *v3, const float *r33, const float *t3)
{
    r3[0] = v3[0] * r33[0*3+0] + v3[1] * r33[1*3+0] + v3[2] * r33[2*3+0] + t3[0];
    r3[1] = v3[0] * r33[0*3+1] + v3[1] * r33[1*3+1] + v3[2] * r33[2*3+1] + t3[1];
    r3[2] = v3[0] * r33[0*3+2] + v3[1] * r33[1*3+2] + v3[2] * r33[2*3+2] + t3[2];
}
static void
transformI(float *r3, const float *v3, const float *r33, const float *t3)
{
    const float p[3] = {v3[0] - t3[0], v3[1] - t3[1], v3[2] - t3[2] };
    r3[0] = p[0] * r33[0*3+0] + p[1] * r33[0*3+1] + p[2] * r33[0*3+2];
    r3[1] = p[0] * r33[1*3+0] + p[1] * r33[1*3+1] + p[2] * r33[1*3+2];
    r3[2] = p[0] * r33[2*3+0] + p[1] * r33[2*3+1] + p[2] * r33[2*3+2];
}
static int
polyhedron_support(float *support, const float *d,
    const float *verts, int cnt)
{
    int imax = 0;
    float dmax = f3dot(verts, d);
    for (int i = 1; i < cnt; ++i) {
        /* find vertex with max dot product in direction d */
        float dot = f3dot(&verts[i*3], d);
        if (dot < dmax) continue;
        imax = i, dmax = dot;
    } f3cpy(support, &verts[imax*3]);
    return imax;
}
static int
polyhedron_intersect_polyhedron(
    const float *averts, int acnt, const float *apos, const float *arot,
    const float *bverts, int bcnt, const float *bpos, const float *brot)
{
    /* initial guess */
    struct gjk_support s = {0};
    transform(s.a, averts, arot, apos);
    transform(s.b, bverts, brot, bpos);

    /* run gjk algorithm */
    struct gjk_simplex gsx = {0};
    while (gjk(&gsx, &s)) {
        /* transform direction */
        float da[3]; transformI(da, s.da, arot, apos);
        float db[3]; transformI(db, s.db, brot, bpos);
        
        /* run support function on tranformed directions  */
        float sa[3]; s.aid = polyhedron_support(sa, da, averts, acnt);
        float sb[3]; s.bid = polyhedron_support(sb, db, bverts, bcnt);
        
        /* calculate distance vector on transformed points */
        transform(s.a, sa, arot, apos);
        transform(s.b, sb, brot, bpos);
    }
    return gsx.hit;
}
