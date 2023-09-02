


#ifndef COLLISIONCACHE_H
#define COLLISIONCACHE_H

#include<vector>
#include<unordered_map>
#include<memory>

namespace gpp{

    class gpp_index;
typedef std::shared_ptr<CollisionInfo> shared_collisioninfo;
typedef std::vector<shared_collisioninfo> CollisionRow;
typedef std::shared_ptr<CollisionRow> shared_collisionrow;
typedef std::vector<shared_collisionrow> CollisionTable;

struct PairHasher {
    template <typename T1, typename T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        std::size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);
        return seed;
    }

    template <class T>
    void hash_combine(std::size_t& seed, const T& v) const {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

class CollisionCache
{
private:
std::unordered_map<std::pair<uint32, uint32>, uint32, PairHasher> hindexs;
std::unordered_map<uint64, shared_collisioninfo> hcollisions;
CollisionTable htable;
public:
CollisionCache();
CollisionCache(const CollisionCache& gm)=delete;
CollisionCache& operator=(const CollisionCache& gm)=delete;
virtual ~CollisionCache();
std::string toString()const;
void cleanup();
void insert(const shared_collisioninfo& info);
bool removeByHash(uint64 id);
uint32 removeByHashs(const std::vector<uint64>& hashs);
bool removeByIndex(const gpp_index& id);
uint32 removeByIndexs(const std::vector<gpp_index>& ids);
bool contains(uint64 id)const;
bool contains(const gpp_index& id)const;
shared_collisioninfo get(uint64 id);
shared_collisionrow getRow(const gpp_index& id);
private:
void internalUpdateRows();
};
}
#endif
