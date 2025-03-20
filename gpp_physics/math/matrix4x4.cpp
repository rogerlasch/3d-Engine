
#include<numeric>
#include<math.h>
#include <algorithm>
#include<functional>
#include"math.h"

using namespace std;

namespace gpp {

    matrix4x4::matrix4x4() {
arr.fill(0.0f);
    }

    matrix4x4::matrix4x4(const matrix4x4& mt) {
*this=mt;
    }

    matrix4x4::matrix4x4(const array<array<decimal, 4>, 4>& mt) {
*this=mt;
    }

matrix4x4::matrix4x4(const initializer_list<initializer_list<decimal>>& mt){
*this=mt;
}

    matrix4x4& matrix4x4::operator=(const matrix4x4& mt) {
if(this!=&mt){
this->arr=mt.arr;
}

return *this;
    }

    matrix4x4& matrix4x4::operator=(const array<array<decimal, 4>, 4>& mt) {
for(uint32 i=0; i<4; i++){
for(uint32 i1=0; i1<4; i1++){
arr[i*4+i1]=mt[i][i1];
}
}
return *this;
    }

matrix4x4& matrix4x4::operator=(const initializer_list<initializer_list<decimal>>& mt){
    auto row_it = mt.begin();
    for (uint32 i = 0; i < 4 && row_it != mt.end(); ++i, ++row_it) {
        auto col_it = row_it->begin();
        for (uint32 j = 0; j < 4 && col_it != row_it->end(); ++j, ++col_it) {
            arr[i * 4 + j] = *col_it;
        }
    }
return *this;
}
//Operators overload...

matrix4x4& matrix4x4::operator+=(decimal s) {
    for_each(arr.begin(), arr.end(), [s](decimal& element) { element += s; });
    return *this;
}

matrix4x4& matrix4x4::operator-=(decimal s) {
    for_each(arr.begin(), arr.end(), [s](decimal& element) { element -= s; });
    return *this;
}

matrix4x4& matrix4x4::operator*=(decimal s) {
    for_each(arr.begin(), arr.end(), [s](decimal& element) { element *= s; });
    return *this;
}

matrix4x4& matrix4x4::operator/=(decimal s) {
    if (s != 0) {
        for_each(arr.begin(), arr.end(), [s](decimal& element) { element /= s; });
    }
    return *this;
}

matrix4x4& matrix4x4::operator+=(const matrix4x4& mt) {
    for (uint32 i = 0; i < 16; ++i) {
        arr[i] += mt.arr[i];
    }
    return *this;
}

matrix4x4& matrix4x4::operator-=(const matrix4x4& mt) {
    for (uint32 i = 0; i < 16; ++i) {
        arr[i] -= mt.arr[i];
    }
    return *this;
}

    void matrix4x4::zero() {
arr.fill(0);
    }

    void matrix4x4::identity() {
arr[0]=1;
arr[5]=1;
arr[10]=1;
arr[15]=1;
    }

    void matrix4x4::setDiagonal(const array<decimal, 4>& d){
arr[0]=d[0];
arr[5]=d[1];
arr[10]=d[2];
arr[15]=d[3];
    }

    array<decimal, 4> matrix4x4::getDiagonal() const {
array<decimal, 4> r={arr[0], arr[5], arr[10], arr[15]};
return r;
    }

matrix4x4& matrix4x4::negate() {
for_each(arr.begin(), arr.end(), [&](decimal& val){val*=-1;});
    return *this;
}

matrix4x4& matrix4x4::inverse() {
    std::array<decimal, 32> augmented;
    for (uint32 i = 0; i < 4; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
            augmented[i * 8 + j] = arr[i * 4 + j];
            augmented[i * 8 + j + 4] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Aplica a eliminação de Gauss-Jordan
    for (uint32 i = 0; i < 4; ++i) {
        // Encontra o pivô máximo na coluna i
        uint32 maxRow = i;
        for (uint32 k = i + 1; k < 4; ++k) {
            if (std::abs(augmented[k * 8 + i]) > std::abs(augmented[maxRow * 8 + i])) {
                maxRow = k;
            }
        }

        // Troca as linhas se necessário
        if (maxRow != i) {
            for (uint32 j = 0; j < 8; ++j) {
                std::swap(augmented[i * 8 + j], augmented[maxRow * 8 + j]);
            }
        }

        // Verifica se a matriz é singular
        if (std::abs(augmented[i * 8 + i]) < 1e-9) {
            throw std::runtime_error("Matriz singular, não pode ser invertida.");
        }

        // Normaliza a linha do pivô
        decimal pivot = augmented[i * 8 + i];
        for (uint32 j = 0; j < 8; ++j) {
            augmented[i * 8 + j] /= pivot;
        }

        // Elimina as outras linhas
        for (uint32 k = 0; k < 4; ++k) {
            if (k != i) {
                decimal factor = augmented[k * 8 + i];
                for (uint32 j = 0; j < 8; ++j) {
                    augmented[k * 8 + j] -= factor * augmented[i * 8 + j];
                }
            }
        }
    }

    // Extrai a matriz inversa da parte direita da matriz aumentada
    for (uint32 i = 0; i < 4; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
            arr[i * 4 + j] = augmented[i * 8 + j + 4];
        }
    }

    return *this;
}

matrix4x4& matrix4x4::transpose() {
    array<decimal, 16> temp;
    for (uint32 i = 0; i < 4; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
            temp[i * 4 + j] = arr[j * 4 + i];
        }
    }
    arr = temp; // Atribuindo a matriz transposta de volta a 'arr'
    return *this;
}

