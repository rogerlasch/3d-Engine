


#ifndef DEFAULTFILTER_H
#define DEFAULTFILTER_H

namespace gpp {

	class DefaultFilter : public iFilter {
	public:
		virtual void filter(std::vector<iRigidBody*>& hinput, std::vector<iRigidBody*>& hout, WorldInfo* info);
		virtual void filter_and_prepare(iRigidBody* rb, std::vector<iRigidBody*>& hbodies, WorldInfo* info);
	};
}
#endif
