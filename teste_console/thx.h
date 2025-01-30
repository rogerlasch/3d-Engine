

#ifndef THX_H
#define THX_H

#include <iostream>
#include <array>
#include<algorithm>
#include<functional>
#include<numeric>
#include <vector>
#include <stdexcept>
#include<optional>
#include <limits>
#include <cmath> // Para std::numeric_limits

using namespace std;

// Classe Transform
class Transform {
public:
    vector3d vec0, vec1, vec2, vec3;

Transform(){
}
    Transform(const vector3d& vec0, const vector3d& vec1, const vector3d& vec2, const vector3d& vec3)
        : vec0(vec0), vec1(vec1), vec2(vec2), vec3(vec3) {}

Transform(const Transform& t){
*this=t;
}

Transform& operator=(const Transform& t){
this->vec0=t.vec0;
this->vec1=t.vec1;
this->vec2=t.vec2;
this->vec3=t.vec3;
return *this;
}
    // Operador de adição para adicionar uma translação
    Transform operator+(const vector3d& translation) const {
        return Transform(vec0, vec1, vec2, vec3 + translation);
    }

    static Transform fromComponents(const vector3d& translation, const quaternion& rotation, const vector3d& scale) {
        return Transform(
            quaternion_vector_rotate(rotation, vector3d(scale.x, 0.0f, 0.0f)),
            quaternion_vector_rotate(rotation, vector3d(0.0f, scale.y, 0.0f)),
            quaternion_vector_rotate(rotation, vector3d(0.0f, 0.0f, scale.z)),
            quaternion_vector_rotate(rotation, translation)
        );
    }

    void print() const {
cout.precision(3);
cout<<vec0<<endl<<vec1<<endl<<vec2<<endl<<vec3<<endl;
    }
};

// Classe Simplex
class Simplex {
public:
    std::vector<vector3d> vertices;
    std::vector<std::vector<vector3d>> attempts;
    bool finished;

    Simplex() : finished(false) {}

