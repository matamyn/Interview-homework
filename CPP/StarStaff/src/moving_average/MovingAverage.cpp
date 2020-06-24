#include "MovingAverage.h"

#include <fstream>
#include <iostream>
MovingAverage::MovingAverage(const std::string &a_file)
    : file_(std::make_shared<std::ifstream>(a_file)) {}

void MovingAverage::work_iteration(const std::string &a_line) {
  long double t = std::stold(a_line);
  sum += t;
  if (count == 1 || count == 100 || count == 1000000 || count == 500000000) {
    sma.insert(std::pair<size_t, long double>{count, calculate_sma(sum)});
    printf("Number %zu: %Lf - moving sum: %01Lf - SMA: %02Lf\n", count, t, sum,
           sma.find(count)->second);
  }
}
void MovingAverage::start() {
  std::string s;
  //Была идея использовать кольцевой буффер, но т.к. нет условия параллельной
  //записи в файл и мой boost с qt решили не подключать все готовые реализации,
  //я решил не изобретать велосипед
  while (getline(*file_, s)) {
    count++;
    work_iteration(s);
  }
}
long double MovingAverage::calculate_sma(const long double a_current_sum) {

  return a_current_sum == 0.0 ? 0.0 : a_current_sum / count;
}