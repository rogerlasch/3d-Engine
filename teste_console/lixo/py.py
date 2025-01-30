from math import pi, sin

class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
    
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
        return self.x * other.x + self.y * other.y + self.z * other.z
    
    def cross(self, other):
        return Vector(self.y * other.z - self.z * other.y, self.z * other.x - self.x * other.z, self.x * other.y - self.y * other.x)
    
    def length(self):
        return self.dot(self) ** 0.5

class Quaternion:
    def __init__(self, vec, angle, raw = False):
        if raw:
            self.vector = vec
            self.scalar = angle
            return
        mul = sin(angle / 360.0 * pi)
        len = vec.length()
        self.vector = vec / len * mul
        self.scalar = (1.0 - mul * mul) ** 0.5
    
    def __mul__(self, other):
        if isinstance(other, Vector):
            conj = Quaternion(-self.vector, self.scalar, True)
            vec = Quaternion(other, 0.0, True)
            quat = self * vec * conj
            return quat.vector
        return Quaternion(self.vector.cross(other.vector) + self.vector * other.scalar + other.vector * self.scalar, self.scalar * other.scalar - self.vector.dot(other.vector), True)

class Matrix:
    def __init__(self, translation, rotation, scale):
        self.vec0 = rotation * Vector(scale.x, 0.0, 0.0)
        self.vec1 = rotation * Vector(0.0, scale.y, 0.0)
        self.vec2 = rotation * Vector(0.0, 0.0, scale.z)
        self.vec3 = translation
        self.norm0 = rotation * Vector(1.0, 0.0, 0.0)
        self.norm1 = rotation * Vector(0.0, 1.0, 0.0)
        self.norm2 = rotation * Vector(0.0, 0.0, 1.0)

class Ray:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction
    
    def closest_point(self, point):
        if point == self.origin:
            return point
        point = point - self.origin
        len = self.direction.length()
        if len == 0.0:
            return self.origin
        norm = self.direction / len
        fac = norm.dot(point) / len
        if fac < 0.0:
            return self.origin
        if fac > 1.0:
            return self.origin + self.direction
        return self.origin + self.direction * fac

class Plane:
    def __init__(self, origin, normal):
        self.origin = origin
        self.normal = normal
    
    def distance_to_point(self, point):
        point = point - self.origin
        return point.dot(self.normal)

class Box:
    def __init__(self, transform):
        self.transform = transform
    
    def contains_point(self, point):
        point = point - self.transform.vec3
        face = Plane(self.transform.vec0, self.transform.norm0)
        dist = face.distance_to_point(point)
        if dist < self.transform.vec0.length() * -2.0 or dist > 0.0:
            return False
        face = Plane(self.transform.vec1, self.transform.norm1)
        dist = face.distance_to_point(point)
        if dist < self.transform.vec1.length() * -2.0 or dist > 0.0:
            return False
        face = Plane(self.transform.vec2, self.transform.norm2)
        dist = face.distance_to_point(point)
        if dist < self.transform.vec2.length() * -2.0 or dist > 0.0:
            return False
        return True
    
    def intersects_ray(self, ray):
        point = ray.closest_point(self.transform.vec3)
        return self.contains_point(point)
    
    def intersects_box(self, other):
        if self.contains_point(other.transform.vec3):
            return True
        o0 = other.transform.vec3 - other.transform.vec0 - other.transform.vec1 - other.transform.vec2
        o1 = other.transform.vec3 + other.transform.vec0 - other.transform.vec1 - other.transform.vec2
        edge = Ray(o0, o1 - o0)
        if self.intersects_ray(edge):
            return True
        o2 = other.transform.vec3 + other.transform.vec0 + other.transform.vec1 - other.transform.vec2
        edge = Ray(o1, o2 - o1)
        if self.intersects_ray(edge):
            return True
        o3 = other.transform.vec3 - other.transform.vec0 + other.transform.vec1 - other.transform.vec2
        edge = Ray(o2, o3 - o2)
        if self.intersects_ray(edge):
            return True
        edge = Ray(o3, o0 - o3)
        if self.intersects_ray(edge):
            return True
        o4 = other.transform.vec3 - other.transform.vec0 - other.transform.vec1 + other.transform.vec2
        o5 = other.transform.vec3 + other.transform.vec0 - other.transform.vec1 + other.transform.vec2
        edge = Ray(o4, o5 - o4)
        if self.intersects_ray(edge):
            return True
        o6 = other.transform.vec3 + other.transform.vec0 + other.transform.vec1 + other.transform.vec2
        edge = Ray(o5, o6 - o5)
        if self.intersects_ray(edge):
            return True
        o7 = other.transform.vec3 - other.transform.vec0 + other.transform.vec1 + other.transform.vec2
        edge = Ray(o6, o7 - o6)
        if self.intersects_ray(edge):
            return True
        edge = Ray(o7, o4 - o7)
        if self.intersects_ray(edge):
            return True
        edge = Ray(o0, o4 - o0)
        if self.intersects_ray(edge):
            return True
        edge = Ray(o1, o5 - o1)
        if self.intersects_ray(edge):
            return True
        edge = Ray(o2, o6 - o2)
        if self.intersects_ray(edge):
            return True
        edge = Ray(o3, o7 - o3)
        if self.intersects_ray(edge):
            return True
        return False

