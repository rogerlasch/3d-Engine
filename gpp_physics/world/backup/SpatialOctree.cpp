
#include <sstream>
#include<stack>
#include <execution>
#include "gpp_world.h"

using namespace std;

namespace gpp {

SpatialOctree::SpatialOctree(const AABB& bounds, uint32 maxDepth, uint32 treshold) {
this->maxDepth=maxDepth;
    this->treshold = treshold;
    root = new OctreeNode();
    root->bounds = bounds;
hprofiler=new ProfilerManager(PF_NANO, "octree_profiler.txt");
}

SpatialOctree::~SpatialOctree() {
    cleanup();

    if (root) {
        delete root;
        root = nullptr;
    }

delete hprofiler;
}

string SpatialOctree::toString() const {
    stringstream ss;
    ss << fixed;
    ss.precision(2);
    ss << "Octree" << endl;
    ss << "MaxDepth=" << maxDepth << ", Treshold=" << treshold << endl;
    ss << root->toString() << endl;

    return ss.str();
}

void SpatialOctree::cleanup() {
    if (root) {
        clearNode(root);
    }
}

void SpatialOctree::insert(RigidBody* rb) {
ProfilerObject _OBJ(__FUNCTION__, hprofiler);
AABB ab;
rb->getAABB(&ab);
    insertInternal(rb, ab, root, 0);
}

void SpatialOctree::remove(RigidBody* rb) {
ProfilerObject _OBJ(__FUNCTION__, hprofiler);

uint32 index=0;

    if (getNodeIndex(rb, &index)){
objectsNodeMap[index].second->bodies.erase(std::remove(objectsNodeMap[index].second->bodies.begin(), objectsNodeMap[index].second->bodies.end(), rb), objectsNodeMap[index].second->bodies.end());
        objectsNodeMap.erase(objectsNodeMap.begin()+index);
    }
}

void SpatialOctree::update(RigidBody* rb) {
ProfilerObject _OBJ(__FUNCTION__, hprofiler);

uint32 index=0;

if(!getNodeIndex(rb, &index)){
insert(rb);
}
else{
AABB ab;
rb->getAABB(&ab);
OctreeNode* hnode=objectsNodeMap[index].second;
if(hnode->bounds.contains(ab)){
insertInternal(rb, ab, hnode, hnode->depth);
}
else{
while(hnode!=NULL){
hnode=hnode->hparent;

if(hnode->bounds.contains(ab)){
insertInternal(rb, ab, hnode, hnode->depth);
break;
}
}
}
}
}

void SpatialOctree::rayCast(const vector3d& origin, const vector3d& dir, vector<RayInfo>& infos) {
    ProfilerObject _OBJ(__FUNCTION__, hprofiler);

    // Configuração inicial do raio
    RayInfo ray;
    ray.origin = origin;
    ray.dir = dir;
    ray.colliding = false;

    // Pré-computações para o ray-AABB
    vector3d invDir(1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z);
    int32 dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };

    // Stack para processamento iterativo
    struct StackItem {
        OctreeNode* node;
        decimal tMin;
        decimal tMax;

        // Operador para ordenação (menor tMin primeiro)
        bool operator<(const StackItem& other) const {
            return tMin > other.tMin; // Ordena em ordem decrescente para pop do menor
        }
    };

    std::stack<StackItem> stack;
    std::priority_queue<StackItem> queue; // Para ordenação front-to-back

    // Inicia com o nó raiz
    decimal tMin, tMax;
    if (root->bounds.computedRayCast(origin, invDir, dirIsNeg, tMin, tMax)) {
        queue.push({root, tMin, tMax});
    }

    // Processamento iterativo
    while (!queue.empty()) {
        StackItem item = queue.top();
        queue.pop();
        OctreeNode* node = item.node;

        // Teste contra todos os objetos no nó atual
        for (RigidBody* body : node->bodies) {
            RayInfo tempRay = ray;
            if (body->rayCast(&tempRay)) {
                infos.emplace_back(tempRay);
            }
        }

        // Processa filhos que intersectam o raio
        if (!node->childs.empty()) {
            for (OctreeNode* child : node->childs) {
                if (!child) continue;

                decimal ctMin, ctMax;
                if (child->bounds.computedRayCast(origin, invDir, dirIsNeg, ctMin, ctMax)) {
                    queue.push({child, ctMin, ctMax});
                }
            }
        }
    }
}

