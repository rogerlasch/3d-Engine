


#ifndef OCTREEINFO_H
#define OCTREEINFO_H

#include<map>

namespace gpp{

class octreeinfo{
public:
uint32 max_depth;
uint32 blimit;
uint32 nbodies;
uint32 nodes;
octreeinfo();
octreeinfo(const octreeinfo& info);
octreeinfo& operator=(const octreeinfo& info);
virtual ~octreeinfo();
std::string toString()const;
void cleanup();
};
}
#endif
