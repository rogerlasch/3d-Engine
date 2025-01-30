Gilbert–Johnson–Keerthi (GJK) 3D distance algorithm
---------------------------------------------------

The Gilbert–Johnson–Keerthi (GJK) distance algorithm is a method of determining the minimum distance between two convex sets.
The algorithm's stability, speed which operates in near-constant time, and small storage footprint make it popular for realtime collision detection.  

Unlike many other distance algorithms, it has no requirments on geometry data to be stored in any specific format,
but instead relies solely on a support function to iteratively generate closer simplices to the correct answer
using the Minkowski sum (CSO) of two convex shapes.  

GJK algorithms are used incrementally. In this mode, the final simplex from a previous solution is used as the initial guess 
in the next iteration. If the positions in the new frame are close to those in the old frame, the algorithm will converge
in one or two iterations.

![gjk0](https://user-images.githubusercontent.com/8057201/36906721-f58bd592-1e36-11e8-9329-6a23a6131916.png)
![gjk1](https://user-images.githubusercontent.com/8057201/36906732-ff8cd44c-1e36-11e8-9d91-eb72440ad97c.png)

Features
------------

- GJK 3D distance algorithm implementation useful for collision detection
- No dependencies on other libraries (including standard library)
- Easy to embed (single .h and .c file)
- Unabstracted iterative API. Does not require any specific primitives to be passed. Instead just relies
  on provided support data structure.
- Easy to create debug functionality by storing each simplex iteration inside an array. Allows to run algorithm
  and draw output in single steps.
- No specific math datastructures to convert to and from. Instead everything is just float arrays

Usage
---------

1. Fill `gjk_support` struct with initial vertex positions `a` and `b` one for each polyhedron.
2. Calculate distance vector between points `a` and `b`.
3. Create zeroed out `gjk_simplex` struct.
4. Optionally set maximum number of iterations to run. If not set will be set to default value.
5. Call function `gjk` in a loop until it returns `0` and pass filled out `gjk_support` and distance vector.
6. Call support function for first polyhedron with from `gjk` returned negative direction vector.
7. Store vertex furthest along the negative direction vector in first polyhedron in `gjk_support`.
8. Store vertex index furthest along the negative direction vector in first polyhedron into `a` in `gjk_support`.
9. Call support function for second polyhedron with returned director vector returned from `gjk`.
10. Store vertex furthest along the direction vector in second polyhedron into `b` inside `gjk_support`.
11. Store vertex index furthest along the direction vector in second polyhedron into `gjk_support`.
12. Calculate distance vector between points `a` and `b`.
12. Run next iteration of 'gjk' until it returns '0'.
13. Check `gjk_simplex` if we have a collision by checking variable `hit` is non-zero.
14. Optionally call `gjk_analyze` to calculate closest points and distance between polyhedrons.
15. Optionally for quadratic polyhedrons like spheres `gjk_quad` calculates correct closest point and distance.

Compilation
-----------

Just copy and paste the single `.h` and `.c` into your project and compile.
This project also features examples showcasing how to use this implementation.  

Examples
-----------

1.  The first example `xample_simple.c` showcases a simple use cases of collision detection between a
    polyhedron and a capsule. It is made out of two support functions one for the
    polyhedron and one for the capsule as well as a function to check for a collision.  
    
    The collision funtion itself runs the GJK algorithm to calculate the distance between the
    polhedron and the capsule internal line representation. However since we want to check the
    capsule and not just the line representation it also checks the distance between both polytopes
    and only notifies of a collision if the distance is smaller or equal the capsule radius.  

2.  The second example `xample_transform.c` implements a polyhedron to polyhedron collision detection check
    and extends the algorithm showcased in the first example by adding transformation for both polytopes
    with translation and rotation in form of a 3x3 rotation matrix. It is made out of a single support function
    for polyhedrons as well as function to calculate the transformation and inverse transformation of a point.  
    
    The collision function is similar to the first example with one obvious difference: transformations. Instead
    of transforming all vertexes of a polyhedron only the currently processed vertexes are transformed.This is
    more performant but also requires the direction passed to the support function to be transformed as well.

3.  The third example `xample_xdebug.c` showcases how to create debug visualization. One interesting
    property of having a iterative API with fixed upper bound is that each iteration output can be safed and
    used for debug drawing visualization without having to rely on command buffers or coroutines to draw
    information. The example contains three function. One for calculating the support point of polyhedron,
    one function to calculate the simplex of each iteration and finally a function to draw a specific iteration.

4.  The final example `xample_xmov.c` combines the second example with moving objects by adding two additional arguments
    for object translation. Adding the concept of movement to GJK is relatively straight forward. For collision we just
    combine each object before and after applying the translation. However there is a small optimization. If we consider our
    translation `t` in context of our current direction `d`. Only if both point in the same direction we need to consider
    the translated object (Real time Collision Detection Page.: 409).

Support Functions
-----------

- Support functions are the core of the algorithm and main abstraction over convex polytopes
- Need to calculate both vertex position and index furthest along a direction. 
- Polytropes like polyhedrons and AABBs only require calculating max dot product between all points and the 
  direction (implemented by a single support function taking a number of vertexes, looping and calculating the max dot product) 
- Quadric shaped collision volumes like spheres and capsules rely on the distance measured of capsule internal line or sphere center for collision testing

The reason why support functions are not part of the GJK algorithm implemenation is abstraction. One of the interesting
properties of GJK is that it does not depend on any polytope abstraction implementation. Therefore it is possible
for example to transform (scale, rotate, move, shear, ...) any object by just transforming the support vertexes
without modifing the GJK algorithm. Which is both more performant and grants more control in total.

Implementation notes
------------------------

- Main termination criteria is a duplicated simplex vertex and _not_ a directional check.
- Caseys implementation of using planes to check for the correct voroni region is not numerical safe.
- Caseys optimizations of only taking some of the voronoi regions into account produces wrong results in some cases.
- Instead of planes this implementation uses barycentric coordinates, areas and volumes for voronoi region determination.
- In each iterations all voronoi regions are checked to be as numerical safe as possible
- While the duplication check is enough for 2D it will hit the maximum iteration in 3D. To fix that
  this implementation additionally validates that distance between polytopes over each iteration is getting smaller.
  
Debugging
-------------

Due to the iterative nature of this API it is possible to store each simplex iteration inside an array with size of
the maximum number of iterations (either set directly or `GJK_MAX_ITERATIONS`). Each iteration step can be passed
to `gjk_analyze` to generate information like currently proccessed points, current distance and simplex shape.
All information can be used to draw debug information without having to store draw commands or do coroutine style
debug drawing.
  
References
--------------

- [Erin Cattos presentation on 2D GJK](https://box2d.org/files/ErinCatto_GJK_GDC2010.pdf)
- [Randy Gauls 3D GJK implementation]( https://bitbucket.org/rgaul/lm/src/806fe4427db32f98e514c646b51f99ce1cfc5c14/src/lm/collision/query/lmDistance.cpp?at=default&fileviewer=file-view-default)
- [Casey Muratoris video lecture](https://mollyrocket.com/849)
- Christer Ericson Real Time Collision Detection

Special Thanks
------------------

- Randy Gaul (@RandyPGaul)
- Erin Catto (@erin_catto)
- Per Vognsen (@pervognsen)
- Casey Muratori (@cmuratori)
