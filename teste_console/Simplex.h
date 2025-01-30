

#ifndef SIMPLEX_H
#define SIMPLEX_H

class Simplex {
public:
    std::vector<vector3d> vertices;
    std::vector<std::vector<vector3d>> attempts;
    bool finished;

    Simplex() : finished(false) {}
Simplex(const Simplex& spx){
*this=spx;
}
Simplex& operator=(const Simplex& spx){

if(this!=&spx){
this->vertices=spx.vertices;
this->attempts=spx.attempts;
this->finished=spx.finished;
}

return *this;
}

    bool pushVertex(const vector3d& vertex) {
        if (finished) {
            return true;
        }
        // Rejeita se o último segmento de linha formado com este vértice não passa pela origem
        if (!vertices.empty() && vertex.dot(vertex - vertices.back()) < 0.0f) {
            return false;
        }
        // Rejeita se o vértice for a origem
        if (vector3dIsEqual(vertex ,  vector3d::ORIGIN, 0.01f)) {
            finished = true;
            return false;
        }

        switch (vertices.size()) {
            case 2: {
                // Rejeita se a linha formada pelos outros dois vértices contém este vértice
                if (vector3dIsEqual((vertices[1] - vertices[0]).cross(vertex), vector3d::ORIGIN, 0.01f)) {
                    return false;
                }
                break;
            }
            case 3: {
                // Rejeita se o vértice é coplanar ao plano formado pelos outros três vértices
                vector3d perp = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[0]);
                if (perp.dot(vertex - vertices[0]) == 0.0f) {
                    return false;
                }
                break;
            }
            case 4:
                vertices.clear();
                break;
        }

        // Falha se o vértice for duplicado
        if (std::find(vertices.begin(), vertices.end(), vertex) != vertices.end()) {
            return false;
        }

        vertices.push_back(vertex);

        // Falha se este simplex já tiver sido tentado
        if (vertices.size() == 4) {
            for (const auto& attempt : attempts) {
                bool all_vertices_match = true;
                for (const auto& v : vertices) {
                    if (std::find(attempt.begin(), attempt.end(), v) == attempt.end()) {
                        all_vertices_match = false;
                        break;
                    }
                }
                if (all_vertices_match) {
                    return false;
                }
            }
            attempts.push_back(vertices);
        }

        return true;
    }

    bool containsOrigin() {
        if (finished) {
            return vertices.size() == 4;
        }
        if (vertices.size() == 4) {
            for (const auto& face : { std::array<int, 4>{0, 1, 2, 3}, std::array<int, 4>{0, 1, 3, 2},
                                      std::array<int, 4>{0, 2, 3, 1}, std::array<int, 4>{1, 2, 3, 0} }) {
                vector3d perp = (vertices[face[1]] - vertices[face[0]]).cross(vertices[face[2]] - vertices[face[0]]);
                perp = perp * perp.dot(vertices[face[0]] - vertices[face[3]]);
                if (perp.dot(vertices[face[0]]) < 0.0f) {
                    break;
                }
            }
            finished = true;
        } else {
            return false;
        }
        return finished;
    }

    vector3d suggestDirection() {
        if (finished) {
            return vector3d::ORIGIN;
        }

        switch (vertices.size()) {
            case 1:
                // Retorna a direção para a origem
                return -vertices[0];
            case 2: {
                // Se o segmento cruza a origem, sugere um vetor perpendicular aleatório
                if (vector3dIsEqual(vertices[0].cross(vertices[1]), vector3d::ORIGIN, 0.01f)) {
                    vector3d perpx = vector3d(1.0f, 0.0f, 0.0f).cross(vertices[1]);
                    return (perpx != vector3d::ORIGIN) ? perpx : vector3d(0.0f, 1.0f, 0.0f).cross(vertices[1]);
                }
                // Retorna um vetor perpendicular ao segmento na direção geral da origem
                return (vertices[1] - vertices[0]).cross(-vertices[0]).cross(vertices[1] - vertices[0]);
            }
            case 3: {
                // Se a origem estiver no mesmo plano que o triângulo, retorna um vetor perpendicular aleatório
                vector3d perp = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[0]);
                decimal proj = perp.dot(vertices[0]);
                if (proj == 0.0f) {
                    return perp;
                }
                // Retorna o vetor perpendicular ao triângulo na direção geral da origem
                return perp * -proj;
            }
            default:
                return vector3d::ORIGIN;
        }
    }
};

#endif
