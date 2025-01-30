

#ifndef OBBGLM_H
#define OBBGLM_H

#include <glm/glm.hpp>
#include <vector>
#include <limits>


class EPAResult {
public:
    bool collision;        // Indica se houve colisão ou não
    glm::vec3 normal;      // A normal da colisão
    float penetrationDepth; // A profundidade de penetração
    glm::vec3 contactPoint; // O ponto de contato

    // Construtor padrão inicializa com valores default
    EPAResult()
        : collision(false), normal(glm::vec3(0.0f)), penetrationDepth(0.0f), contactPoint(glm::vec3(0.0f)) {}

    // Construtor para inicializar com valores específicos
    EPAResult(bool col, const glm::vec3& norm, float depth, const glm::vec3& contact)
        : collision(col), normal(norm), penetrationDepth(depth), contactPoint(contact) {}
};


class Simplex {
public:
    // Adiciona um novo ponto ao simplex
    void Add(const glm::vec3& point) {
assert(vertices.size()<=4);
        vertices.push_back(point);
    }

    // Retorna o último ponto adicionado ao simplex
    glm::vec3 GetLast() const {
        return vertices.back();
    }

    // Limpa os pontos do simplex
    void Clear() {
        vertices.clear();
    }

    // Função para verificar se o simplex contém a origem
    // e atualizar a direção para a próxima iteração do GJK
    bool ContainsOrigin(glm::vec3& direction) {
        switch (vertices.size()) {
            case 2:
                return LineCase(direction);
            case 3:
                return TriangleCase(direction);
            case 4:
                return TetrahedronCase(direction);
        }
        return false;
    }

std::vector<glm::vec3> GetVertices()const{return vertices;}

private:
    std::vector<glm::vec3> vertices;

    // Caso de linha (2 pontos no simplex)
    bool LineCase(glm::vec3& direction) {
        const glm::vec3& A = vertices[1];
        const glm::vec3& B = vertices[0];

        glm::vec3 AB = B - A;
        glm::vec3 AO = -A;

        // Nova direção é perpendicular a AB em direção à origem
        direction = glm::cross(glm::cross(AB, AO), AB);
        return false;
    }

    // Caso de triângulo (3 pontos no simplex)
    bool TriangleCase(glm::vec3& direction) {
        const glm::vec3& A = vertices[2];
        const glm::vec3& B = vertices[1];
        const glm::vec3& C = vertices[0];

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AO = -A;

        glm::vec3 ABCNormal = glm::cross(AB, AC);

        if (glm::dot(glm::cross(ABCNormal, AC), AO) > 0) {
            if (glm::dot(AC, AO) > 0) {
                vertices = { A, C };
                direction = glm::cross(glm::cross(AC, AO), AC);  // Perpendicular a AC
            } else {
                return LineCase(direction);  // Verificar se precisa reduzir para o caso de linha
            }
        } else {
            if (glm::dot(glm::cross(AB, ABCNormal), AO) > 0) {
                return LineCase(direction);  // Verificar se precisa reduzir para o caso de linha
            } else {
                if (glm::dot(ABCNormal, AO) > 0) {
                    direction = ABCNormal;  // Direção para fora do triângulo
                } else {
                    vertices = { A, C, B };  // Inverter a ordem dos vértices
                    direction = -ABCNormal;  // Direção para o outro lado
                }
            }
        }
        return false;
    }

    // Caso de tetraedro (4 pontos no simplex)
    bool TetrahedronCase(glm::vec3& direction) {
        const glm::vec3& A = vertices[3];
        const glm::vec3& B = vertices[2];
        const glm::vec3& C = vertices[1];
        const glm::vec3& D = vertices[0];

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AD = D - A;
        glm::vec3 AO = -A;

        glm::vec3 ABCNormal = glm::cross(AB, AC);
        glm::vec3 ACDNormal = glm::cross(AC, AD);
        glm::vec3 ADBNormal = glm::cross(AD, AB);

        // Verifica cada face do tetraedro para determinar a próxima direção
        if (glm::dot(ABCNormal, AO) > 0) {
            vertices = { A, B, C };
            return TriangleCase(direction);
        }

        if (glm::dot(ACDNormal, AO) > 0) {
            vertices = { A, C, D };
            return TriangleCase(direction);
        }

        if (glm::dot(ADBNormal, AO) > 0) {
            vertices = { A, D, B };
            return TriangleCase(direction);
        }

        // A origem está dentro do tetraedro
        return true;
    }
};

