

#ifndef GPP_ZONE_H
#define GPP_ZONE_H


namespace gpp
{

class gpp_zone : public RigidBody
{
public:
gpp_floor floor;
gpp_zone* parent;
std::vector<gpp_zone*> childs;
gpp_zone();
gpp_zone(const gpp_zone& gz)=delete;
gpp_zone& operator=(const gpp_zone& gz)=delete;
virtual ~gpp_zone();
void setFloor(const gpp_floor& f);
gpp_floor getFloor()const;
void addChild(gpp_zone* gz);
bool removeChild(const gpp_index& id);
gpp_zone* getChild(const gpp_index& id, uint32* index=NULL);
virtual std::string toString()const;
virtual void onStep(float dt, vector3d* onchange=NULL);
virtual void translate(const vector3d& v);
};
}

#endif
