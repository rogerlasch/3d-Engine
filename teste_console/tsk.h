

#ifndef TSK_H
#define TSK_H

#include<tuple>
#include<functional>
#include<algorithm>

#define qvh(x, y, z) quaternion_from_euler_angles(x, y, z)

class Transform{
public:
vector3d vec0, vec1, vec2, vec3;
vector3d norm0, norm1, norm2;
Transform(){}
Transform(const vector3d& translation, const quaternion& rotation, const vector3d& scale){
create(translation, rotation, scale);
}
void create(const vector3d& translation, const quaternion& rotation, const vector3d& scale){
vec0=quaternion_vector_rotate(rotation, {scale.x, 0,0});
vec1=quaternion_vector_rotate(rotation, {0, scale.y, 0});
vec2=quaternion_vector_rotate(rotation, {0, 0, scale.z});
vec3=quaternion_vector_rotate(rotation, translation);
norm0=quaternion_vector_rotate(rotation, {1,0,0});
norm1=quaternion_vector_rotate(rotation, {0,1,0});
norm2=quaternion_vector_rotate(rotation, {0,0,1});
}
};


class Tetrahedron {
private:
    std::vector<vector3d> vertices;

public:
    Tetrahedron() {}

    void add_vertex(const vector3d& vertex) {
        if (vertices.size() < 4 && std::find(vertices.begin(), vertices.end(), vertex) == vertices.end()) {
            vertices.push_back(vertex);
        }
    }

    bool contains_origin() {
        const decimal epsilon = 1e-6;
        if (vertices.size() < 4) {
            return false;
        }

        for (const auto& face : {std::make_tuple(0, 1, 2, 3), std::make_tuple(0, 1, 3, 2),
                                  std::make_tuple(0, 2, 3, 1), std::make_tuple(1, 2, 3, 0)}) {
            vector3d origin = vertices[std::get<0>(face)];
            vector3d perp = vector3d::cross(vertices[std::get<1>(face)] - origin, vertices[std::get<2>(face)] - origin);
            perp = (vector3d::dot(perp, vertices[std::get<3>(face)] - origin) > 0.0) ? perp : -perp;
            if (vector3d::dot(perp, -origin) > epsilon) {
                decimal minlen = std::numeric_limits<decimal>::infinity();
                std::vector<vector3d> verts;
                for (const auto& vert : vertices) {
                    decimal sqlen = vert*vert;
                    if (sqlen < minlen) {
                        minlen = sqlen;
                        verts = {vert};
                    } else if (sqlen == minlen) {
                        verts.push_back(vert);
                    }
                }
                vertices = verts;
                return false;
            }
        }
        return true;
    }
};

class Box {
public:
    vector3d position;
    std::vector<vector3d> vertices;
public:
    Box(const Transform& transform) {
        position = transform.vec3;
        vertices = {
            transform.vec3 - transform.vec0 - transform.vec1 - transform.vec2,
            transform.vec3 + transform.vec0 - transform.vec1 - transform.vec2,
            transform.vec3 - transform.vec0 + transform.vec1 - transform.vec2,
            transform.vec3 + transform.vec0 + transform.vec1 - transform.vec2,
            transform.vec3 - transform.vec0 - transform.vec1 + transform.vec2,
            transform.vec3 + transform.vec0 - transform.vec1 + transform.vec2,
            transform.vec3 - transform.vec0 + transform.vec1 + transform.vec2,
            transform.vec3 + transform.vec0 + transform.vec1 + transform.vec2
        };
    }
    vector3d find_support_point(const vector3d& direction)const {
        if ((direction*direction) == 0.0) {
            throw std::invalid_argument("Direction is null vector");
        }
        vector3d best;
        decimal max = -std::numeric_limits<decimal>::infinity();
        for (const auto& vertex : vertices) {
            decimal dot = vector3d::dot(vertex, direction);
            if (dot > max) {
                best = vertex;
                max = dot;
            }
        }
        return best;
    }
};

decimal epsilon = 1e-6; // Define epsilon value as needed

bool shape_intersects_shape(const Box& shape0, const Box& shape1) {
    if (shape1.position == shape0.position) {
        return true;
    }
    vector3d dir = shape1.position - shape0.position;
    vector3d point0 = shape0.find_support_point(-dir);
    vector3d point1 = shape1.find_support_point(dir);
    vector3d diff0 = point1 - point0;
    point0 = shape0.find_support_point(dir);
    point1 = shape1.find_support_point(-dir);
    vector3d diff1 = point1 - point0;
    if (vector3d::dot(diff0, diff1) > 0.0) {
        return false;
    }
    std::vector<vector3d> dirs;
    Tetrahedron tet;
    tet.add_vertex(diff0);
    tet.add_vertex(diff1);
    while (true) {
        if (dirs.size() >= 256) {
            throw std::runtime_error("Too many iterations");
        }
        vector3d dir =vector3d::cross(vector3d::cross(diff1 - diff0, -diff1), diff1 - diff0);
        if (vector3d::dot(dir, dir) < epsilon) {
            return true;
        }
        if (std::find(dirs.begin(), dirs.end(), dir) != dirs.end()) {
            return false;
        }
        dirs.push_back(dir);
        point0 = shape0.find_support_point(dir);
        point1 = shape1.find_support_point(-dir);
        diff0 = diff1;
        diff1 = point1 - point0;
        tet.add_vertex(diff1);
        if (tet.contains_origin()) {
            return true;
        }
    }
}


#endif
