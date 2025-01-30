

#ifndef POLYEDRON_H
#define POLYEDRON_H

#include<functional>
#include<algorithm>
#include<numeric>

class Polyhedron {
public:
    std::vector<Triangle> triangles;
    std::unordered_map<vector3d, int> vertex_count;
    bool finished;

    // Construtor
    Polyhedron(const Simplex& simplex) : finished(false) {
        // Verifica se o simplex cont�m a origem
        if (std::find(simplex.vertices.begin(), simplex.vertices.end(), vector3d::ORIGIN) != simplex.vertices.end()) {
            finished = true;
            return;
        }

        // Inicializa os tri�ngulos a partir do simplex
        triangles = Triangle::fromSimplex(simplex);

        // Inicializa contagem dos v�rtices
        for (const auto& vertex : simplex.vertices) {
            vertex_count[vertex] = 3;
        }
    }

    // Adiciona um tri�ngulo ao poliedro
    void add_triangle(const Triangle& triangle) {
        triangles.push_back(triangle);
        for (const auto& vert : triangle.vertices) {
            vertex_count[vert]++;
        }
    }

    // Remove um tri�ngulo do poliedro
    void remove_triangle(const Triangle& triangle) {
        auto it = std::find(triangles.begin(), triangles.end(), triangle);
        if (it != triangles.end()) {
            triangles.erase(it);
            for (const auto& vert : triangle.vertices) {
                vertex_count[vert]--;
            }
        }
    }

    // Adiciona um v�rtice ao poliedro
    bool add_vertex(const vector3d& vertex) {
        if (finished) {
            return false;
        }

        // Termina se o v�rtice j� foi adicionado
        if (vertex_count.find(vertex) != vertex_count.end()) {
            finished = true;
            return false;
        }

        // Marca os tri�ngulos que podem ver o novo v�rtice para deletar
        std::vector<Triangle> oldtris;
        for (const auto& tri : triangles) {
            if (tri.faces_point(vertex)) {
                oldtris.push_back(tri);
            }
        }

        // Se nenhum tri�ngulo � marcado, o v�rtice j� faz parte do poliedro
        if (oldtris.empty()) {
            finished = true;
            return false;
        }

        // Remove os tri�ngulos e guarda os v�rtices �nicos
        std::vector<vector3d> rem;
        for (const auto& tri : oldtris) {
            remove_triangle(tri);
            for (const auto& vert : tri.vertices) {
                if (std::find(rem.begin(), rem.end(), vert) == rem.end()) {
                    rem.push_back(vert);
                }
            }
        }

        // Filtra os v�rtices que n�o t�m outras refer�ncias
        std::vector<vector3d> dangs;
        for (const auto& vert : rem) {
            if (vertex_count[vert] > 0) {
                dangs.push_back(vert);
            }
        }

        // Calcula um vetor m�dio entre o novo v�rtice e os v�rtices pendentes
        std::vector<vector3d> dirs;
        for (const auto& vert : dangs) {
            dirs.push_back((vert - vertex).normalize());
        }
        vector3d avgdir = std::accumulate(dirs.begin(), dirs.end(), vector3d::ORIGIN);

        // Calcula os eixos horizontal, vertical e profundidade
        vector3d dp = -avgdir.normalize();
        vector3d vt = avgdir.cross(dirs[0]).normalize();
        vector3d hz = avgdir.cross(vt).normalize();

        // Ordena os v�rtices pendentes pelo �ngulo
        std::vector<std::pair<vector3d, vector3d>> dangdirs;
        for (size_t i = 0; i < dangs.size(); ++i) {
            vector3d dir = (dirs[i] - dp * dirs[i].dot(dp)).normalize();
            dangdirs.push_back({dangs[i], dir});
        }

        std::sort(dangdirs.begin(), dangdirs.end(), [&](const auto& a, const auto& b) {
            return std::copysign(a.second.dot(hz) + 1.0, a.second.dot(vt)) < std::copysign(b.second.dot(hz) + 1.0, b.second.dot(vt));
        });

        // Cria novos tri�ngulos a partir dos �ngulos ordenados
        for (size_t idx = 0; idx < dangdirs.size(); ++idx) {
            Triangle tri(dangdirs[idx].first, dangdirs[(idx + 1) % dangdirs.size()].first, vertex);
            add_triangle(tri);
        }

        return true;
    }

    // Sugere uma dire��o
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

    // Calcula a transla��o da origem
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

#endif
