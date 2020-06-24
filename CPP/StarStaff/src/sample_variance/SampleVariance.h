#include <memory>
#include <string>
class SampleVariance {
public:
  explicit SampleVariance(const std::string &a_file);
  void start();

private:
  void work_iteration(const std::string &a_line);
  long double variance() const;
  std::shared_ptr<std::ifstream> file_;
  size_t count{0};
  long double old_mean{}, new_mean{};
  long double old_Var{}, new_Var{};
};