    bool add_vertex(const vector3d& vertex) {
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

    bool contains_origin() {
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

    vector3d suggest_direction() {
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

class Triangle {
public:
    std::array<vector3d, 3> vertices;
    vector3d normal;
    double distance;

    // Construtor
    Triangle(const vector3d& vertex0, const vector3d& vertex1, const vector3d& vertex2) {
        vertices = {vertex0, vertex1, vertex2};
        vector3d perp = (vertex1 - vertex0).cross(vertex2 - vertex0);
        normal = perp.normalize();
        distance = normal.dot(vertex0);
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
    static std::vector<Triangle> from_simplex(const Simplex& simplex) {
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

class Polyhedron {
public:
    std::vector<Triangle> triangles;
    std::unordered_map<vector3d, int> vertex_count;
    bool finished;

    // Construtor
    Polyhedron(const Simplex& simplex) : finished(false) {
        // Verifica se o simplex contém a origem
        if (std::find(simplex.vertices.begin(), simplex.vertices.end(), vector3d::ORIGIN) != simplex.vertices.end()) {
            finished = true;
            return;
        }

        // Inicializa os triângulos a partir do simplex
        triangles = Triangle::from_simplex(simplex);

        // Inicializa contagem dos vértices
        for (const auto& vertex : simplex.vertices) {
            vertex_count[vertex] = 3;
        }
    }

    // Adiciona um triângulo ao poliedro
    void add_triangle(const Triangle& triangle) {
        triangles.push_back(triangle);
        for (const auto& vert : triangle.vertices) {
            vertex_count[vert]++;
        }
    }

    // Remove um triângulo do poliedro
    void remove_triangle(const Triangle& triangle) {
        auto it = std::find(triangles.begin(), triangles.end(), triangle);
        if (it != triangles.end()) {
            triangles.erase(it);
            for (const auto& vert : triangle.vertices) {
                vertex_count[vert]--;
            }
        }
    }

    // Adiciona um vértice ao poliedro
    bool add_vertex(const vector3d& vertex) {
        if (finished) {
            return false;
        }

        // Termina se o vértice já foi adicionado
        if (vertex_count.find(vertex) != vertex_count.end()) {
            finished = true;
            return false;
        }

        // Marca os triângulos que podem ver o novo vértice para deletar
        std::vector<Triangle> oldtris;
        for (const auto& tri : triangles) {
            if (tri.faces_point(vertex)) {
                oldtris.push_back(tri);
            }
        }

        // Se nenhum triângulo é marcado, o vértice já faz parte do poliedro
        if (oldtris.empty()) {
            finished = true;
            return false;
        }

        // Remove os triângulos e guarda os vértices únicos
        std::vector<vector3d> rem;
        for (const auto& tri : oldtris) {
            remove_triangle(tri);
            for (const auto& vert : tri.vertices) {
                if (std::find(rem.begin(), rem.end(), vert) == rem.end()) {
                    rem.push_back(vert);
                }
            }
        }

        // Filtra os vértices que não têm outras referências
        std::vector<vector3d> dangs;
        for (const auto& vert : rem) {
            if (vertex_count[vert] > 0) {
                dangs.push_back(vert);
            }
        }

        // Calcula um vetor médio entre o novo vértice e os vértices pendentes
        std::vector<vector3d> dirs;
        for (const auto& vert : dangs) {
            dirs.push_back((vert - vertex).normalize());
        }
        vector3d avgdir = std::accumulate(dirs.begin(), dirs.end(), vector3d::ORIGIN);

        // Calcula os eixos horizontal, vertical e profundidade
        vector3d dp = -avgdir.normalize();
        vector3d vt = avgdir.cross(dirs[0]).normalize();
        vector3d hz = avgdir.cross(vt).normalize();

        // Ordena os vértices pendentes pelo ângulo
        std::vector<std::pair<vector3d, vector3d>> dangdirs;
        for (size_t i = 0; i < dangs.size(); ++i) {
            vector3d dir = (dirs[i] - dp * dirs[i].dot(dp)).normalize();
            dangdirs.push_back({dangs[i], dir});
        }

        std::sort(dangdirs.begin(), dangdirs.end(), [&](const auto& a, const auto& b) {
            return std::copysign(a.second.dot(hz) + 1.0, a.second.dot(vt)) < std::copysign(b.second.dot(hz) + 1.0, b.second.dot(vt));
        });

        // Cria novos triângulos a partir dos ângulos ordenados
        for (size_t idx = 0; idx < dangdirs.size(); ++idx) {
            Triangle tri(dangdirs[idx].first, dangdirs[(idx + 1) % dangdirs.size()].first, vertex);
            add_triangle(tri);
        }

        return true;
    }

    // Sugere uma direção
    vector3d suggestDirection() const {
        vector3d dir;
        double mindist = std::numeric_limits<double>::infinity();
        for (const auto& tri : triangles) {
            double dist = tri.distance;
            if (dist < mindist) {
                dir = tri.normal;
                mindist = dist;
            }
        }
        return dir;
    }

    // Calcula a translação da origem
    vector3d compute_origin_translation() const {
        if (!finished) {
            return vector3d::ORIGIN;
        }

        double mindist = std::numeric_limits<double>::infinity();
        vector3d norm;

        for (const auto& tri : triangles) {
            double dist = tri.distance;
            if (dist < mindist) {
                mindist = dist;
                norm = tri.normal;
            }
        }

        if (!vector3dIsEqual(norm, vector3d::ORIGIN)) {
            return norm * -mindist;
        }

        return vector3d::ORIGIN;
    }
};

class Box {
public:
    vector3d position;
    std::array<vector3d, 8> vertices;
Transform htransform;
    // Construtor
    Box(const Transform& transform) {
        position = transform.vec3;
this->htransform=transform;
        vertices = {
            position - transform.vec0 - transform.vec1 - transform.vec2,
            position + transform.vec0 - transform.vec1 - transform.vec2,
            position - transform.vec0 + transform.vec1 - transform.vec2,
            position + transform.vec0 + transform.vec1 - transform.vec2,
            position - transform.vec0 - transform.vec1 + transform.vec2,
            position + transform.vec0 - transform.vec1 + transform.vec2,
            position - transform.vec0 + transform.vec1 + transform.vec2,
            position + transform.vec0 + transform.vec1 + transform.vec2
        };
    }

Transform getTransform()const{
return htransform;
}
    // Encontra o ponto de suporte (o ponto mais distante na direção fornecida)
    vector3d find_support_point(const vector3d& direction) const {
        if (direction == vector3d::ORIGIN) {
            throw std::invalid_argument("Direction is null vector");
        }

        // Procura o ponto mais distante em uma direção específica
        vector3d best;
        double max_dot = -std::numeric_limits<double>::infinity();

        for (const auto& vertex : vertices) {
            double dot = vertex.dot(direction);
            if (dot > max_dot) {
                best = vertex;
                max_dot = dot;
            }
        }

        return best;
    }
};

vector3d shape_intersects_shape(const Box& shape0, const Box& shape1) {
    Simplex smpx;
    // Gerar o primeiro vetor de direção.
    vector3d dir = shape1.position - shape0.position;
    if (dir == vector3d::ORIGIN) {
        dir = vector3d(0.0, 0.0, 1.0); // Define uma direção arbitrária
    }

    // Verifica se as formas estão se intersectando usando o algoritmo GJK.
    while (true) {
        vector3d point0 = shape0.find_support_point(-dir);
        vector3d point1 = shape1.find_support_point(dir);
        vector3d diff = point1 - point0;

        // Se o vértice não puder ser adicionado, não há como continuar.
        if (!smpx.add_vertex(diff)) {
            return vector3d::ORIGIN;
        }

        // Se o simplex contém a origem, os sólidos estão sobrepostos.
        if (smpx.contains_origin()) {
            break;
        }

        dir = smpx.suggest_direction();
        // Se nenhuma direção for sugerida, mira na diferença anteriormente definida.
        if (dir==vector3d::ORIGIN) {
            dir = diff;
        }
    }

    // Computa o vetor de penetração mínima usando EPA.
    Polyhedron pldr(smpx);
    while (true) {
        dir = pldr.suggest_direction();
        vector3d point0 = shape0.find_support_point(-dir);
        vector3d point1 = shape1.find_support_point(dir);
        vector3d diff = point1 - point0;

        if (!pldr.add_vertex(diff)) {
            return pldr.compute_origin_translation();  // Retorna o vetor de penetração
        }
    }
return vector3d::ORIGIN;
}

#endif
