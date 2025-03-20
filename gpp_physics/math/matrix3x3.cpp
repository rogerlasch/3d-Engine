

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

vector3d matrix3x3::getRow(uint32 index)const{
if(index>2){
throw runtime_error("Index out of bounds in getRow method in matrix3x3.");
}
if(index==0){
return vector3d(arr[0], arr[1], arr[2]);
}

if(index==2){
return vector3d(arr[3], arr[4], arr[5]);
}

return vector3d(arr[6], arr[7], arr[8]);
}

matrix3x3& matrix3x3::zero() {
arr.fill(0);
return *this;
    }

matrix3x3& matrix3x3::identity() {
arr[0]=1;
arr[4]=1;
arr[8]=1;
return *this;
    }

matrix3x3& matrix3x3::setDiagonal(const array<decimal, 3>& d){
arr[0]=d[0];
arr[4]=d[1];
arr[8]=d[2];
return*this;
    }

    array<decimal, 3> matrix3x3::getDiagonal() const {
array<decimal, 3> r={arr[0], arr[4], arr[8]};
return r;
    }

matrix3x3& matrix3x3::negate() {
for_each(arr.begin(), arr.end(), [&](decimal& val){val*=-1;});
    return *this;
}

matrix3x3& matrix3x3::inverse() {
    const decimal det = determinant();
    if (std::abs(det) < 1e-8) return *this;

    matrix3x3 adj;
    // Cálculo da matriz adjunta
    adj.arr[0] = +(arr[4]*arr[8] - arr[5]*arr[7]);
    adj.arr[1] = -(arr[1]*arr[8] - arr[2]*arr[7]);
    adj.arr[2] = +(arr[1]*arr[5] - arr[2]*arr[4]);
    
    adj.arr[3] = -(arr[3]*arr[8] - arr[5]*arr[6]);
    adj.arr[4] = +(arr[0]*arr[8] - arr[2]*arr[6]);
    adj.arr[5] = -(arr[0]*arr[5] - arr[2]*arr[3]);

    adj.arr[6] = +(arr[3]*arr[7] - arr[4]*arr[6]);
    adj.arr[7] = -(arr[0]*arr[7] - arr[1]*arr[6]);
    adj.arr[8] = +(arr[0]*arr[4] - arr[1]*arr[3]);

    // Transposição e multiplicação pelo inverso do determinante
    *this = adj.transpose() * (1.0f/det);
    return *this;

}

matrix3x3& matrix3x3::transpose() {

    std::array<decimal, 9> temp;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            temp[i*3 + j] = arr[j*3 + i];
        }
    }
    arr = temp;
    return *this;

}

decimal matrix3x3::determinant() const{
    return arr[0]*(arr[4]*arr[8] - arr[5]*arr[7])
         - arr[1]*(arr[3]*arr[8] - arr[5]*arr[6])
         + arr[2]*(arr[3]*arr[7] - arr[4]*arr[6]);
}

matrix3x3 matrix3x3::getTranslation(const vector3d& vt) {
    return matrix3x3{
        {1, 0, vt.x},
        {0, 1, vt.y},
        {0, 0, 1}
    };
}

matrix3x3 matrix3x3::getScale(const vector3d& scale) {
    return matrix3x3{
        {scale.x, 0, 0},
        {0, scale.y, 0},
        {0, 0, scale.z}
    };
}

matrix3x3 matrix3x3::getRotation(const vector3d& angles) {

    const auto Rx = matrix3x3{{
        {1, 0, 0},
        {0, std::cos(angles.x), -std::sin(angles.x)},
        {0, std::sin(angles.x), std::cos(angles.x)}
    }};

    const auto Ry = matrix3x3{{
        {std::cos(angles.y), 0, std::sin(angles.y)},
        {0, 1, 0},
        {-std::sin(angles.y), 0, std::cos(angles.y)}
    }};

    const auto Rz = matrix3x3{{
        {std::cos(angles.z), -std::sin(angles.z), 0},
        {std::sin(angles.z), std::cos(angles.z), 0},
        {0, 0, 1}
    }};

    return Rz * Ry * Rx;
    }

matrix3x3 matrix3x3::getSRT(const vector3d& scale, const vector3d& angles, const vector3d& translation){

matrix3x3 sc=getScale(scale);
matrix3x3 rt=getRotation(angles);
matrix3x3 ts=getTranslation(translation);

return sc*rt*ts;
}

matrix3x3 matrix3x3::lerp(const matrix3x3& a, const matrix3x3& b, decimal t) {
    matrix3x3 result;
    for (int i = 0; i < 9; ++i) {
        result.arr[i] = a.arr[i] * (1 - t) + b.arr[i] * t;
    }
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
    for (int i = 0; i < 3; ++i) {
        os << "[ ";
        for (int j = 0; j < 3; ++j) {
            os << mt.arr[i*3 + j];
            if (j < 2) os << ", ";
        }
        os << " ]\n";
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
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.arr[i*3 + j] = 0;
            for (int k = 0; k < 3; ++k) {
                result.arr[i*3 + j] += mt1.arr[i*3 + k] * mt2.arr[k*3 + j];
            }
        }
    }

    return result;
}

       vector3d operator*(const matrix3x3& m, const vector3d& v)       {
    return vector3d(
        m.arr[0]*v.x + m.arr[1]*v.y + m.arr[2]*v.z,
        m.arr[3]*v.x + m.arr[4]*v.y + m.arr[5]*v.z,
        m.arr[6]*v.x + m.arr[7]*v.y + m.arr[8]*v.z
    );
}

       vector3d operator*(const vector3d& v, const matrix3x3& m)       {
vector3d u;
u.x=m.arr[0]*v.x + m.arr[1]*v.y + m.arr[2]*v.z;
u.y=m.arr[3]*v.x + m.arr[4]*v.y + m.arr[5]*v.z;
u.z=m.arr[6]*v.x + m.arr[7]*v.y + m.arr[8]*v.z;
return u;
}

bool operator==(const matrix3x3& m1, const matrix3x3& m2){
return matrix3x3_isEqual(m1, m2, numeric_limits<decimal>::epsilon());
}

bool matrix3x3_isEqual(const matrix3x3& m1, const matrix3x3& m2, decimal tol){
for(uint32 i=0; i<9; i++){
if(fabs(m2.arr[i]-m1.arr[i])>tol){
return false;
}
}
return true;
}
}
