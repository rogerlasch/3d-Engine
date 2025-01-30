#include "gjk.h"

static int
line_support(float *support, const float *d,
    const float *a, const float *b)
{
    int i = 0;
    if (f3dot(a, d) < f3dot(b, d)) {
        f3cpy(support, b); i = 1;
    } else f3cpy(support, a);
    return i;
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
polyhedron_intersect_capsule(const float *verts, int cnt,
    const float *ca, const float *cb, float cr)
{
    /* initial guess */
    struct gjk_support s = {0};
    f3cpy(s.a, verts);
    f3cpy(s.b, ca);

    /* run gjk algorithm */
    struct gjk_simplex gsx = {0};
    while (gjk(&gsx, &s)) {
        s.aid = polyhedron_support(s.a, s.da, verts, cnt);
        s.bid = line_support(s.b, s.db, ca, cb);
    }
    /* check distance between closest points */
    struct gjk_result res;
    gjk_analyze(&res, &gsx);
    gjk_quad(&res, 0, cr);
    return res.hit;
}