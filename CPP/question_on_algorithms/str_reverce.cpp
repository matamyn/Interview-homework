#include <iostream>
#include <string>
int main() {
  std::string str1;
  std::string str2;
  std::cin >> str1 >> str2;
  std::string str1_revert;
  for (auto it = str1.rbegin(); it != str1.rend(); ++it) {
    str1_revert.push_back(*it);
  }
  auto res = str1 == str1_revert;
  std::cout << res << std::endl;
}
