#include "moving_average/MovingAverage.h"
#include "sample_variance/SampleVariance.h"
#include <thread>
int main(int argc, char *argv[]) {

  MovingAverage movingAverage(argv[1]);
  SampleVariance sampleVariance(argv[1]);

  std::thread tA(&MovingAverage::start, movingAverage);
  std::thread tB(&SampleVariance::start, sampleVariance);
  tA.join();
  tB.join();
  return (0);
}