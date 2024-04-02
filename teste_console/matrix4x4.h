#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <type_traits>
#include <array>
#include<initializer_list>
#include <iostream>

namespace gpp {

    template<class T>
    class matrix4x4 {
static_assert(is_floating_point<T>::value, "O tipo T precisa ser um tipo de ponto flutuante como float, ou double.");
    private:
        std::array<T, 16> arr;

    public:
        matrix4x4();
        matrix4x4(const matrix4x4& mt);
        matrix4x4(const std::array<std::array<T, 4>, 4>& mt);
matrix4x4(const std::initializer_list<std::initializer_list<T>>& mt);
        matrix4x4& operator=(const matrix4x4& mt);
        matrix4x4& operator=(const std::array<std::array<T, 4>, 4>& mt);
matrix4x4& operator=(const std::initializer_list<std::initializer_list<T>>& mt);

//The operators...

//scalar...

matrix4x4& operator+=(T s);
matrix4x4& operator-=(T s);
matrix4x4& operator*=(T s);
matrix4x4& operator/=(T s);

matrix4x4& operator+=(const matrix4x4& mt);
matrix4x4& operator-=(const matrix4x4& mt);

//Methods...

        void zero();
        void identity();
        void setDiagonal(const std::array<T, 4>& d);
        std::array<T, 4> getDiagonal() const;

matrix4x4& negate();
matrix4x4& inverse();
matrix4x4& transpose();
T determinant();

static matrix4x4 negate(const matrix4x4& mt);
static matrix4x4 inverse(const matrix4x4& mt);
static matrix4x4 transpose(const matrix4x4& mt);
static T determinant(const matrix4x4& mt);

    template<class T2>
friend     std::ostream& operator<<(std::ostream& os, const matrix4x4<T2>& mt);
template<class T3>
friend matrix4x4<T3> operator*(const matrix4x4<T3>& mt1, const matrix4x4<T3>& mt2);

    };

    template<class T>
    std::ostream& operator<<(std::ostream& os, const matrix4x4<T>& mt);
    template<class T>
    std::ostream& operator<<(std::ostream& os, const std::array<T, 4>& arr);


//overloads...

template<class T>
matrix4x4<T> operator+(const matrix4x4<T>& mt, T s);
template<class T>
matrix4x4<T> operator+(T s, const matrix4x4<T>& mt);
template<class T>
matrix4x4<T> operator-(const matrix4x4<T>& mt, T s);
template<class T>
matrix4x4<T> operator-(T s, const matrix4x4<T>& mt);
template<class T>
matrix4x4<T> operator*(const matrix4x4<T>& mt, T s);
template<class T>
matrix4x4<T> operator*(T s, const matrix4x4<T>& mt);
template<class T>
matrix4x4<T> operator/(const matrix4x4<T>& mt, T s);
template<class T>
matrix4x4<T> operator/(T s, const matrix4x4<T>& mt);


template<class T>
matrix4x4<T> operator+(const matrix4x4<T>& mt1, const matrix4x4<T>& mt2);
template<class T>
matrix4x4<T> operator-(const matrix4x4<T>& mt1, const matrix4x4<T>& mt2);
template<class T>
matrix4x4<T> operator*(const matrix4x4<T>& mt1, const matrix4x4<T>& mt2);


}
#endif // MATRIX4X4_H

#ifndef MATRIX4X4_IMPLEMENTATION_CPP
#define MATRIX4X4_IMPLEMENTATION_CPP

namespace gpp {

    template<class T>
    matrix4x4<T>::matrix4x4() {
arr.fill(0);
    }

    template<class T>
    matrix4x4<T>::matrix4x4(const matrix4x4& mt) {
this->arr=mt.arr;
    }

    template<class T>
    matrix4x4<T>::matrix4x4(const std::array<std::array<T, 4>, 4>& mt) {
*this=mt;
    }

template<class T>
matrix4x4<T>::matrix4x4(const std::initializer_list<std::initializer_list<T>>& mt){
*this=mt;
}

    template<class T>
    matrix4x4<T>& matrix4x4<T>::operator=(const matrix4x4& mt) {
this->arr=mt.arr;
return *this;
    }

