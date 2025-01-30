

#ifndef MAPCHUNK_H
#define MAPCHUNK_H

class MapChunk{
public:
uint32 id;

vector3d min;
vector3d max;

std::string floor;
std::string name;
std::string shortdesc;
std::string longdesc;

MapChunk* hparent;
std::vector<MapChunk*> childs;
std::unordered_set<MapChunk*> neighbors;

MapChunk();
MapChunk(const MapChunk& mp)=delete;
MapChunk& operator=(const MapChunk& mp)=delete;
virtual ~MapChunk();

std::string toString()const;
void toDefault();

std::string getFullLocationString()const;
void getfullLocation(std::vector<MapChunk*>& chunks);

void pushChunk(MapChunk* hchunk);
void removeChunk(MapChunk* hchunk);
void pushNeighbor(MapChunk* hchunk);
void removeNeighbor(MapChunk* hchunk);

bool canContains(MapChunk* hchunk);
bool isColliding(MapChunk* hchunk);
void rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos, bool multilevel=false);
};
#endif
