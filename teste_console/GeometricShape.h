
#ifndef GEOMETRICSHAPE_H
#define GEOMETRICSHAPE_H


namespace gpp {

enum GEOMETRICTYPE {
    GTYPE_NONE ,
    GTYPE_SPHERE = (1 << 0),
    GTYPE_BOX = (1 << 1),
    GTYPE_CAPSULE = (1 << 2),
    GTYPE_CYLINDER = (1 << 3),
    GTYPE_POLYEDRON = (1 << 4)
};

class GeometricShape {
protected:
    uint32 type;          // Tipo da forma geom�trica
    Transform transform;  // Transforma��o associada � forma (composi��o)

public:
    GeometricShape(uint32 type, const Transform& transform = Transform());
    virtual ~GeometricShape() = default;

    uint32 getType() const { return type; }  // Retorna o tipo da forma
    // M�todos para manipula��o da transforma��o
    void setTransform(const Transform& transform){this->transform=transform;}
    const Transform& getTransform() const{return this->transform;}
    Transform& getTransform(){return this->transform;}

    // M�todos para manipula��o da forma geom�trica
    virtual std::string toString() const;  // Descri��o textual da forma
    virtual vector3d getClosestPoint(const vector3d& pt) const{return transform.getPosition();}//Ponto mais pr�ximo
    virtual bool contains(const vector3d& pt) const{return false;}  // Verifica se o ponto est� dentro
    virtual bool rayCast(RayInfo* info) const{return false;}  // Interse��o com raio
    virtual AABB getAABB() const{return AABB();}  // Retorna a AABB da forma
    virtual decimal getVolume() const{return 0.0f;}  // Volume da forma
    virtual decimal getSurfaceArea() const{return 0.0f;}  // �rea da superf�cie
    virtual matrix3x3 getInertiaTensor(decimal mass) const{return matrix3x3().identity();}  // Tensor de in�rcia (com massa)

    // M�todos para transforma��o da forma
    void translate(const vector3d& translation);  // Move a forma no espa�o
void rotate(decimal x, decimal y, decimal z);
    void rotate(const quaternion& rotation);      // Rotaciona a forma
    void scale(const vector3d& scaling);          // Escala a forma
};

} // namespace gpp

#endif // GEOMETRICSHAPE_H