    template<class T>
    matrix4x4<T>& matrix4x4<T>::operator=(const std::array<std::array<T, 4>, 4>& mt) {
for(uint32 i=0; i<4; i++){
for(uint32 i1=0; i1<4; i1++){
arr[i*4+i1]=mt[i][i1];
}
}
return *this;
    }

template<class T>
matrix4x4<T>& matrix4x4<T>::operator=(const std::initializer_list<std::initializer_list<T>>& mt){
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

//Scalar...

#include <algorithm>

template<class T>
matrix4x4<T>& matrix4x4<T>::operator+=(T s) {
    std::for_each(arr.begin(), arr.end(), [s](T& element) { element += s; });
    return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::operator-=(T s) {
    std::for_each(arr.begin(), arr.end(), [s](T& element) { element -= s; });
    return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::operator*=(T s) {
    std::for_each(arr.begin(), arr.end(), [s](T& element) { element *= s; });
    return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::operator/=(T s) {
    if (s != 0) {
        std::for_each(arr.begin(), arr.end(), [s](T& element) { element /= s; });
    }
    return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::operator+=(const matrix4x4<T>& mt) {
    for (uint32 i = 0; i < 16; ++i) {
        arr[i] += mt.arr[i];
    }
    return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::operator-=(const matrix4x4<T>& mt) {
    for (uint32 i = 0; i < 16; ++i) {
        arr[i] -= mt.arr[i];
    }
    return *this;
}

    template<class T>
    void matrix4x4<T>::zero() {
arr.fill(0);
    }

    template<class T>
    void matrix4x4<T>::identity() {
arr[0]=1;
arr[5]=1;
arr[10]=1;
arr[15]=1;
    }

    template<class T>
    void matrix4x4<T>::setDiagonal(const std::array<T, 4>& d){
arr[0]=d[0];
arr[5]=d[1];
arr[10]=d[2];
arr[15]=d[3];
    }

    template<class T>
    std::array<T, 4> matrix4x4<T>::getDiagonal() const {
std::array<T, 4> r={arr[0], arr[5], arr[10], arr[15]};
return r;
    }

template<class T>
matrix4x4<T>& matrix4x4<T>::negate() {
    for (size_t i = 0; i < 16; ++i) {
        arr[i] = -arr[i];
    }
    return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::inverse() {
T det=this->determinant();
if(det==0.0){
return *this;
}

det=1.0/det;
for_each(arr.begin(), arr.end(), [det](T& val){val*=det;});
return *this;
}

template<class T>
matrix4x4<T>& matrix4x4<T>::transpose() {
    std::array<T, 16> temp;

    // Transposição da matriz
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            temp[i * 4 + j] = arr[j * 4 + i];
        }
    }

    arr = temp; // Atribuindo a matriz transposta de volta a 'arr'

    return *this;
}

template<class T>
T matrix4x4<T>::determinant() {
T result= arr[12] * arr[9]  * arr[6]  * arr[3]   -  arr[8] * arr[13] * arr[6]  * arr[3]   -
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

template<class T>
matrix4x4<T> matrix4x4<T>::negate(const matrix4x4<T>& mt) {
    matrix4x4<T> result(mt);
    return result.negate();
}

template<class T>
matrix4x4<T> matrix4x4<T>::inverse(const matrix4x4<T>& mt) {
    matrix4x4<T> result(mt);
    return result.inverse();
}

template<class T>
matrix4x4<T> matrix4x4<T>::transpose(const matrix4x4<T>& mt) {
    matrix4x4<T> result(mt);
    return result.transpose();
}

template<class T>
T matrix4x4<T>::determinant(const matrix4x4<T>& mt) {
    return mt.determinant();
}

    template<class T>
    std::ostream& operator<<(std::ostream& os, const matrix4x4<T>& mt) {
for(uint32 i=0; i<4; i++){
for(uint32 i1=0; i1<4; i1++){
if(i1<3){
os<<mt.arr[i*4+i1]<<", ";
}
else{
os<<mt.arr[i*4+i1]<<std::endl;
}
}
}
return os;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& os, const std::array<T, 4>& arr){
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

template<class T>
matrix4x4<T> operator+(const matrix4x4<T>& mt, T s) {
    matrix4x4<T> result(mt);
    result += s;
    return result;
}

template<class T>
matrix4x4<T> operator+(T s, const matrix4x4<T>& mt) {
matrix4x4<T> result(mt);
result+=s;
return result;
}

template<class T>
matrix4x4<T> operator-(const matrix4x4<T>& mt, T s) {
    matrix4x4<T> result(mt);
    result -= s;
    return result;
}

template<class T>
matrix4x4<T> operator-(T s, const matrix4x4<T>& mt) {
    matrix4x4<T> result(mt);
    result -=s;
    return result;
}

template<class T>
matrix4x4<T> operator*(const matrix4x4<T>& mt, T s) {
    matrix4x4<T> result(mt);
    result *= s;
    return result;
}

template<class T>
matrix4x4<T> operator*(T s, const matrix4x4<T>& mt) {
matrix4x4<T> result(mt);
result*=s;
return result;
}

template<class T>
matrix4x4<T> operator/(const matrix4x4<T>& mt, T s) {
if(s!=0){
    matrix4x4<T> result(mt);
    result /= s;
    return result;
}
return mt;
}

template<class T>
matrix4x4<T> operator/(T s, const matrix4x4<T>& mt) {
if(s!=0){
    matrix4x4<T> result(mt);
    result /= s;
    return result;
}
return mt;
}

template<class T>
matrix4x4<T> operator+(const matrix4x4<T>& mt1, const matrix4x4<T>& mt2) {
    matrix4x4<T> result(mt1);
    result += mt2;
    return result;
}

template<class T>
matrix4x4<T> operator-(const matrix4x4<T>& mt1, const matrix4x4<T>& mt2) {
    matrix4x4<T> result(mt1);
    result -= mt2;
    return result;
}

template<class T>
matrix4x4<T> operator*(const matrix4x4<T>& mt1, const matrix4x4<T>& mt2) {
    matrix4x4<T> result;

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
#endif
