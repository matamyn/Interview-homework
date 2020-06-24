#include "SampleVariance.h"
#include <cmath>
#include <fstream>
#include <iostream>

SampleVariance::SampleVariance(const std::string &a_file)
    : file_(std::make_shared<std::ifstream>(a_file)) {}
void SampleVariance::start() {
  std::string s;
  while (getline(*file_, s)) {
    count++;
    work_iteration(s);
  }
}
void SampleVariance::work_iteration(const std::string &a_line) {
  long double t = std::stold(a_line);
  if (count == 1) {
    old_mean = new_mean = t;
    old_Var = 0.0;
  } else {
    new_mean = old_mean + (t - old_mean) / count;
    new_Var = old_Var + (t - old_mean) * (t - new_mean);
    old_mean = new_mean;
    old_Var = new_Var;
  }
  auto sample_var = std::sqrt(variance());
  if (count == 1 || count == 100 || count == 1000000 || count == 500000000) {
    printf("Number %zu: %Lf - sample variance: %Lf\n", count, t, sample_var);
  }
}
long double SampleVariance::variance() const { return ((count > 1) ? new_Var / (count - 1) : 0.0);  }