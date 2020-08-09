#pragma once
#include <iostream>
using namespace std;
class A {
public:
  A() { cout << "constructor A" << endl; }
};
class B : public A {
public:
  B() { cout << "constructor B" << endl; }
};
