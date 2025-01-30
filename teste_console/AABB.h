
#ifndef AABB_H
#define AABB_H


class aabb {
public:
    vector3d min;
    vector3d max;

aabb* hparent;
std::vector<aabb*> childs;

    // Construtor padrão e inicializador
    aabb(const vector3d& min = {0, 0, 0}, const vector3d& max = {0, 0, 0}) {
        this->min = min;
        this->max = max;
hparent=NULL;
    }

    // Construtor de cópia
    aabb(const aabb& ab) {
        *this = ab;
    }

    // Sobrecarga do operador de atribuição
    aabb& operator=(const aabb& ab) {
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

    // Função que calcula o volume do AABB
    decimal volume() const {
        vector3d v = max - min;
        return (v.x * v.y) * v.z;
    }

    // Zera os valores de min e max
    void zero() {
        min.zero();
        max.zero();
    }

bool contains(const vector3d& v)const{
if(v.x<min.x||v.x>max.x)return false;
if(v.y<min.y||v.y>max.y)return false;
if(v.z<min.z||v.z>max.z)return false;
return true;
}

bool contains(aabb* ab)const{
return min.x<=ab->min.x&&max.x>=ab->max.x&&min.y<=ab->min.y&&max.y>=ab->max.y&&min.z<=ab->min.z&&max.z>=ab->max.z;
}

    // Verifica se há colisão com outro AABB
inline     bool isColliding(aabb* ab) const {
if(min.x>ab->max.x||ab->max.x<min.x)return false;
if(min.y>ab->max.y||ab->max.y<min.y)return false;
if(min.z>ab->max.z||ab->max.z<min.z)return false;

return true;
    }



static aabb merge(const aabb& a, const aabb& b){
vector3d min, max;
for(uint32 i=0; i<3; i++){
min[i]=std::min(a.min[i], b.min[i]);
max[i]=std::max(a.max[i], b.max[i]);
}
return aabb(min, max);
}

bool insert(aabb* ab){

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

void detectCollision(const vector3d& v, std::vector<aabb*>& aabbs) {
    // Usamos uma pilha para armazenar os AABBs que devemos processar
    std::vector<aabb*> stack;

    // Iniciamos a pilha com este AABB
    stack.push_back(this);

    // Processamos enquanto houver elementos na pilha
    while (!stack.empty()) {
        // Extraímos o AABB do topo da pilha
        aabb* current = stack.back();
        stack.pop_back();

        // Adicionamos o AABB à lista de colisões detectadas
        aabbs.push_back(current);

        // Colocamos os filhos do AABB atual na pilha para processar
uint32 sz=current->childs.size();
        for (uint32 i=0; i<sz; i++){
//aabb* child=current->childs[i];
if(current->childs[i] ->contains(v)){
            stack.push_back(current->childs[i]);
}
        }
    }
}

void detectCollisions(aabb* ab, std::vector<aabb*>&  aabbs){
    std::vector<aabb*> stack;

    // Iniciamos a pilha com este AABB
    stack.push_back(this);

    // Processamos enquanto houver elementos na pilha
    while (!stack.empty()) {
        // Extraímos o AABB do topo da pilha
        aabb* current = stack.back();
        stack.pop_back();

        // Adicionamos o AABB à lista de colisões detectadas
        aabbs.push_back(current);

        // Colocamos os filhos do AABB atual na pilha para processar
uint32 sz=current->childs.size();
        for (uint32 i=0; i<sz; i++){
//aabb* child=current->childs[i];
if(current->childs[i] ->isColliding(ab)){
            stack.push_back(current->childs[i]);
}
        }
    }

}
};

#endif
