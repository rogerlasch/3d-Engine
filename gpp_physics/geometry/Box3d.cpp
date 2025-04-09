
#include"geometry.h"

using namespace std;

namespace gpp {

Box3d::Box3d(const vector3d& extents, Transform* transform)
    : GeometricShape(GTYPE_BOX, transform), extents(extents) {}

std::string Box3d::toString() const {
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(2);

    quaternion q = transform->getOrientation();
    AABB ab;
getAABB(&ab);
vector<vector3d> vertices;
getVertices(vertices);
    ss << "Type=" << type << " Box3d, Extents=" << extents << std::endl;
    ss << "Position=" << transform->getPosition() << std::endl;
    ss << "Scale=" << transform->getScale() << std::endl;
    ss << "Orientation Euler=" << quaternion_extract_euler_angles(q) << std::endl;
    ss << "Orientation=" << q << std::endl;
    ss << ab.toString();
ss<<"Vértices={"<<endl;
for(auto& it : vertices){
ss<<it<<endl;
}
ss<<"}";

    return ss.str();
}

std::string Box3d::getShortDescription()const{
stringstream ss;
ss<<fixed;
ss.precision(2);

ss<<"Type=Box3d, Extents="<<extents;
return ss.str();
}

vector3d Box3d::getClosestPoint(const vector3d& pt) const {
    // Converte o ponto para o espaço local da caixa
    vector3d localPt = transform->toLocal(pt);

    // Limita o ponto às dimensões da caixa
    localPt.x = std::max(-extents.x, std::min(extents.x, localPt.x));
    localPt.y = std::max(-extents.y, std::min(extents.y, localPt.y));
    localPt.z = std::max(-extents.z, std::min(extents.z, localPt.z));

    // Converte o ponto de volta para o espaço global
    return transform->toGlobal(localPt);
}

vector3d Box3d::getNormal(const vector3d& pt)const{
    vector3d center = getPosition();
    vector3d localPoint = getTransform()->toLocal(pt);
    vector3d normal(0, 0, 0);

    // Encontra o eixo com maior penetração
    vector3d penetration = extents - localPoint.abs();

    if (penetration.x < penetration.y && penetration.x < penetration.z) {
        normal.x = (localPoint.x > 0) ? 1.0f : -1.0f;
    }
    else if (penetration.y < penetration.z) {
        normal.y = (localPoint.y > 0) ? 1.0f : -1.0f;
    }
    else {
        normal.z = (localPoint.z > 0) ? 1.0f : -1.0f;
    }

    // Converte a normal para coordenadas globais
    return getTransform()->toGlobal(normal).normalize();

}

bool Box3d::contains(const vector3d& pt) const {
    // Converte o ponto para o espaço local da caixa
    vector3d localPt = transform->toLocal(pt);

    // Verifica se o ponto está dentro das dimensões da caixa
    return std::abs(localPt.x) <= extents.x &&
           std::abs(localPt.y) <= extents.y &&
           std::abs(localPt.z) <= extents.z;
}

bool Box3d::rayCast(RayInfo* info) const {
    // Converte a origem e a direção do raio para o espaço local da caixa
    vector3d localOrigin = transform->toLocal(info->origin);
    vector3d localDir = transform->toLocal(info->dir);

    // Inicializa os pontos de entrada e saída
    decimal tMin = -std::numeric_limits<decimal>::max();
    decimal tMax = std::numeric_limits<decimal>::max();

    // Testa interseção com cada par de planos da caixa
    for (int i = 0; i < 3; i++) {
        if (std::abs(localDir[i]) < 1e-6) {
            // Raio paralelo aos planos
            if (localOrigin[i] < -extents[i] || localOrigin[i] > extents[i]) {
                return false; // Nenhuma interseção
            }
        } else {
            // Calcula interseções com os planos
            decimal t1 = (-extents[i] - localOrigin[i]) / localDir[i];
            decimal t2 = (extents[i] - localOrigin[i]) / localDir[i];
            if (t1 > t2) std::swap(t1, t2);

            // Atualiza os pontos de entrada e saída
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            // Verifica se não há interseção
            if (tMin > tMax) {
                return false;
            }
        }
    }

    // Preenche os dados de interseção
    info->enterDist = tMin;
    info->enterPoint = info->origin + info->dir * tMin;
    info->outDist = tMax;
    info->outPoint = info->origin + info->dir * tMax;
    info->colliding = true;

    return true;
}

void Box3d::getAABB(AABB* ab) const {
    // Obtém os eixos da caixa no espaço global
    matrix3x3 rotation = transform->getOrientation().toMatrix3x3();
    vector3d axisX = rotation * vector3d(extents.x, 0.0f, 0.0f);
    vector3d axisY = rotation * vector3d(0.0f, extents.y, 0.0f);
    vector3d axisZ = rotation * vector3d(0.0f, 0.0f, extents.z);

    // Calcula os cantos da AABB no espaço global
    vector3d center = transform->getPosition();
ab->min = center - (axisX + axisY + axisZ);
ab->max = center + (axisX + axisY + axisZ);
}

decimal Box3d::getVolume() const {
    return 8.0f * extents.x * extents.y * extents.z;
}

decimal Box3d::getSurfaceArea() const {
    return 8.0f * (extents.x * extents.y + extents.y * extents.z + extents.z * extents.x);
}

matrix3x3 Box3d::getInertiaTensor(decimal mass) const {
    decimal Ixx = (1.0f / 12.0f) * mass * (4.0f * extents.y * extents.y + 4.0f * extents.z * extents.z);
    decimal Iyy = (1.0f / 12.0f) * mass * (4.0f * extents.x * extents.x + 4.0f * extents.z * extents.z);
    decimal Izz = (1.0f / 12.0f) * mass * (4.0f * extents.x * extents.x + 4.0f * extents.y * extents.y);
    return matrix3x3().setDiagonal({Ixx, Iyy, Izz});
}

void Box3d::getVertices(vector<vector3d>& vertices)const {
        // Obtém a matriz de transformação global
        matrix4x4 worldMatrix = transform->getWorldTransformMatrix();

        // Define os 8 vértices locais da caixa (não transformados)
vertices = {
            {-extents.x, -extents.y, -extents.z},
            { extents.x, -extents.y, -extents.z},
            {-extents.x,  extents.y, -extents.z},
            { extents.x,  extents.y, -extents.z},
            {-extents.x, -extents.y,  extents.z},
            { extents.x, -extents.y,  extents.z},
            {-extents.x,  extents.y,  extents.z},
            { extents.x,  extents.y,  extents.z}
        };

        // Transforma os vértices locais para o espaço global
        for (uint32 i=0; i<vertices.size(); i++){
vertices[i]=worldMatrix * vertices[i];
        }
}
} // namespace gpp
