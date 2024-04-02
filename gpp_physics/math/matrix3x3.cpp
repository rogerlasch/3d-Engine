

#include<algorithm>
#include"../types.h"
#include"math.h"

using namespace std;

namespace gpp
{
    matrix3x3::matrix3x3() {
arr.fill(0.0f);
    }

    matrix3x3::matrix3x3(const matrix3x3& mt) {
this->arr=mt.arr;
    }

    matrix3x3::matrix3x3(const array<array<decimal, 3>, 3>& mt) {
*this=mt;
    }

matrix3x3::matrix3x3(const initializer_list<initializer_list<decimal>>& mt){
*this=mt;
}

    matrix3x3& matrix3x3::operator=(const matrix3x3& mt) {
this->arr=mt.arr;
return *this;
    }

    matrix3x3& matrix3x3::operator=(const array<array<decimal, 3>, 3>& mt) {
for(uint32 i=0; i<3; i++){
for(uint32 i1=0; i1<3; i1++){
arr[i*3+i1]=mt[i][i1];
}
}
return *this;
    }

matrix3x3& matrix3x3::operator=(const initializer_list<initializer_list<decimal>>& mt){
    auto row_it = mt.begin();
    for (uint32 i = 0; i < 3 && row_it != mt.end(); ++i, ++row_it) {
        auto col_it = row_it->begin();
        for (uint32 j = 0; j < 3 && col_it != row_it->end(); ++j, ++col_it) {
            arr[i * 3 + j] = *col_it;
        }
    }
return *this;
}

//Operators overload...

matrix3x3& matrix3x3::operator+=(decimal s) {
    for_each(arr.begin(), arr.end(), [s](decimal& element) { element += s; });
    return *this;
}

matrix3x3& matrix3x3::operator-=(decimal s) {
    for_each(arr.begin(), arr.end(), [s](decimal& element) { element -= s; });
    return *this;
}

matrix3x3& matrix3x3::operator*=(decimal s) {
    for_each(arr.begin(), arr.end(), [s](decimal& element) { element *= s; });
    return *this;
}

matrix3x3& matrix3x3::operator/=(decimal s) {
    if (s != 0) {
        for_each(arr.begin(), arr.end(), [s](decimal& element) { element /= s; });
    }
    return *this;
}

matrix3x3& matrix3x3::operator+=(const matrix3x3& mt) {
    for (uint32 i = 0; i < 9; ++i) {
        arr[i] += mt.arr[i];
    }
    return *this;
}

matrix3x3& matrix3x3::operator-=(const matrix3x3& mt) {
    for (uint32 i = 0; i < 9; ++i) {
        arr[i] -= mt.arr[i];
    }
    return *this;
}

    void matrix3x3::zero() {
arr.fill(0);
    }

    void matrix3x3::identity() {
arr[0]=1;
arr[4]=1;
arr[8]=1;
    }

    void matrix3x3::setDiagonal(const array<decimal, 3>& d){
arr[0]=d[0];
arr[3]=d[1];
arr[8]=d[2];
    }

    array<decimal, 3> matrix3x3::getDiagonal() const {
array<decimal, 3> r={arr[0], arr[3], arr[8]};
return r;
    }

matrix3x3& matrix3x3::negate() {
for_each(arr.begin(), arr.end(), [&](decimal& val){val*=-1;});
    return *this;
}

matrix3x3& matrix3x3::inverse() {
decimal det=this->determinant();
if(det==0.0){
return *this;
}
det=1.0f/det;
for_each(arr.begin(), arr.end(), [det](decimal& val){val*=det;});
return *this;
}

matrix3x3& matrix3x3::transpose() {
    array<decimal, 9> temp;
    for (uint32 i = 0; i < 3; ++i) {
        for (uint32 j = 0; j < 3; ++j) {
            temp[i * 3 + j] = arr[j * 3 + i];
        }
    }
    arr = temp; // Atribuindo a matriz transposta de volta a 'arr'
    return *this;
}

decimal matrix3x3::determinant() const{
decimal result = arr[0] * arr[4] * arr[8] +
                 arr[1] * arr[5] * arr[6] +
                 arr[2] * arr[3] * arr[7] -
                 arr[2] * arr[4] * arr[6] -
                 arr[1] * arr[3] * arr[8] -
                 arr[0] * arr[5] * arr[7];
return result;
}

matrix3x3 matrix3x3::negate(const matrix3x3& mt) {
    matrix3x3 result(mt);
    return result.negate();
}

matrix3x3 matrix3x3::inverse(const matrix3x3& mt) {
    matrix3x3 result(mt);
    return result.inverse();
}

matrix3x3 matrix3x3::transpose(const matrix3x3& mt) {
    matrix3x3 result(mt);
    return result.transpose();
}

decimal matrix3x3::determinant(const matrix3x3& mt) {
    return mt.determinant();
}

