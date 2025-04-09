
#include <sstream>
#include<stack>
#include <execution>
#include "gpp_world.h"

using namespace std;

namespace gpp {

SpatialOctree::SpatialOctree(const AABB& bounds, uint32 maxDepth, uint32 threshold) {
this->maxDepth=maxDepth;
    this->threshold = threshold;
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
    ss << "MaxDepth=" << maxDepth << ", Treshold=" << threshold << endl;
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

    auto it = objectsNodeMap.find(rb);
    if (it != objectsNodeMap.end()) {
        it->second->bodies.unsafe_erase(rb);
        objectsNodeMap.unsafe_erase(it);
    }
}

void SpatialOctree::removePendingObjects(){
for(auto& it : removeObjectsNode){
it.second->bodies.unsafe_erase(it.first);
}

removeObjectsNode.clear();
}

void SpatialOctree::update(RigidBody* rb) {
//ProfilerObject _OBJ(__FUNCTION__, hprofiler);
    auto it = objectsNodeMap.find(rb);
    if (it == objectsNodeMap.end()) {
        insert(rb);
        return;
    }

    AABB ab;
    rb->getAABB(&ab);
    OctreeNode* hnode = it->second;

    if (hnode->bounds.contains(ab)) {
if(hnode!=it->second){
        insertInternal(rb, ab, hnode, hnode->depth);
}
    } else {
        while (hnode != nullptr) {
            hnode = hnode->hparent;
            if (hnode && hnode->bounds.contains(ab)) {
                insertInternal(rb, ab, hnode, hnode->depth);
                break;
            }
        }
    }
}

void SpatialOctree::rayCast(const vector3d& origin, const vector3d& dir, tbb::concurrent_vector<RayInfo>& infos) {
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

void SpatialOctree::broadPhase(tbb::concurrent_vector<pair<RigidBody*, RigidBody*>>& pairs) {
    ProfilerObject _OBJ(__FUNCTION__, hprofiler);

    tbb::parallel_for_each(objectsNodeMap.begin(), objectsNodeMap.end(), [&](const std::pair<RigidBody*, OctreeNode*>& hpair) {
        AABB ab;
        hpair.first->getAABB(&ab);

        // Versão iterativa
        stack<OctreeNode*> nodeStack;
        nodeStack.push(hpair.second);

        while (!nodeStack.empty()) {
            OctreeNode* current = nodeStack.top();
            nodeStack.pop();

            // Processa corpos no nó atual
for_each(current->bodies.find(hpair.first), current->bodies.end(), [&](RigidBody* rb2) {

                if (rb2 == hpair.first){
 return;
}
                if ((hpair.first->hasFlag(BF_STATIC)) && (rb2->hasFlag(BF_STATIC))) return;

                AABB ab2;
                rb2->getAABB(&ab2);

                if (ab.intersects(ab2)) {

pairs.emplace_back(hpair.first, rb2);
                }
            });

            // Adiciona filhos para processamento
            for (OctreeNode* child : current->childs) {
                if (child && child->bounds.intersects(ab)) {
                    nodeStack.push(child);
                }
            }
        }
    });
}

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
//    ProfilerObject _OBJ(__FUNCTION__, hprofiler);

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
        frame.node->bodies.insert(rb);

if(objectsNodeMap.find(rb)==objectsNodeMap.end()){
    objectsNodeMap.insert(make_pair(rb,  frame.node));
}
else{
auto it=objectsNodeMap.find(rb);
removeObjectsNode.emplace_back(it->first,it->second);
    objectsNodeMap.insert(make_pair(rb,  frame.node));
}

        // Subdivide se necessário
        if (frame.node->bodies.size() > threshold && frame.depth < maxDepth) {
            subDivide(frame.node);
            redistribute(frame.node);
        }

        return true;
    }

    return false;
}

void SpatialOctree::subDivide(OctreeNode* hnode) {

//ProfilerObject _OBJ(__FUNCTION__, hprofiler);

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
//ProfilerObject _OBJ(__FUNCTION__, hprofiler);
    if (hnode->childs.empty()) return;

    tbb::concurrent_set<RigidBody*> remainingBodies;
    for (auto* rb : hnode->bodies) {
        AABB ab;
        rb->getAABB(&ab);

        bool moved = false;
        for (auto* child : hnode->childs) {
            if (child->bounds.contains(ab)) {
                child->bodies.insert(rb);
                objectsNodeMap.insert(make_pair(rb, child));
                moved = true;
                break;
            }
        }

        if (!moved) remainingBodies.insert(rb);
    }

    hnode->bodies = remainingBodies;
}
}
