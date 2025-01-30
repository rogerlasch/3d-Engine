

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle {
public:
    std::array<vector3d, 3> vertices;
    vector3d normal;
    decimal distance;

Triangle(){
}

    // Construtor
    Triangle(const vector3d& vertex0, const vector3d& vertex1, const vector3d& vertex2) {
        vertices = {vertex0, vertex1, vertex2};
        vector3d perp = (vertex1 - vertex0).cross(vertex2 - vertex0);
        normal = perp.normalize();
        distance = normal.dot(vertex0);
    }
Triangle(const Triangle& tr){
*this=tr;
}

Triangle& operator=(const Triangle& tr){

if(this!=&tr){
this->vertices=tr.vertices;
this->normal=tr.normal;
this->distance=tr.distance;
}
return *this;
}

    bool operator==(const Triangle& other) const {
for(uint32 i=0; i<3; i++){
if(!vector3dIsEqual(vertices[i], other.vertices[i], 0.01f)){
return false;
}
}

return true;
    }

    // Verifica se o triângulo "olha" para o ponto
    bool faces_point(const vector3d& point) const {
        return normal.dot(point - vertices[0]) > 0.0;
    }

    // Cria uma lista de triângulos a partir de um simplex
    static std::vector<Triangle> fromSimplex(const Simplex& simplex) {
        std::vector<Triangle> tris;
        const std::array<std::array<int, 4>, 4> facets = {{
            {0, 1, 2, 3},
            {1, 0, 3, 2},
            {2, 3, 0, 1},
            {3, 2, 1, 0}
        }};

        for (const auto& facet : facets) {
            vector3d perp = (simplex.vertices[facet[1]] - simplex.vertices[facet[0]]).cross(simplex.vertices[facet[2]] - simplex.vertices[facet[0]]);
            if (perp.dot(simplex.vertices[facet[0]] - simplex.vertices[facet[3]]) > 0.0) {
                tris.emplace_back(simplex.vertices[facet[0]], simplex.vertices[facet[1]], simplex.vertices[facet[2]]);
            } else {
                tris.emplace_back(simplex.vertices[facet[2]], simplex.vertices[facet[1]], simplex.vertices[facet[0]]);
            }
        }
        return tris;
    }
};
#endif