def check_box_box_collision(box0, box1):
    return box0.intersects_box(box1) or box1.intersects_box(box0)

def test(name, actual, expected):
    print("%s: %s" % (name, "Passed" if actual == expected else "Failed"))

translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion(Vector(0.0, 0.0, 1.0), 0.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Matrix(translation, rotation, scale)
box0 = Box(transform)
test("Same AABB", check_box_box_collision(box0, box0), True)
translation = Vector(0.75, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box1 = Box(transform)
test("Overlapping AABBs with intersecting center points", check_box_box_collision(box0, box1), True)
translation = Vector(-0.75, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box2 = Box(transform)
test("Overlapping AABBs without intersecting center points", check_box_box_collision(box1, box2), True)
translation = Vector(2.0, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box3 = Box(transform)
test("Touching AABBs", check_box_box_collision(box0, box3), True)
translation = Vector(3.0, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box4 = Box(transform)
test("Non-overlapping AABBs", check_box_box_collision(box0, box4), False)
translation = Vector(0.0, 0.0, 0.0)
scale = Vector(2.0, 2.0, 2.0)
transform = Matrix(translation, rotation, scale)
box5 = Box(transform)
test("Container and contained ABBs", check_box_box_collision(box0, box5), True)
translation = Vector(0.0, 0.0, 0.0)
scale = Vector(1.0, 4.0, 1.0)
transform = Matrix(translation, rotation, scale)
box6 = Box(transform)
translation = Vector(4.0, 4.0, 0.0)
scale = Vector(4.0, 1.0, 1.0)
transform = Matrix(translation, rotation, scale)
box7 = Box(transform)
test("AABBs with overlapping ends", check_box_box_collision(box6, box7), True)

translation = Vector(0.0, 0.0, 0.0)
rotation = Quaternion(Vector(0.0, 0.0, 1.0), 45.0)
scale = Vector(1.0, 1.0, 1.0)
transform = Matrix(translation, rotation, scale)
box0 = Box(transform)
test("Same oriented box", check_box_box_collision(box0, box0), True)
translation = Vector(1.0, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box1 = Box(transform)
test("Overlapping oriented boxes with intersecting center points", check_box_box_collision(box0, box1), True)
translation = Vector(-1.0, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box2 = Box(transform)
test("Overlapping oriented boxes without intersecting center points", check_box_box_collision(box1, box2), True)
translation = Vector(3.0, 0.0, 0.0)
transform = Matrix(translation, rotation, scale)
box3 = Box(transform)
test("Non-overlapping oriented boxes", check_box_box_collision(box0, box3), False)
translation = Vector(0.0, 0.0, 0.0)
scale = Vector(2.0, 2.0, 2.0)
transform = Matrix(translation, rotation, scale)
box4 = Box(transform)
test("Container and contained oriented boxes", check_box_box_collision(box0, box4), True)
sincos = 2 ** 0.5 * 0.5
scale = Vector(1.0, 4.0, 1.0)
transform = Matrix(translation, rotation, scale)
box5 = Box(transform)
translation = Vector(4.0 * sincos, 4.0 * sincos, 0.0)
scale = Vector(4.0, 1.0, 1.0)
transform = Matrix(translation, rotation, scale)
box6 = Box(transform)
test("Oriented boxes with overlapping ends", check_box_box_collision(box5, box6), True)
rr=Ray(Vector(5,5,0), Vector(1,1,0))
pt=rr.closest_point(Vector(10,9,0))
print("{}, {}, {}", pt.x, pt.y, pt.z)