
#ifndef BVH_H
#define BVH_H


class bvh {
public:
    vector3d min;
    vector3d max;

bvh* hparent;
std::vector<bvh*> childs;

    // Construtor padrão e inicializador
    bvh(const vector3d& min = {0, 0, 0}, const vector3d& max = {0, 0, 0}) {
        this->min = min;
        this->max = max;
hparent=NULL;
    }

    // Construtor de cópia
    bvh(const bvh& ab) {
        *this = ab;
    }

    // Sobrecarga do operador de atribuição
    bvh& operator=(const bvh& ab) {
        if (this != &ab) {
            this->min = ab.min;
            this->max = ab.max;
        }
        return *this;
    }

    // Função para exibir os valores de min e max como string formatada
    std::string toString() const {
        stringstream ss;
        ss << fixed;
        ss.precision(2);

        string axis = "XYZ";
        for (uint32_t i = 0; i < 3; i++) {
            ss << axis[i] << "=" << min[i] << ", " << max[i] << endl;
        }

ss<<"Total de filhos: "<<childs.size()<<endl;
for(auto& it : childs){
ss<<it->toString()<<endl;
}
        return ss.str();
    }

    // Função que calcula o volume do BVH
    decimal volume() const {
        vector3d v = max - min;
        return (v.x * v.y) * v.z;
    }

    // Zera os valores de min e max
    void zero() {
        min.zero();
        max.zero();
    }

vector3d getCenter()const{
return (min+max)*0.5f;
}

bool contains(const vector3d& v)const{
if(v.x<min.x||v.x>max.x)return false;
if(v.y<min.y||v.y>max.y)return false;
if(v.z<min.z||v.z>max.z)return false;
return true;
}

bool contains(bvh* ab)const{
return min.x<=ab->min.x&&max.x>=ab->max.x&&min.y<=ab->min.y&&max.y>=ab->max.y&&min.z<=ab->min.z&&max.z>=ab->max.z;
}

    // Verifica se há colisão com outro BVH
inline     bool isColliding(bvh* ab) const {
if(min.x>ab->max.x||ab->max.x<min.x)return false;
if(min.y>ab->max.y||ab->max.y<min.y)return false;
if(min.z>ab->max.z||ab->max.z<min.z)return false;

return true;
    }



static bvh merge(const bvh& a, const bvh& b){
vector3d min, max;
for(uint32 i=0; i<3; i++){
min[i]=std::min(a.min[i], b.min[i]);
max[i]=std::max(a.max[i], b.max[i]);
}
return bvh(min, max);
}

void remove(bvh* ab){
auto it=std::find(childs.begin(), childs.end(), ab);
if(it!=childs.end()){
childs.erase(it);
}
}

bool insert(bvh* ab){

if(!contains(ab)){
return false;
}

for(auto& it : childs){
if(it->insert(ab)){
return true;
}
}

ab->hparent=this;
childs.push_back(ab);
return true;
}

void detectCollision(const vector3d& v, std::vector<bvh*>& bvhs) {
    // Usamos uma pilha para armazenar os BVHs que devemos processar
    std::vector<bvh*> stack;

    // Iniciamos a pilha com este BVH
    stack.push_back(this);

    // Processamos enquanto houver elementos na pilha
    while (!stack.empty()) {
        // Extraímos o BVH do topo da pilha
        bvh* current = stack.back();
        stack.pop_back();

        // Adicionamos o BVH à lista de colisões detectadas
        bvhs.push_back(current);

        // Colocamos os filhos do BVH atual na pilha para processar
uint32 sz=current->childs.size();
        for (uint32 i=0; i<sz; i++){
//bvh* child=current->childs[i];
if(current->childs[i] ->contains(v)){
            stack.push_back(current->childs[i]);
}
        }
    }
}

void detectCollisions(bvh* ab, std::vector<bvh*>&  bvhs){
    std::vector<bvh*> stack;

    // Iniciamos a pilha com este BVH
    stack.push_back(this);

    // Processamos enquanto houver elementos na pilha
    while (!stack.empty()) {
        // Extraímos o BVH do topo da pilha
        bvh* current = stack.back();
        stack.pop_back();

        // Adicionamos o BVH à lista de colisões detectadas
        bvhs.push_back(current);

        // Colocamos os filhos do BVH atual na pilha para processar
uint32 sz=current->childs.size();
        for (uint32 i=0; i<sz; i++){
//bvh* child=current->childs[i];
if(current->childs[i] ->isColliding(ab)){
            stack.push_back(current->childs[i]);
}
        }
    }
}

bool rayCast(const vector3d& origin, const vector3d& dir, RayInfo* info){
return rayBox(origin, dir, min, max, info);
}

void rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos){
RayInfo info;
if(!rayBox(origin, dir, min, max, &info)){
return;
}

infos.push_back(info);
for(auto& it : childs){
it->rayCast(origin, dir, infos);
}
}
};

#endif
