import math

class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
    
    def __repr__(self):
        return "[x: %.03f, y: %.03f, z: %.03f]" % (self.x, self.y, self.z)
    
    def __eq__(self, other):
        return round(self.x, 3) == round(other.x, 3) and round(self.y, 3) == round(other.y, 3) and round(self.z, 3) == round(other.z, 3)
    
    def __hash__(self):
        return hash((round(self.x, 3), round(self.y, 3), round(self.z, 3)))
    
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)
    
    def __mul__(self, other):
        if isinstance(other, float):
            return Vector(self.x * other, self.y * other, self.z * other)
        return Vector(self.x * other.x, self.y * other.y, self.z * other.z)
    
    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)
    
    def __truediv__(self, other):
        if isinstance(other, float):
            return Vector(self.x / other, self.y / other, self.z / other)
        return Vector(self.x / other.x, self.y / other.y, self.z / other.z)
    
    def __neg__(self):
        return Vector(-self.x, -self.y, -self.z)
    
    def dot(self, other):
        return round(self.x * other.x + self.y * other.y + self.z * other.z, 3)
    
    def cross(self, other):
        return Vector(self.y * other.z - self.z * other.y, self.z * other.x - self.x * other.z, self.x * other.y - self.y * other.x)
    
    def square_length(self):
        return self.dot(self)
    
    def length(self):
        return round(math.sqrt(self.square_length()), 3)
    
    def normalize(self):
        return self/ self.length()
    def print(self):
        print(f"{self.x:.3f}, {self.y:.3f}, {self.z:.3f}")

Vector.ORIGIN = Vector(0.0, 0.0, 0.0)

class Quaternion:
    def __init__(self, vector, scalar):
        self.vector = vector
        self.scalar = scalar
    
    def __mul__(self, other):
        if isinstance(other, Vector):
            conj = Quaternion(-self.vector, self.scalar)
            vec = Quaternion(other, 0.0)
            quat = self * vec * conj
            return quat.vector
        return Quaternion(self.vector.cross(other.vector) + self.vector * other.scalar + other.vector * self.scalar, self.scalar * other.scalar - self.vector.dot(other.vector))
    
    @staticmethod
    def from_axis_angle(axis, angle):
        sin = math.sin(angle / 360.0 * math.pi)
        vec = axis/ axis.length()
        return Quaternion(vec * sin, math.sqrt(1.0 - sin * sin))

class Transform:
    def __init__(self, vec0, vec1, vec2, vec3):
        self.vec0 = vec0
        self.vec1 = vec1
        self.vec2 = vec2
        self.vec3 = vec3
    
    def __add__(self, translation):
        return Transform(self.vec0, self.vec1, self.vec2, self.vec3 + translation)
    
    @staticmethod
    def from_components(translation, rotation, scale):
        return Transform(rotation * Vector(scale.x, 0.0, 0.0), rotation * Vector(0.0, scale.y, 0.0), rotation * Vector(0.0, 0.0, scale.z), rotation * translation)
    def print(self):
        self.vec0.print()
        self.vec1.print()
        self.vec2.print()
        self.vec3.print()
        

class Simplex:
    def __init__(self):
        self.vertices = []
        self.attempts = []
        self.finished = False
    
    def add_vertex(self, vertex):
        if self.finished:
            return True
        # Reject if the last line segment formed with this vertex do not pass the origin.
        if self.vertices and vertex.dot(vertex - self.vertices[-1]) < 0.0:
            return False
        # Reject if the vertex is the origin.
        if vertex == Vector.ORIGIN:
            self.finished = True
            return False
        match len(self.vertices):
            case 2:
                # Reject if the line formed by the other two vertices contains this vertex.
                if vertex.cross(self.vertices[1] - self.vertices[0]) == Vector.ORIGIN:
                    return False
            case 3:
                # Reject if the vertex is coplanar the plane formed by the other three vertices.
                perp = (self.vertices[1] - self.vertices[0]).cross(self.vertices[2] - self.vertices[0])
                if perp.dot(vertex - self.vertices[0]) == 0.0:
                    return False
            case 4:
                self.vertices.clear()
        # Fail if the vertex is duplicate.
        if vertex in self.vertices:
            return False
        self.vertices.append(vertex)
        # Fail if this simplex has already been tried.
        if len(self.vertices) == 4:
            for attempt in self.attempts:
                for vertex in self.vertices:
                    if vertex not in attempt:
                        break
                else:
                    return False
            self.attempts.append(self.vertices.copy())
        return True
    
    def contains_origin(self):
        if self.finished:
            return len(self.vertices) == 4
        if len(self.vertices) == 4:
            # Fail if the origin is in front of any of the faces.
            for face in ((0, 1, 2, 3), (0, 1, 3, 2), (0, 2, 3, 1), (1, 2, 3, 0)):
                # Find the vector perpendicular to the face.
                perp = (self.vertices[face[1]] - self.vertices[face[0]]).cross(self.vertices[face[2]] - self.vertices[face[0]])
                # Make sure that the vector points outwards.
                perp = perp * perp.dot(self.vertices[face[0]] - self.vertices[face[3]])
                # Check whether the origin is in front of the face.
                if perp.dot(self.vertices[face[0]]) < 0.0:
                    break
            else:
                self.finished = True
        else:
            return False
        return self.finished
    
    def suggest_direction(self):
        if self.finished:
            return
        match len(self.vertices):
            case 1:
                # Return the direction to the origin.
                return -self.vertices[0]
            case 2:
                # If the segment crosses the origin, suggest a random perpendicular vector.
                if self.vertices[0].cross(self.vertices[1]) == Vector.ORIGIN:
                    perpx = Vector(1.0, 0.0, 0.0).cross(self.vertices[1])
                    return perpx if perpx != Vector.ORIGIN else Vector(0.0, 1.0, 0.0).cross(self.vertices[1])
                # Return a vector perpendicular to the line segment in the general direction of the origin.
                return (self.vertices[1] - self.vertices[0]).cross(-self.vertices[0]).cross(self.vertices[1] - self.vertices[0])
            case 3:
                # If the the origin is on the same plane as the triangle, return a random vector perpendicular to it.
                perp = (self.vertices[1] - self.vertices[0]).cross(self.vertices[2] - self.vertices[0])
                proj = perp.dot(self.vertices[0])
                if proj == 0.0:
                    return perp
                # Return the vector perpendicular to the triangle in the general direction of the origin.
                return (perp * -proj)

