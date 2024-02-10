


#ifndef OCTREEINFO_H
#define OCTREEINFO_H



namespace gpp{

class octreeinfo{
public:
uint32 max_depth;
uint32 blimit;
uint32 nbodies;
uint32 nodes;
std::map<uint32, uint32> blevels;
octreeinfo();
octreeinfo(const octreeinfo& info);
octreeinfo& operator=(const octreeinfo& info);
virtual ~octreeinfo();
std::string toString()const;
void cleanup();
};
}
#endif
