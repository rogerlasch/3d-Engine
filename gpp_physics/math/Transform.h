
#ifndef GPP_TRANSFORM_H
#define GPP_TRANSFORM_H

#include<unordered_set>

namespace gpp {

class Transform {
private:
    vector3d position;          // Posi��o no espa�o local
vector3d lastPosition;
    vector3d scale;             // Escala no espa�o local
    quaternion orientation;     // Orienta��o (rota��o) no espa�o local
mutable     matrix4x4 localMatrix;  // Matriz de transforma��o local
    Transform* hparent;         // Transforma��o pai (hierarquia)
    std::unordered_set<Transform*> childs; // Transforma��es filhas
mutable bool needUpdate;

public:

    // Construtores
    Transform();
    Transform(const vector3d& scale, const vector3d& angles={0,0,0}, const vector3d& translation={0,0,0});
    Transform(Transform* hparent, const vector3d& scale, const vector3d& angles={0,0,0}, const vector3d& translation={0,0,0});
    Transform(const Transform& tr);
    Transform& operator=(const Transform& tr);
~Transform();

bool getNeedUpdate()const{return this->needUpdate;}
void setNeedUpdate(bool b)const {this->needUpdate=b;}

    // Getters e Setters
    vector3d getPosition() const { return this->position; }
    void setPosition(const vector3d& pos);

    vector3d getLastPosition() const { return this->lastPosition; }
    void setLastPosition(const vector3d& pos){this->lastPosition=pos;}

    Transform* getParent() const { return this->hparent; }
    void setParent(Transform* hparent){this->hparent=hparent;}

    vector3d getScale() const { return this->scale; }
    void setScale(const vector3d& scale);

    quaternion getOrientation() const { return this->orientation; }
    void setOrientation(const quaternion& orientation);

    // Manipula��o de filhos
    void removeTransform(Transform* child);
    void pushTransform(Transform* child);

    // Obt�m a matriz de transforma��o global
    matrix4x4 getWorldTransformMatrix() const;
matrix4x4 getLocalMatrix()const;

    // Transforma um ponto do espa�o local para o espa�o global
    vector3d toGlobal(const vector3d& v) const;
vector3d toLocal(const vector3d& v)const;
void updateLocalMatrix()const;
void forceUpdateLocalMatrix(bool propagate=false);

void rotate(decimal x, decimal y, decimal z);
void rotate(const quaternion& q);
void translate(const vector3d& translation);

vector3d getDirectionMovement()const;

};
} // namespace gpp
#endif // GPP_TRANSFORM_H