class Triangle:
    def __init__(self, vertex0, vertex1, vertex2):
        self.vertices = (vertex0, vertex1, vertex2)
        perp = (vertex1 - vertex0).cross(vertex2 - vertex0)
        self.normal = perp.normalize()
        self.distance = self.normal.dot(vertex0)
    
    def faces_point(self, point):
        return self.normal.dot(point - self.vertices[0]) > 0.0
    
    @staticmethod
    def from_simplex(simplex):
        tris = []
        for facet in ((0, 1, 2, 3), (1, 0, 3, 2), (2, 3, 0, 1), (3, 2, 1, 0)):
            perp = (simplex.vertices[facet[1]] - simplex.vertices[facet[0]]).cross(simplex.vertices[facet[2]] - simplex.vertices[facet[0]])
            if perp.dot(simplex.vertices[facet[0]] - simplex.vertices[facet[3]]) > 0.0:
                tris.append(Triangle(simplex.vertices[facet[0]], simplex.vertices[facet[1]], simplex.vertices[facet[2]]))
            else:
                tris.append(Triangle(simplex.vertices[facet[2]], simplex.vertices[facet[1]], simplex.vertices[facet[0]]))
        return tris

class Polyhedron:
    def __init__(self, simplex):
        # If one of the vertices of the simplex is the origin, then this is a degenerate case.
        self.finished = Vector.ORIGIN in simplex.vertices
        if self.finished:
            return
        self.triangles = Triangle.from_simplex(simplex)
        # Keep track of the number of references to each vertex of the polyhedron.
        self.vertex_count = {
            simplex.vertices[0]: 3,
            simplex.vertices[1]: 3,
            simplex.vertices[2]: 3,
            simplex.vertices[3]: 3
        }
    
    def add_triangle(self, triangle):
        self.triangles.append(triangle)
        for vert in triangle.vertices:
            ct = self.vertex_count.setdefault(vert, 0)
            self.vertex_count[vert] = ct + 1
    
    def remove_triangle(self, triangle):
        self.triangles.remove(triangle)
        for vert in triangle.vertices:
            ct = self.vertex_count[vert]
            self.vertex_count[vert] = ct - 1
    
    def add_vertex(self, vertex):
        if self.finished:
            return False
        # Terminate if this vertex has already been added.
        if vertex in self.vertex_count.keys():
            self.finished = True
            return False
        # Mark the triangles whose front faces can see the new vertex for deletion to prevent concavities.
        oldtris = []
        for tri in self.triangles:
            if tri.faces_point(vertex):
                oldtris.append(tri)
        # If the new vertex is part of the polyhedron then we have already found the face closest to the origin.
        if not oldtris:
            self.finished = True
            return False
        # Extract unique vectors from the triangles marked for deletion and delete them.
        rem = []
        for tri in oldtris:
            self.remove_triangle(tri)
            for vert in tri.vertices:
                if not vert in rem:
                    rem.append(vert)
        # Filter out the deleted vertices that don't have any other references.
        dangs = [x for x in filter(lambda x: self.vertex_count[x] > 0, rem)]
        # Compute an average vector between the new vertex and the dangling vertices.
        dirs = [x for x in map(lambda x: (x - vertex).normalize(), dangs)]
        avgdir = sum(dirs, Vector.ORIGIN)
        # Compute the horizontal, vertical, and depth axis.
        dp = -avgdir.normalize()
        vt = (avgdir).cross(dirs[0]).normalize()
        hz = avgdir.cross(vt).normalize()
        # Sort the dangling vertices by angle.
        dangdirs = [x for x in map(lambda x: (dangs[x], (dirs[x] - dp * dirs[x].dot(dp)).normalize()), range(len(dangs)))]
        dangdirs = [x for x in sorted(dangdirs, key = lambda x: -math.copysign(x[1].dot(hz) + 1.0, x[1].dot(vt)))]
        # Create the new triangles using the sorted angles to ensure that their normals point outwards.
        for idx in range(len(dangdirs)):
            tri = Triangle(dangdirs[idx][0], dangdirs[(idx + 1) % len(dangdirs)][0], vertex)
            self.add_triangle(tri)
        return True
    
    def suggest_direction(self):
        dir = None
        mindist = math.inf
        for tri in self.triangles:
            dist = tri.distance
            if dist < mindist:
                dir = tri.normal
                mindist = dist
        return dir
    
    def compute_origin_translation(self):
        if not self.finished:
            return
        mindist = math.inf
        norm = None
        for tri in self.triangles:
            dist = tri.distance
            if dist < mindist:
                mindist = dist
                norm = tri.normal
        if norm:
            pen = norm * -mindist
            if pen != Vector.ORIGIN:
                return pen

