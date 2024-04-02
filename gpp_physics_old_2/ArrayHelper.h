


#ifndef ARRAYHELPER_H
#define ARRAYHELPER_H

namespace gpp{

inline bool remove_from_array(std::vector<RigidBody*>& hbodies, RigidBody* rb){
auto it=std::find_if(hbodies.begin(), hbodies.end(), [&](RigidBody* r)->bool{return r->id==rb->id;});
if(it!=hbodies.end()){
hbodies.erase(it);
return true;
}
return false;
}

inline bool remove_from_array(std::vector<RigidBody*>& hbodies, uint32 id){
auto it=std::find_if(hbodies.begin(), hbodies.end(), [&](RigidBody* r)->bool{return r->id==id;});
if(it!=hbodies.end()){
hbodies.erase(it);
return true;
}
return false;
}

inline RigidBody* find_from_array(const std::vector<RigidBody*>& hbodies, RigidBody* rb){
auto it=std::find_if(hbodies.begin(), hbodies.end(), [&](RigidBody* r)->bool{return r->id==rb->id;});
return ((it==hbodies.end()) ? NULL : *it);
}

inline RigidBody* find_from_array(const std::vector<RigidBody*>& hbodies, uint32 id){
auto it=std::find_if(hbodies.begin(), hbodies.end(), [&](RigidBody* r)->bool{return r->id==id;});
return ((it==hbodies.end()) ? NULL : *it);
}

inline void free_array_object(std::vector<RigidBody*>& hbodies){
for(auto& it : hbodies){
delete it;
}
hbodies.clear();
}
}
#endif
