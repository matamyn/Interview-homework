#include <iostream>
#include <string>
void max(u_int &t_count, u_int &b_count) {
  if (t_count > b_count) {
    b_count = t_count;
  }
}
int main(int argc, char *argv[]) {
  std::string str;
  std::cin>>str;
  str[0]=0;
  std::cout << str.c_str() << std::endl;

  u_int t_count = 0;
  u_int b_count = 0;
  for (const auto &ch : str) {
    if (ch == '1') {
      t_count += 1;
      max(t_count, b_count);
    } else {
      t_count = 0;
    }
  }
  std::cout << b_count << std::endl;
}