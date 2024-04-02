

#ifndef BODYHANDLER_H
#define BODYHANDLER_H

#include<functional>

namespace gpp{

class RigidBody;
class WorldProperties;
typedef std::function<void(RigidBody*)> FN_BODY1;
typedef std::function<bool(RigidBody*, WorldProperties*)> FN_BODY2;
typedef std::function<void(RigidBody*, RigidBody*)> FN_BODY3;
typedef std::function<void(RigidBody*, decimal)> FN_BODY4;

class BodyHandler{
public:
FN_BODY2 preStep;
FN_BODY4 step;
FN_BODY1 onLoop;
FN_BODY3 beginContact;
FN_BODY3 updateContact;
FN_BODY3 endContact;

BodyHandler();
virtual ~BodyHandler();

std::string toString()const;
void cleanup();
void toDefault();
void copyFrom(BodyHandler* handler);
};
}
#endif