class Box:
    def __init__(self, transform):
        self.position = transform.vec3
        self.vertices = (
            self.position - transform.vec0 - transform.vec1 - transform.vec2,
            self.position + transform.vec0 - transform.vec1 - transform.vec2,
            self.position - transform.vec0 + transform.vec1 - transform.vec2,
            self.position + transform.vec0 + transform.vec1 - transform.vec2,
            self.position - transform.vec0 - transform.vec1 + transform.vec2,
            self.position + transform.vec0 - transform.vec1 + transform.vec2,
            self.position - transform.vec0 + transform.vec1 + transform.vec2,
            self.position + transform.vec0 + transform.vec1 + transform.vec2
        )
    
    def find_support_point(self, direction):
        if direction == Vector.ORIGIN:
            raise Exception("Direction is null vector")
        # Find the farthest point in a specific direction.
        best = None
        max = -math.inf
        for vertex in self.vertices:
            dot = vertex.dot(direction)
            if dot > max:
                best = vertex
                max = dot
        return best

def shape_intersects_shape(shape0, shape1):
    smpx = Simplex()
    # Generate the first direction vector.
    dir = shape1.position - shape0.position
    if dir == Vector.ORIGIN:
        dir = Vector(0.0, 0.0, 1.0)
    # Check whether the shapes are intersecting using GJK.
    while True:
        point0 = shape0.find_support_point(-dir)
        point1 = shape1.find_support_point(dir)
        diff = point1 - point0
        # If a vertex fails to be added, there's no way to progress further.
        if not smpx.add_vertex(diff):
            return
        # If the simplex contains the origin, then the solids are overlapping.
        if smpx.contains_origin():
            break
        dir = smpx.suggest_direction()
        # If no direction is suggested, indicating no simplex vertices, aim at the previously defined difference.
        if not dir:
            dir = diff
    # Compute the minimum penetration vector using EPA.
    pldr = Polyhedron(smpx)
    while True:
        dir = pldr.suggest_direction()
        point0 = shape0.find_support_point(-dir)
        point1 = shape1.find_support_point(dir)
        diff = point1 - point0
        if not pldr.add_vertex(diff):
            return pldr.compute_origin_translation()

def test(name, shape0, shape1):
    push = shape_intersects_shape(shape0, shape1)
    res = None
    if push:
        #print("Printing, man")
        #transform.print()
        shape2 = Box(transform + push)
        res = shape_intersects_shape(shape0, shape2) if push else None
    print("%s: %s (pushed: %s)" % (name, "Passed" if not res else "Failed", push))


#thx
translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box0 = Box(transform)
test("Same box", box0, box0)
#transform.print()


translation = Vector(0.5, 1.5, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box1 = Box(transform)
test("Intersecting boxes", box0, box1)

translation = Vector(0.0, 3.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box2 = Box(transform)
test("Separate boxes", box0, box2)

translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(1.0, 0.0, 0.0), 45) * Quaternion.from_axis_angle(Vector(1.0, 0.0, 0.0), 45)
scale = Vector(1.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box3 = Box(transform)
test("Intersecting boxes forming a star", box0, box3)

translation = Vector(0.0, 2.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box4 = Box(transform)
test("Boxes with faces touching", box0, box4)

translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box5 = Box(transform)
translation = Vector(0.0, 2.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(3.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box6 = Box(transform)
test("Tall and wide boxes forming a cross", box5, box6)

translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 30.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box7 = Box(transform)
translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), -30.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box8 = Box(transform)
test("Tall boxes forming an X", box7, box8)

translation = Vector(0.0, 0.0, 3.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), -30.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box9 = Box(transform)
test("Skew tall boxes", box7, box9)

translation = Vector(-5.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box10 = Box(transform)
translation = Vector(5.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), -45.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box11 = Box(transform)
test("Boxes forming a V with only edges touching", box10, box11)

translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(4.0, 4.0, 4.0)
transform = Transform.from_components(translation, rotation, scale)
box12 = Box(transform)
translation = Vector(0.5, 1.0, 0.0)
rotation = Quaternion.from_axis_angle(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Transform.from_components(translation, rotation, scale)
box13 = Box(transform)
test("Container and contained boxes", box12, box13)
