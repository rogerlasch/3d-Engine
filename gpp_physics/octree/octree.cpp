
#include<stack>
#include<string.h>
#include"../gpp_physics.h"
#include"octree.h"

using namespace std;

namespace gpp
{
octree::octree()
{
info.cleanup();
root=NULL;
}

octree::~octree()
{
if(root)
{
delete root;
}
root=NULL;
info.cleanup();
}

string octree::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<info.toString();
ss<<"Limites: ";
vector3d m1=root->center-root->radius;
vector3d m2=root->center+root->radius;
for(uint32 i=0; i<3; i++)
{
ss<<m1[i]<<":"<<m2[i]<<", ";
}
//ss << root->toString();
return ss.str();
}

void octree::create(uint32 max_depth, uint32 blimit, float alph)
{
    _GASSERT(root == NULL);
profiler_snap();
info.blimit = blimit;
info.max_depth = max_depth;
this->root = new octreenode();
alph = alph * 0.5f;
root->center = vector3d(0, 0, 0) + alph;
root->radius = alph;
info.nodes+=1;
vector<octreenode*> pstack;
pstack.push_back(root);
uint32 depth=6;
while(pstack.size()>0)
{
octreenode* node=pstack.back();
pstack.pop_back();
if(node->level>=depth)continue;
uint32 size=node->childs.size();
if(size==0){
splitNode(node);
}
for(uint32 i=0; i<8; i++)
{
if(node->childs[i]->level<depth)
{
pstack.push_back(node->childs[i]);
}
}
}
}

void octree::clear()
{
profiler_snap();
if(root){
delete root;
}
root = NULL;
info.cleanup();
}

void octree::insert(iRigidBody* rb){
    _GASSERT(rb != NULL);
    profiler_snap();
    octreenode* node = getDeepest(rb);
    _GASSERT(node != NULL);
    if (node->bodies.size() >= info.blimit) {
        splitAndRedistribute(node);
        node = getDeepest(rb, node);
        while (node != NULL) {
            if (node->bodies.size() < info.blimit) {
                moveObject(NULL, node, rb);
                return;
            }
            else {
                node = node->parent;
            }
        }
        string msg = safe_format("A árvore está completamente cheia e não conseguiu encontrar um espaço para o objeto:\n\"{}\"", rb->toString());
        _GINFO("{}", toString());
        _GASSERT_MSG(node != NULL, msg);
    }
    else {
        moveObject(NULL, node, rb);
    }
}

void octree::remove(iRigidBody* rb)
{
    profiler_snap();
}

void octree::BroadPhase(vector<iRigidBody*>& hbodies, vector<CollisionInfo>& collisions)
{
    profiler_snap();
    uint32 size = hbodies.size();
    for (uint32 i = 0; i < size; i++) {
        broadPhase(hbodies[i], collisions);
    }
}

void octree::broadPhase(iRigidBody* rb, vector<CollisionInfo>& collisions) {
    profiler_snap();
stack<octreenode*> pstack;
pstack.push(root);
while(pstack.size()>0){
octreenode* hnode=pstack.top();
pstack.pop();
uint32 size = hnode->bodies.size();
for (uint32 i = 0; i < size; i++) {
    if (aabbOverlap(rb->aabb, hnode->bodies[i]->aabb)) {
        auto& it = collisions.emplace_back();
        it.r1 = rb;
        it.r2 = hnode->bodies[i];
    }
}
size=hnode->childs.size();
for(uint32 i=0; i<size; i++)
{
if(aabbOverlap(hnode->childs[i]->center, hnode->childs[i]->radius, rb->aabb))
{
pstack.push(hnode->childs[i]);
}
}
}
}

octreenode* octree::getDeepest(iRigidBody* rb, octreenode* node)
{
    profiler_snap();
    octreenode* currentnode = ((node==NULL) ? root : node);
    uint32 iterations=info.max_depth+1;
    uint32 x = 0;
    while (x<iterations) {
        x++;
        if (currentnode->childs.size() == 0) {
            return currentnode;
        }
        uint32 size = currentnode->childs.size();
        bool done = true;
        for (uint32 i = 0; i < size; i++) {
            if (aabbInsideAll(currentnode->childs[i]->center, currentnode->childs[i]->radius, rb->aabb)) {
                currentnode = currentnode->childs[i];
                done = false;
                break;
            }
        }
        if (done)break;
    }
    _GASSERT_MSG(x < iterations, "O número máximo de iterações foi atingido. Possível loop infinito.");
    return currentnode;
}