class Polytope {
public:
    // Estrutura para uma face da poliedro
    struct Face {
        glm::vec3 normal;
        float distance;  // Distância da face até a origem
        int indices[3];  // Índices dos vértices que compõem a face

        Face(int a, int b, int c, const std::vector<glm::vec3>& vertices) {
            indices[0] = a;
            indices[1] = b;
            indices[2] = c;
            ComputeNormalAndDistance(vertices);
        }

        // Calcula a normal da face e sua distância até a origem
        void ComputeNormalAndDistance(const std::vector<glm::vec3>& vertices) {
            glm::vec3 AB = vertices[indices[1]] - vertices[indices[0]];
            glm::vec3 AC = vertices[indices[2]] - vertices[indices[0]];
            normal = glm::normalize(glm::cross(AB, AC));
            distance = glm::dot(normal, vertices[indices[0]]);
        }

        // Verifica se um ponto está "atrás" da face
        bool IsPointBehindFace(const glm::vec3& point) const {
            return glm::dot(normal, point) > distance;
        }
    };

    // Construtor: inicializa a poliedro com o simplex resultante do GJK
    Polytope(const Simplex& simplex) {
        const auto& verts = simplex.GetVertices();
        vertices = verts;

        // Inicializa as faces com base nos vértices do simplex (um tetraedro)
        faces.push_back(Face(0, 1, 2, vertices));
        faces.push_back(Face(0, 1, 3, vertices));
        faces.push_back(Face(0, 2, 3, vertices));
        faces.push_back(Face(1, 2, 3, vertices));
    }

    // Adiciona um novo vértice ao poliedro e atualiza as faces
    void AddVertex(const glm::vec3& newVertex) {
        int newVertexIndex = vertices.size();
        vertices.push_back(newVertex);

        std::vector<Face> newFaces;
        std::vector<Face> oldFacesToRemove;

        // Para cada face, verifica se o novo vértice está "atrás" da face
        for (const Face& face : faces) {
            if (face.IsPointBehindFace(newVertex)) {
                oldFacesToRemove.push_back(face);
                // Para cada face "visível" ao novo ponto, adicione novas faces ligando o novo vértice
                newFaces.push_back(Face(face.indices[0], face.indices[1], newVertexIndex, vertices));
                newFaces.push_back(Face(face.indices[1], face.indices[2], newVertexIndex, vertices));
                newFaces.push_back(Face(face.indices[2], face.indices[0], newVertexIndex, vertices));
            }
        }

        // Remove as faces antigas
        for (const Face& faceToRemove : oldFacesToRemove) {
            RemoveFace(faceToRemove);
        }

        // Adiciona as novas faces
        faces.insert(faces.end(), newFaces.begin(), newFaces.end());
    }

    // Retorna a face mais próxima da origem
    Face GetClosestFaceToOrigin() const {
        Face closestFace = faces[0];
        float minDistance = std::numeric_limits<float>::max();

        for (const Face& face : faces) {
            if (face.distance < minDistance) {
                minDistance = face.distance;
                closestFace = face;
            }
        }
        return closestFace;
    }

private:
    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    // Remove uma face específica do poliedro
    void RemoveFace(const Face& faceToRemove) {
        faces.erase(
            std::remove_if(faces.begin(), faces.end(), [&](const Face& face) {
                return face.indices[0] == faceToRemove.indices[0] &&
                       face.indices[1] == faceToRemove.indices[1] &&
                       face.indices[2] == faceToRemove.indices[2];
            }),
            faces.end());
    }
};

