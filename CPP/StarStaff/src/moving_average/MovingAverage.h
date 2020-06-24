#include <map>
#include <memory>
#include <string>

class MovingAverage {
public:
  explicit MovingAverage(const std::string &a_file);
  void start();

private:
  void work_iteration(const std::string &a_line);
  long double calculate_sma(long double a_current_sum);
  std::shared_ptr<std::ifstream> file_;

  //Я использую здесь map исключительно из за специфики задания, вывести только
  // 4 эллемента. Если необходимо сначала посчитать все эллементы, а затем их
  //вывести, идеально будет использовать какое либо дерево, но и вектор даёт
  //достаточную скорость доступа по индексу
  std::map<size_t, long double> sma;
  long double sum{0};
  size_t count{0};
};