void octree::splitAndRedistribute(octreenode* node) {
    profiler_snap();
    if (node->childs.size() == 0) {
        splitNode(node);
    }
    redistributeNode(node);
    computeInfos();
}

    void octree::splitNode(octreenode * node) {
        _GASSERT_MSG(node->childs.size() == 0, "Tentando dividir um nó que já foi dividido...");
        _GASSERT_MSG(node->level < info.max_depth, "A profundidade máxima neste ramo já foi atingida! Nível: {}, Max_depth:{}", node->level, info.max_depth);
        float step = node->radius * 0.5f;
        for (uint32 i = 0; i < 8; i++)
        {
            octreenode* sf = new octreenode();
            sf->level = node->level + 1;
            sf->center.x = ((i & 1) ? step : -step);
            sf->center.y = ((i & 2) ? step : -step);
            sf->center.z = ((i & 4) ? step : -step);
            sf->center += node->center;
            sf->radius = step;
            sf->parent = node;
            node->childs.push_back(sf);
        }
        info.nodes += 8;
        info.blevels[node->childs[0]->level] = 0;
    }

    void octree::redistributeNode(octreenode * node) {
        profiler_snap();
        vector<iRigidBody*> hbodies;
        extractObjectsNode(node, hbodies);
        function<bool(iRigidBody*, iRigidBody*)> hsort = [](iRigidBody* r1, iRigidBody* r2)->bool {
            return r1->aabb->getVolume() < r2->aabb->getVolume();
        };
        std::sort(hbodies.begin(), hbodies.end(), hsort);
        uint32 size = hbodies.size();
        for (uint32 i = 0; i < size; i++) {
            octreenode* hnode = getDeepest(hbodies[i], node);
            if (hnode->childs.size() == 0) {
                splitNode(hnode);
                hnode = getDeepest(hbodies[i], hnode);
            }
            while (hnode != NULL) {
                if (hnode->bodies.size() < info.blimit) {
                    moveObject(NULL, hnode, hbodies[i]);
                    break;
                }
                else {
                    hnode = hnode->parent;
                }
            }
            _GASSERT_MSG(hnode != NULL, "Nenhum espaço localizado para adicionar o objeto. Isto definitivamente não deveria ter acontecido.");
        }
    }

void octree::moveObject(octreenode* from, octreenode* to, iRigidBody* rb){
    _GASSERT(to != NULL);
    _GASSERT(rb != NULL);
    if (from == NULL)
    {
        BinaryUtils::insert(to->bodies, rb);
        info.nbodies += 1;
        info.blevels[to->level] += 1;
    }
    else {
        iRigidBody* r = NULL;
        BinaryUtils::remove(from->bodies, rb->index, &r);
        BinaryUtils::insert(to->bodies, rb);
        info.blevels[to->level] -= 1;
        info.blevels[to->level] += 1;
    }
}

void octree::extractObjectsNode(octreenode* node, vector<iRigidBody*>& hbodies) {
    profiler_snap();
    hbodies.clear();
    function<void(octreenode*)> hfunc = [&](octreenode* hnode) {
        if (hnode->bodies.size() > 0) {
            hbodies.insert(hbodies.end(), hnode->bodies.begin(), hnode->bodies.end());
            hnode->bodies.clear();
        }
        if (hnode->childs.size() > 0) {
            for (uint32 i = 0; i < 8; i++) {
                hfunc(hnode->childs[i]);
            }
        }
    };
    hfunc(node);
}

void octree::computeInfos() {
    profiler_snap();
    info.blevels.clear();
    info.nbodies = 0;
    function<void(octreenode*)> hfunc = [&](octreenode* node) {
        info.blevels[node->level] += node->bodies.size();
        info.nbodies += node->bodies.size();
        if (node->childs.size() > 0){
            for (uint32 i = 0; i < 8; i++) {
                hfunc(node->childs[i]);
            }
        }
    };
    hfunc(root);
}
}
