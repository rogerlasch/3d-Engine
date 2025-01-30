

#ifndef OBB_H
#define OBB_H

#include"Simplex.h"
#include"Triangle.h"
#include"Polyedron.h"

class obb{
public:

vector3d center;
vector3d alf;
quaternion orientation;
std::array<vector3d, 3> axis;

//Informa��es de colis�o.
vector3d normal;
vector3d contactPoint;
decimal depth;

obb(){
}
obb(const vector3d& alf, const quaternion& orientation){
this->alf=alf;
this->orientation=orientation;
for(uint32 i=0; i<3; i++){
vector3d v;
v[i]=alf[i];
axis[i]=quaternion_vector_rotate(orientation, v);
}
}
obb(const obb& b)=delete;
obb& operator=(const obb& b)=delete;

string toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"pos="<<center<<endl;
ss<<"alf="<<alf<<endl;
ss<<"qt="<<orientation<<endl;
ss<<"euler="<<quaternion_extract_euler_angles(orientation)<<endl<<"{"<<endl;
for(auto& it : axis){
ss<<it<<endl;
}
ss<<"}"<<endl;

vector<vector3d> vertices=getVertices();
ss<<"V�rtices={"<<endl;
for(auto& it : vertices){
ss<<it<<endl;
}
ss<<"}"<<endl;
return ss.str();
}

void translate(const vector3d& v){
center+=v;
}

void rotate(const vector3d& v, const quaternion& orientation){
this->orientation=this->orientation*orientation;
this->orientation.normalize();

center=quaternion_vector_rotate(orientation, (center-v))+v;

for(uint32 i=0; i<3; i++){
axis[i]=quaternion_vector_rotate(orientation, axis[i]);
}
}

vector<vector3d> getVertices()const{
vector<vector3d> vertices={
            center - axis[0] - axis[1] - axis[2],
            center + axis[0] - axis[1] - axis[2],
            center - axis[0] + axis[1] - axis[2],
            center + axis[0] + axis[1] - axis[2],
            center - axis[0] - axis[1] + axis[2],
            center + axis[0] - axis[1] + axis[2],
            center - axis[0] + axis[1] + axis[2],
            center + axis[0] + axis[1] + axis[2]
};
return vertices;
}

    vector3d find_support_point(const vector3d& direction) const {
        if (direction == vector3d::ORIGIN) {
            throw std::invalid_argument("Direction is null vector");
        }

vector<vector3d> vertices=getVertices();

        // Procura o ponto mais distante em uma dire��o espec�fica
        vector3d best;
        decimal max_dot = -std::numeric_limits<decimal>::infinity();

        for (const auto& vertex : vertices) {
            decimal dot = vertex.dot(direction);
            if (dot > max_dot) {
                best = vertex;
                max_dot = dot;
            }
        }

        return best;
    }

bool isColliding(obb& b2){
    Simplex smpx;
    // Gerar o primeiro vetor de dire��o.
    vector3d dir = b2.center - this->center;
    if (dir == vector3d::ORIGIN) {
        dir = vector3d(0.0, 0.0, 1.0); // Define uma dire��o arbitr�ria
    }

    // Verifica se as formas est�o se intersectando usando o algoritmo GJK.
    while (true) {
        vector3d point0 = this->find_support_point(-dir);
        vector3d point1 = b2.find_support_point(dir);
        vector3d diff = point1 - point0;

        // Se o v�rtice n�o puder ser adicionado, n�o h� como continuar.
        if (!smpx.pushVertex(diff)) {
return false;
        }

        // Se o simplex cont�m a origem, os s�lidos est�o sobrepostos.
        if (smpx.containsOrigin()) {
            break;
        }

        dir = smpx.suggestDirection();
        // Se nenhuma dire��o for sugerida, mira na diferen�a anteriormente definida.
        if (dir==vector3d::ORIGIN) {
            dir = diff;
        }
    }

// computa normal de colis�o, ponto de contato e profundidade usando EPA
    // Computa o vetor de penetra��o m�nima usando EPA.
    Polyhedron pldr(smpx);
    while (true) {
        dir = pldr.suggestDirection();
        vector3d point0 = this->find_support_point(-dir);
        vector3d point1 = b2.find_support_point(dir);
        vector3d diff = point1 - point0;

        if (!pldr.add_vertex(diff)) {
contactPoint=pldr.compute_origin_translation();  // Retorna o vetor de penetra��o
return true;
        }
    }

return true;
}
};
#endif
