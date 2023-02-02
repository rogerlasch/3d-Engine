

#include<signal.h>
#include<iostream>
#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
using namespace gpp;
using namespace std;

int main() {
  quaternion q1(1, 2, 3, 4);
  quaternion q2(5, 6, 7, 8);

  cout << "quaternion 1: " << q1.toString() << endl;
  cout << "quaternion 2: " << q2.toString() << endl;

  cout << "quaternion 1 + quaternion 2: " << (q1 + q2).toString() << endl;
  cout << "quaternion 1 - quaternion 2: " << (q1 - q2).toString() << endl;
  cout << "quaternion 1 * quaternion 2: " << (q1 * q2).toString() << endl;
  cout << "quaternion 1 / quaternion 2: " << (q1 / q2).toString() << endl;

  cout << "quaternion 1 conjugate: " << q1.conjugate().toString() << endl;
  cout << "quaternion 1 magnitude: " << q1.magnitude() << endl;
  cout << "quaternion 1 normalize: " << q1.normalize().toString() << endl;

  return 0;
}
