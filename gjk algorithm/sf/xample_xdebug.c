#include "gjk.h"

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
polyhedron_intersect_sphere_debug(
    struct gjk_simplex *gsx,
    const float *verts, int cnt,
    const float *sc, float sr)
{
    /* initial guess */
    int n = 0;
    struct gjk_support s = {0};
    f3cpy(s.a, verts);
    f3cpy(s.b, sc);

    /* run gjk algorithm */
    while (gjk(&gsx[n++], &s)) {
        s.aid = polyhedron_support(s.a, s.da, verts, cnt);
        gsx[n] = gsx[n-1];
    }
    return n;
}
static int
debug_draw_polyhedron_intersect_sphere(int key_frame,
    const float *verts, int cnt,
    const float *sc, float sr)
{
    struct gjk_simplex gsx[GJK_MAX_ITERATIONS] = {{0}};
    int n = polyhedron_intersect_sphere_debug(gsx, verts, cnt, sc, sr);
    key_frame = clamp(0, key_frame, n-1);
    
    struct gjk_result res;
    gjk_analyze(&gsx[key_frame]);
    gjk_quad(&res, 0, sr);
    
    glBox(res.p0, 0.05f, 0.05f, 0.05f);
    glBox(res.p1, 0.05f, 0.05f, 0.05f);
    glLine(res.p0, res.p1);
    return key_frame;
}   