void SpatialOctree::broadPhase(vector<pair<RigidBody*, RigidBody*>>& pairs) {
    ProfilerObject _OBJ(__FUNCTION__, hprofiler);

    mutex pairsMutex;
    for_each(execution::par, objectsNodeMap.begin(), objectsNodeMap.end(),
        [&](const pair<RigidBody*, OctreeNode*>& hpair) {
            AABB ab;
            hpair.first->getAABB(&ab);
            vector<pair<RigidBody*, RigidBody*>> localPairs;

            // Versão iterativa
            stack<OctreeNode*> nodeStack;
            nodeStack.push(hpair.second);

            while (!nodeStack.empty()) {
                OctreeNode* current = nodeStack.top();
                nodeStack.pop();

                // Processa corpos no nó atual
                for (RigidBody* rb2 : current->bodies) {
                    if (rb2 == hpair.first) continue;
if((hpair.first->hasFlag(BF_STATIC))&&(rb2->hasFlag(BF_STATIC)))continue;

                    AABB ab2;
                    rb2->getAABB(&ab2);

                    if (ab.intersects(ab2)) {
                        localPairs.emplace_back(hpair.first, rb2);
                    }
                }

                // Adiciona filhos para processamento
                for (OctreeNode* child : current->childs) {
                    if (child && child->bounds.intersects(ab)) {
                        nodeStack.push(child);
                    }
                }
            }

            if (!localPairs.empty()) {
                lock_guard<mutex> lck(pairsMutex);
                pairs.insert(pairs.end(), localPairs.begin(), localPairs.end());
            }
        });
}

// Métodos internos

void SpatialOctree::clearNode(OctreeNode* hnode) {
ProfilerObject _OBJ(__FUNCTION__, hprofiler);
    hnode->bodies.clear();
    hnode->hparent = nullptr;

    for (auto& child : hnode->childs) {
        if (child) {
            clearNode(child);
            delete child;
            child = nullptr;
        }
    }

    hnode->childs.clear();
}

bool SpatialOctree::insertInternal(RigidBody* rb, const AABB& ab, OctreeNode* startNode, uint32 startDepth) {
    ProfilerObject _OBJ(__FUNCTION__, hprofiler);

    struct StackFrame {
        OctreeNode* node;
        uint32 depth;
    };

    stack<StackFrame> stack;
    stack.push({startNode, startDepth});

    while (!stack.empty()) {
        StackFrame frame = stack.top();
        stack.pop();

        if (!frame.node->bounds.contains(ab)) {
            continue;
        }

        // Tenta inserir nos filhos primeiro
        bool insertedInChild = false;
        if (!frame.node->childs.empty()) {
            for (OctreeNode* child : frame.node->childs) {
                if (child->bounds.contains(ab)) {
                    stack.push({child, frame.depth + 1});
                    insertedInChild = true;
                }
            }
            if (insertedInChild) continue;
        }

        // Insere no nó atual
        frame.node->bodies.push_back(rb);
        uint32 index = 0;

        if (!getNodeIndex(rb, &index)) {
            objectsNodeMap.emplace_back(rb, frame.node);
        } else if (objectsNodeMap[index].second != frame.node) {
            objectsNodeMap[index].second->bodies.erase(                 std::remove(objectsNodeMap[index].second->bodies.begin(),                       objectsNodeMap[index].second->bodies.end(), rb),                 objectsNodeMap[index].second->bodies.end());
            objectsNodeMap[index].second = frame.node;
        }

        // Subdivide se necessário
        if (frame.node->bodies.size() > treshold && frame.depth < maxDepth) {
            subDivide(frame.node);
            redistribute(frame.node);
        }

        return true;
    }

    return false;
}

void SpatialOctree::subDivide(OctreeNode* hnode) {

ProfilerObject _OBJ(__FUNCTION__, hprofiler);

if(hnode->childs.size()>0){
return;
}

    const vector3d center = hnode->bounds.getCenter();
    const vector3d extents = hnode->bounds.getExtents();

    hnode->childs.resize(8, nullptr);

    for (int i = 0; i < 8; ++i) {
        vector3d newMin = center;
        vector3d newMax = center;

        newMin.x += (i & 1) ? 0 : -extents.x;
        newMax.x += (i & 1) ? extents.x : 0;
        newMin.y += (i & 2) ? 0 : -extents.y;
        newMax.y += (i & 2) ? extents.y : 0;
        newMin.z += (i & 4) ? 0 : -extents.z;
        newMax.z += (i & 4) ? extents.z : 0;

        hnode->childs[i] = new OctreeNode();
        hnode->childs[i]->hparent = hnode;
        hnode->childs[i]->depth= hnode->depth+1;
        hnode->childs[i]->bounds = AABB(newMin, newMax);
    }
}

void SpatialOctree::redistribute(OctreeNode* hnode) {
ProfilerObject _OBJ(__FUNCTION__, hprofiler);
    if (hnode->childs.empty()) return;

    vector<RigidBody*> remainingBodies;
    for (auto* rb : hnode->bodies) {
        AABB ab;
        rb->getAABB(&ab);

        bool moved = false;
        for (auto* child : hnode->childs) {
            if (child->bounds.contains(ab)) {
                child->bodies.push_back(rb);
//                objectsNodeMap[rb] = child;
                moved = true;
                break;
            }
        }

        if (!moved) remainingBodies.push_back(rb);
    }

    hnode->bodies = remainingBodies;
}

bool SpatialOctree::getNodeIndex(RigidBody* rb, uint32* index) const {
    auto it = find_if(objectsNodeMap.begin(), objectsNodeMap.end(),
                           [rb](const pair<RigidBody*, OctreeNode*>& hpair) {
                               return hpair.first == rb;
                           });

if(index!=NULL){
*index=std::distance(objectsNodeMap.begin(), it);
}

return it!=objectsNodeMap.end();

}
}
