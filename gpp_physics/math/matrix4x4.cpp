
#include <algorithm>
#include"math.h"

using namespace std;

namespace gpp {

    matrix4x4::matrix4x4() {
arr.fill(0.0f);
    }

    matrix4x4::matrix4x4(const matrix4x4& mt) {
this->arr=mt.arr;
    }

    matrix4x4::matrix4x4(const array<array<decimal, 4>, 4>& mt) {
*this=mt;
    }

matrix4x4::matrix4x4(const initializer_list<initializer_list<decimal>>& mt){
*this=mt;
}

    matrix4x4& matrix4x4::operator=(const matrix4x4& mt) {
this->arr=mt.arr;
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
decimal det=this->determinant();
if(det==0.0){
return *this;
}
det=1.0f/det;
for_each(arr.begin(), arr.end(), [det](decimal& val){val*=det;});
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
decimal result= arr[12] * arr[9]  * arr[6]  * arr[3]   -  arr[8] * arr[13] * arr[6]  * arr[3]   -
         arr[12] * arr[5]  * arr[10] * arr[3]   +  arr[4] * arr[13] * arr[10] * arr[3]   +
         arr[8]  * arr[5]  * arr[14] * arr[3]   -  arr[4] * arr[9]  * arr[14] * arr[3]   -
         arr[12] * arr[9]  * arr[2]  * arr[7]   +  arr[8] * arr[13] * arr[2]  * arr[7]   +
         arr[12] * arr[1]  * arr[10] * arr[7]   -  arr[0] * arr[13] * arr[10] * arr[7]   -
         arr[8]  * arr[1]  * arr[14] * arr[7]   +  arr[0] * arr[9]  * arr[14] * arr[7]   +
         arr[12] * arr[5]  * arr[2]  * arr[11]  -  arr[4] * arr[13] * arr[2]  * arr[11]  -
         arr[12] * arr[1]  * arr[6]  * arr[11]  +  arr[0] * arr[13] * arr[6]  * arr[11]  +
         arr[4]  * arr[1]  * arr[14] * arr[11]  -  arr[0] * arr[5]  * arr[14] * arr[11]  -
         arr[8]  * arr[5]  * arr[2]  * arr[15]  +  arr[4] * arr[9]  * arr[2]  * arr[15]  +
         arr[8]  * arr[1]  * arr[6]  * arr[15]  -  arr[0] * arr[9]  * arr[6]  * arr[15]  -
         arr[4]  * arr[1]  * arr[10] * arr[15]  +  arr[0] * arr[5]  * arr[10] * arr[15];
return result;
}

matrix4x4 matrix4x4::negate(const matrix4x4& mt) {
    matrix4x4 result(mt);
    return result.negate();
}

matrix4x4 matrix4x4::inverse(const matrix4x4& mt) {
    matrix4x4 result(mt);
    return result.inverse();
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
    for (uint32 i = 0; i < 4; ++i) {
        for (uint32 j = 0; j < 4; ++j) {
            for (uint32 k = 0; k < 4; ++k) {
                result.arr[i * 4 + j] += mt1.arr[i * 4 + k] * mt2.arr[k * 4 + j];
            }
        }
    }
    return result;
}
}
