

#ifndef GPP_ZONE_H
#define GPP_ZONE_H


namespace gpp
{

class gpp_zone : public RigidBody
{
public:
gpp_ground ground;
gpp_zone* parent;
std::vector<gpp_zone*> childs;
std::vector<RigidBody*> bodies;
gpp_zone();
gpp_zone(const gpp_zone& gz)=delete;
gpp_zone& operator=(const gpp_zone& gz)=delete;
virtual ~gpp_zone();
void setGround(const gpp_ground& f);
gpp_ground getGround()const;
virtual std::string toString()const;
virtual void translate(const vector3d& v);
void setGroundType(uint32 type);
gpp_zone* getParent()const;
void setParent(gpp_zone* z);
bool addChild(gpp_zone* a);
bool removeChild(const gpp_index& id);
gpp_zone* getChild(const gpp_index& id);
bool addBody(RigidBody* b);
bool removeBody(const gpp_index& id);
};
}

#endif
