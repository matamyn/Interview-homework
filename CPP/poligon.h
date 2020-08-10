#pragma once
#include <iostream>
#include <mutex>

using namespace std;
class A {
public:
  A(std::recursive_mutex &mut_) {
    cout << "constructor A" << endl;
    std::lock_guard<std::recursive_mutex> amd1{mut_};
  }
};
class B : public A {
public:
  B() : A(mut_) {
    cout << "constructor B" << endl;
    std::lock_guard<std::recursive_mutex> amd{mut_};
  }
  std::recursive_mutex mut_;
};