    ostream& operator<<(ostream& os, const matrix3x3& mt) {
for(uint32 i=0; i<3; i++){
for(uint32 i1=0; i1<3; i1++){
if(i1<2){
os<<mt.arr[i*3+i1]<<", ";
}
else{
os<<mt.arr[i*3+i1]<<endl;
}
}
}
return os;
    }

    ostream& operator<<(ostream& os, const array<decimal, 3>& arr){
for(uint32 i=0; i<3; i++){
if(i<2){
os<<arr[i]<<", ";
}
else{
os<<arr[i]<<endl;
}
}
return os;
}

//Overloads

matrix3x3 operator+(const matrix3x3& mt, decimal s) {
    matrix3x3 result(mt);
    result += s;
    return result;
}

matrix3x3 operator+(decimal s, const matrix3x3& mt) {
matrix3x3 result(mt);
result+=s;
return result;
}

matrix3x3 operator-(const matrix3x3& mt, decimal s) {
    matrix3x3 result(mt);
    result -= s;
    return result;
}

matrix3x3 operator-(decimal s, const matrix3x3& mt) {
    matrix3x3 result(mt);
    result -=s;
    return result;
}

matrix3x3 operator*(const matrix3x3& mt, decimal s) {
    matrix3x3 result(mt);
    result *= s;
    return result;
}

matrix3x3 operator*(decimal s, const matrix3x3& mt) {
matrix3x3 result(mt);
result*=s;
return result;
}

matrix3x3 operator/(const matrix3x3& mt, decimal s) {
if(s!=0){
    matrix3x3 result(mt);
    result /= s;
    return result;
}
return mt;
}

matrix3x3 operator/(decimal s, const matrix3x3& mt) {
if(s!=0){
    matrix3x3 result(mt);
    result /= s;
    return result;
}
return mt;
}

matrix3x3 operator+(const matrix3x3& mt1, const matrix3x3& mt2) {
    matrix3x3 result(mt1);
    result += mt2;
    return result;
}

matrix3x3 operator-(const matrix3x3& mt1, const matrix3x3& mt2) {
    matrix3x3 result(mt1);
    result -= mt2;
    return result;
}

matrix3x3 operator*(const matrix3x3& mt1, const matrix3x3& mt2) {
    matrix3x3 result;
    for (uint32 i = 0; i < 3; ++i) {
        for (uint32 j = 0; j < 3; ++j) {
            for (uint32 k = 0; k < 3; ++k) {
                result.arr[i * 3 + j] += mt1.arr[i * 3 + k] * mt2.arr[k * 3 + j];
            }
        }
    }
    return result;
}

       vector3d operator*(const matrix3x3& m, const vector3d& v)       {
vector3d u;
u.x=m.arr[0]*v.x + m.arr[1]*v.y + m.arr[2]*v.z;
u.y=m.arr[3]*v.x + m.arr[4]*v.y + m.arr[5]*v.z;
u.z=m.arr[6]*v.x + m.arr[7]*v.y + m.arr[8]*v.z;
return u;
}

       vector3d operator*(const vector3d& v, const matrix3x3& m)       {
vector3d u;
u.x=m.arr[0]*v.x + m.arr[1]*v.y + m.arr[2]*v.z;
u.y=m.arr[3]*v.x + m.arr[4]*v.y + m.arr[5]*v.z;
u.z=m.arr[6]*v.x + m.arr[7]*v.y + m.arr[8]*v.z;
return u;
}
}
