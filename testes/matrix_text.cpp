

#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>


using namespace std;
using namespace gpp;

vector3d mu2(const matrix3x3& m, const vector3d& v);

bool operator==(const matrix3x3& m1, const matrix3x3& m2);
void printResult(bool test, const string& func_name, const matrix3x3& result, const matrix3x3& expected_result);
#define _pres(status, result, expected) printResult(status, __FUNCTION__, result, expected)
void testSum();
void testSub();
void testMul();
void testDiv();
void testIdentity();
void testTranspose();
void testDeterminant();
void testInverse();
void testVector();
int main()
{
setlocale(LC_ALL, "Portuguese");
testSum();
testSub();
testMul();
testDiv();
testIdentity();
testTranspose();
testDeterminant();
testInverse();
testVector();
return 0;
}

bool operator==(const matrix3x3& m1, const matrix3x3& m2)
{
float tol=0.001f;
return fabs(m2.e11-m1.e11)<=tol&&fabs(m2.e12-m1.e12)<=tol&&fabs(m2.e13-m1.e13)<=tol&&
fabs(m2.e21-m1.e21)<=tol&&fabs(m2.e22-m1.e22)<=tol&&fabs(m2.e23-m1.e23)<=tol&&
fabs(m2.e31-m1.e31)<=tol&&fabs(m2.e32-m1.e32)<=tol&&fabs(m2.e33-m1.e33)<=tol;
}

void printResult(bool test, const string& func_name, const matrix3x3& result, const matrix3x3& expected_result)
{
if(test)
{
_FLOG("O teste da função {} passou!", func_name);
}
else
{
_FLOG("O teste da função {} falhou!", func_name);
_FLOG("Resultado esperado:\n{}", expected_result.toString());
_FLOG("Resultado encontrado:\n{}", result.toString());
}
}

void testSum()
{
matrix3x3 m1(1,2,3,4,5,6,7,8,9);
matrix3x3 m2(1,2,3,4,5,6,7,8,9);
matrix3x3 m3=m1+m2;
matrix3x3 result(2,4,6,8,10,12,14,16,18);
_pres(m3==result, m3, result);
}

void testSub()
{
matrix3x3 m1(1,2,3,4,5,6,7,8,9);
matrix3x3 m2(1,2,3,4,5,6,7,8,9);
matrix3x3 m3=m2-m1;
matrix3x3 result(0,0,0,0,0,0,0,0,0);
_pres(m3==result, m3, result);
}

void testMul()
{
matrix3x3 m1(1,2,3,4,5,6,7,8,9);
matrix3x3 m2(1,2,3,4,5,6,7,8,9);
matrix3x3 m3=m1*m2;
matrix3x3 result(30,36,42, 66, 81, 96,  102, 126, 150 );
_pres(m3==result, m3, result);
}

void testDiv()
{
matrix3x3 m1(1,2,3,4,5,6,7,8,9);
matrix3x3 m2=m1/2.0f;
matrix3x3 result(0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f);
_pres(m2==result, m2, result);
}

void testIdentity()
{
matrix3x3 m1;
m1.Identity();
matrix3x3 result(1,0,0,0,1,0,0,0,1);
_pres(m1==result, m1, result);
}

void testTranspose()
{
matrix3x3 m1(1,2,3,4,5,6,7,8,9);
matrix3x3 m2=m1.Transpose();
matrix3x3 result(1,4,7,2,5,8,3,6,9);
_pres(m2==result, m2, result);
}

void testDeterminant()
{
matrix3x3 m1(1250, 0,0,0,1250,0,0,0,1250);
float result=1953124992;
float dt=m1.det();
_pres(dt==result, matrix3x3(), matrix3x3());
}

void testInverse()
{
matrix3x3 m1(1250,0,0,0,1250,0,0,0,1250);
    matrix3x3 m2=m1.Inverse().Inverse();
_pres(m2==m1, m2, m1);
}

void testVector()
{
matrix3x3 m(1,2,3,4,5,6,7,8,9);
vector3d v(1,2,3);
vector3d result=m1*v;
vector3d expected(14,32,50);
if(result==expected)
{
_FLOG("O teste da função {} passou!", __FUNCTION__);
}
else
{
_FLOG("O teste da função {} falhou!", __FUNCTION__);
_FLOG("Resultado esperado: {}:{}:{}", expected.x, expected.y, expected.z);
_FLOG("Resultado obitito: {}:{}:{}", result.x, result.y, result.z);
}
}


vector3d mu2(const matrix3x3& m, const vector3d& v)
{
vector3d v2;
v2.x=m.e11*v.x+m.e12*v.y+m.e13*v.z;
v2.y=m.e21*v.x+m.e22*v.y+m.e23*v.z;
v2.z=m.e31*v.x+m.e32*v.y+m.e33*v.z;
return v2;
}