decimal matrix4x4::determinant() const{
    decimal det = arr[0] * (arr[5] * arr[10] * arr[15] +
                            arr[6] * arr[11] * arr[13] +
                            arr[7] * arr[9] * arr[14] -
                            arr[7] * arr[10] * arr[13] -
                            arr[6] * arr[9] * arr[15] -
                            arr[5] * arr[11] * arr[14]);

    return det;
}

matrix4x4 matrix4x4::getTranslation(const vector3d& translation) {
    return matrix4x4{
        {1, 0, 0, translation.x},
        {0, 1, 0, translation.y},
        {0, 0, 1, translation.z},
        {0, 0, 0, 1}
    };
}

matrix4x4 matrix4x4::getScale(const vector3d& scale) {
    return matrix4x4{
        {scale.x, 0, 0, 0},
        {0, scale.y, 0, 0},
        {0, 0, scale.z, 0},
        {0, 0, 0, 1}
    };
}

matrix4x4 matrix4x4::getRotation(const vector3d& angles) {

    decimal cosX = cos(degrees_to_radians(angles.x));
    decimal sinX = sin(degrees_to_radians(angles.x));
    decimal cosY = cos(degrees_to_radians(angles.y));
    decimal sinY = sin(degrees_to_radians(angles.y));
    decimal cosZ = cos(degrees_to_radians(angles.z));
    decimal sinZ = sin(degrees_to_radians(angles.z));

    // Matriz de rotação em torno do eixo X
    matrix4x4 rotX = {
        {1, 0, 0, 0},
        {0, cosX, -sinX, 0},
        {0, sinX, cosX, 0},
        {0, 0, 0, 1}
    };

    // Matriz de rotação em torno do eixo Y
    matrix4x4 rotY = {
        {cosY, 0, sinY, 0},
        {0, 1, 0, 0},
        {-sinY, 0, cosY, 0},
        {0, 0, 0, 1}
    };

    // Matriz de rotação em torno do eixo Z
    matrix4x4 rotZ = {
        {cosZ, -sinZ, 0, 0},
        {sinZ, cosZ, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    // Combina as rotações na ordem ZYX
    return rotZ * rotY * rotX;
}

matrix4x4 matrix4x4::getSRT(const vector3d& scale, const vector3d& angles, const vector3d& translation){

matrix4x4 sc=getScale(scale);
matrix4x4 rt=getRotation(angles);
matrix4x4 ts=getTranslation(translation);

    return ts * rt * sc;
}

matrix4x4 matrix4x4::lerp(const matrix4x4& a, const matrix4x4& b, decimal t) {
    matrix4x4 result;
    for (int i = 0; i < 16; ++i) {
        result.arr[i] = a.arr[i] * (1 - t) + b.arr[i] * t;
    }
    return result;
}

matrix4x4 matrix4x4::negate(const matrix4x4& mt) {
    matrix4x4 result(mt);
    return result.negate();
}

matrix4x4 matrix4x4::inverse(const matrix4x4& mt) {
    matrix4x4 result(mt);
result.inverse();
return result;
}

matrix4x4 matrix4x4::transpose(const matrix4x4& mt) {
    matrix4x4 result(mt);
    return result.transpose();
}

decimal matrix4x4::determinant(const matrix4x4& mt) {
    return mt.determinant();
}

    ostream& operator<<(ostream& os, const matrix4x4& mt) {
for(uint32 i=0; i<4; i++){
for(uint32 i1=0; i1<4; i1++){
if(i1<3){
os<<mt.arr[i*4+i1]<<", ";
}
else{
os<<mt.arr[i*4+i1]<<endl;
}
}
}
return os;
    }

    ostream& operator<<(ostream& os, const array<decimal, 4>& arr){
for(uint32 i=0; i<4; i++){
if(i<3){
os<<arr[i]<<", ";
}
else{
os<<arr[i]<<endl;
}
}
return os;
}

//Overloads

matrix4x4 operator+(const matrix4x4& mt, decimal s) {
    matrix4x4 result(mt);
    result += s;
    return result;
}

matrix4x4 operator+(decimal s, const matrix4x4& mt) {
matrix4x4 result(mt);
result+=s;
return result;
}

matrix4x4 operator-(const matrix4x4& mt, decimal s) {
    matrix4x4 result(mt);
    result -= s;
    return result;
}

matrix4x4 operator-(decimal s, const matrix4x4& mt) {
    matrix4x4 result(mt);
    result -=s;
    return result;
}

matrix4x4 operator*(const matrix4x4& mt, decimal s) {
    matrix4x4 result(mt);
    result *= s;
    return result;
}

matrix4x4 operator*(decimal s, const matrix4x4& mt) {
matrix4x4 result(mt);
result*=s;
return result;
}

matrix4x4 operator/(const matrix4x4& mt, decimal s) {
if(s!=0){
    matrix4x4 result(mt);
    result /= s;
    return result;
}
return mt;
}

matrix4x4 operator/(decimal s, const matrix4x4& mt) {
if(s!=0){
    matrix4x4 result(mt);
    result /= s;
    return result;
}
return mt;
}

matrix4x4 operator+(const matrix4x4& mt1, const matrix4x4& mt2) {
    matrix4x4 result(mt1);
    result += mt2;
    return result;
}

matrix4x4 operator-(const matrix4x4& mt1, const matrix4x4& mt2) {
    matrix4x4 result(mt1);
    result -= mt2;
    return result;
}

matrix4x4 operator*(const matrix4x4& mt1, const matrix4x4& mt2) {
    matrix4x4 result;
    result.zero(); // Inicializa com zeros
    for (uint32 i = 0; i < 4; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
            for (uint32 k = 0; k < 4; ++k) {
                result.arr[i*4 + j] += mt1.arr[i*4 + k] 
                                      * mt2.arr[k*4 + j];
            }
        }
    }
    return result;

}

bool operator==(const matrix4x4& m1, const matrix4x4& m2){
return matrix4x4_isEqual(m1, m2, numeric_limits<decimal>::epsilon());
}

bool matrix4x4_isEqual(const matrix4x4& m1, const matrix4x4& m2, decimal tol){
for(uint32 i=0; i<16; i++){
if(fabs(m2.arr[i]-m1.arr[i])>tol){
return false;
}
}
return true;
}

vector3d operator*(const matrix4x4& m, const vector3d& v) {
    decimal x = m.arr[0] * v.x + m.arr[1] * v.y + m.arr[2] * v.z + m.arr[3];
    decimal y = m.arr[4] * v.x + m.arr[5] * v.y + m.arr[6] * v.z + m.arr[7];
    decimal z = m.arr[8] * v.x + m.arr[9] * v.y + m.arr[10] * v.z + m.arr[11];
    decimal w = m.arr[12] * v.x + m.arr[13] * v.y + m.arr[14] * v.z + m.arr[15];

    // Normaliza o vetor resultante (divide por w se w != 0)
    if (w != 0) {
        x /= w;
        y /= w;
        z /= w;
    }

    return vector3d(x, y, z);
}

vector3d operator*(const vector3d& v, const matrix4x4& m) {

/*
    decimal x = v.x * m.arr[0] + v.y * m.arr[4] + v.z * m.arr[8] + m.arr[12];
    decimal y = v.x * m.arr[1] + v.y * m.arr[5] + v.z * m.arr[9] + m.arr[13];
    decimal z = v.x * m.arr[2] + v.y * m.arr[6] + v.z * m.arr[10] + m.arr[14];
    decimal w = v.x * m.arr[3] + v.y * m.arr[7] + v.z * m.arr[11] + m.arr[15];

    // Normaliza o vetor resultante (divide por w se w != 0)
    if (w != 0) {
        x /= w;
        y /= w;
        z /= w;
    }

    return vector3d(x, y, z);
*/
    decimal x = m.arr[0] * v.x + m.arr[1] * v.y + m.arr[2] * v.z + m.arr[3];
    decimal y = m.arr[4] * v.x + m.arr[5] * v.y + m.arr[6] * v.z + m.arr[7];
    decimal z = m.arr[8] * v.x + m.arr[9] * v.y + m.arr[10] * v.z + m.arr[11];
    decimal w = m.arr[12] * v.x + m.arr[13] * v.y + m.arr[14] * v.z + m.arr[15];

    // Normaliza o vetor resultante (divide por w se w != 0)
    if (w != 0) {
        x /= w;
        y /= w;
        z /= w;
    }

    return vector3d(x, y, z);

}
}