// Classe base abstrata para uma forma geométrica
class Shape {
public:
    virtual ~Shape() = default;

    // Função virtual pura que retorna o ponto de suporte (support point)
    // na direção especificada
    virtual glm::vec3 GetFurthestPoint(const glm::vec3& direction) const = 0;
};

// Implementação de uma caixa
class Box : public Shape {
public:
    glm::vec3 min;  // Extremo mínimo (canto inferior esquerdo)
    glm::vec3 max;  // Extremo máximo (canto superior direito)

    Box(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    // Função de suporte para a caixa
    glm::vec3 GetFurthestPoint(const glm::vec3& direction) const override {
        glm::vec3 result;
        result.x = (direction.x > 0) ? max.x : min.x;
        result.y = (direction.y > 0) ? max.y : min.y;
        result.z = (direction.z > 0) ? max.z : min.z;
        return result;
    }
};

// Implementação de uma esfera
class Sphere : public Shape {
public:
    glm::vec3 center;
    float radius;

    Sphere(const glm::vec3& center, float radius) : center(center), radius(radius) {}

    // Função de suporte para a esfera
    glm::vec3 GetFurthestPoint(const glm::vec3& direction) const override {
glm::vec3 v=center + glm::normalize(direction) * radius;
cout<<v.x<<", "<<v.y<<", "<<v.z<<endl;
return v;
    }
};

// Implementação de um polígono convexo (genérico)
class ConvexPolygon : public Shape {
public:
    std::vector<glm::vec3> vertices;

    ConvexPolygon(const std::vector<glm::vec3>& vertices) : vertices(vertices) {}

    // Função de suporte para o polígono convexo
    glm::vec3 GetFurthestPoint(const glm::vec3& direction) const override {
        float maxDot = -std::numeric_limits<float>::max();
        glm::vec3 supportPoint;

        // Encontra o vértice mais distante na direção dada
        for (const auto& vertex : vertices) {
            float dotProduct = glm::dot(vertex, direction);
            if (dotProduct > maxDot) {
                maxDot = dotProduct;
                supportPoint = vertex;
            }
        }

        return supportPoint;
    }
};

glm::vec3 Support(const Shape& shapeA, const Shape& shapeB, const glm::vec3& direction) {
    glm::vec3 pA = shapeA.GetFurthestPoint(direction);
    glm::vec3 pB = shapeB.GetFurthestPoint(-direction);
    return pA - pB;  // Ponto no espaço de Minkowski
}

bool GJK(const Shape& shapeA, const Shape& shapeB, Simplex& simplex) {
    glm::vec3 direction(0.0f, 0.0f, 1.0f);  // Começamos com uma direção arbitrária
    simplex.Add(Support(shapeA, shapeB, direction));

    direction = -simplex.GetLast();  // Nova direção é na direção inversa ao último ponto

int x=0;
    while (x<20){
x++;
        glm::vec3 newPoint = Support(shapeA, shapeB, direction);
        if (glm::dot(newPoint, direction) <= 0) {
            return false;  // Não há colisão
        }

        simplex.Add(newPoint);
        if (simplex.ContainsOrigin(direction)) {
            return true;  // Colisão detectada
        }
    }
}

EPAResult EPA(const Shape& shapeA, const Shape& shapeB, const Simplex& simplex) {
    Polytope polytope( simplex);  // Expandir o simplex para uma polítope
    glm::vec3 direction;
    float minDistance = FLT_MAX;
    glm::vec3 normal;
    glm::vec3 contactPoint;
int x=0;
    while (x<1024) {
x++;
//std::cout<<x<<std::endl;
        Polytope::Face closestFace = polytope.GetClosestFaceToOrigin();
        direction = closestFace.normal;
        glm::vec3 supportPoint = Support(shapeA, shapeB, direction);

        float distance = glm::dot(supportPoint, direction);
        if (distance - closestFace.distance < 0.0001f) {
            return {true,  normal, distance, contactPoint };
        }

        polytope.AddVertex(supportPoint);
    }
}
#endif
