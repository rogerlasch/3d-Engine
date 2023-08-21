
#ifndef IRIGIDBODY_H
#define IRIGIDBODY_H

namespace gpp
{
    class iRigidBody
    {
    public:
uint32 btype;
uint32 userdata;
float mass;
std::string name;
        gpp_index index;
        AABB* aabb;
        vector3d position;
        quaternion orientation;
std::atomic<uint16> bflags;
        iRigidBody() = default;
        virtual ~iRigidBody() = default;

inline virtual void setBType(uint32 btype) {this->btype=btype;}
inline virtual uint32 getBType()const{return this->btype;}

inline virtual void setUserData(uint32 userdata){this->userdata=userdata;}
inline virtual uint32 getUserData()const{return this->userdata;}

inline virtual void setMass(float mass){this->mass=mass;}
inline virtual float getMass()const{return this->mass;}

inline virtual void setName(const std::string& name){this->name=name;}
inline virtual std::string getName()const{return this->name;}

inline         virtual void setIndex(const gpp_index& index) { this->index = index; }
inline         virtual gpp_index getIndex() const { return this->index; }

inline         virtual void setPosition(const vector3d& position) { this->position = position; }
inline         virtual vector3d getPosition() const { return this->position; }

inline         virtual void setOrientation(const quaternion& orientation) { this->orientation = orientation; }
inline         virtual quaternion getOrientation() const { return this->orientation; }

inline         virtual void setAABB(AABB* aabb) { this->aabb = aabb; }
inline         virtual AABB* getAABB() const { return this->aabb; }

virtual std::string toString()const=0;

inline void setGeometricShape(GeometricShape* sh)
{
_GASSERT(aabb!=NULL);
aabb->setGeometricShape(sh);
setPosition(sh->getCenter());
}
inline GeometricShape* getGeometricShape()const {return this->aabb->getGeometricShape();}

inline void setBodyFlag(uint16 bf)
{
if((bflags&bf)!=bf)
{
bflags|=bf;
}
}

inline void removeBodyFlag(uint16 bf)
{
if((bflags&bf)!=0)
{
bflags^=bf;
}
}

inline bool containsBodyFlag(uint16 bf)const
{
return (bflags&bf)==bf;
}
    };
} // namespace gpp

#endif
