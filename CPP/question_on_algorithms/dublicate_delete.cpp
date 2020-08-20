#include <fstream>
#include <iostream>
#include <set>
int main(int argc, char *argv[]) {
  std::ofstream out;
  out.open("output.txt");
  std::string line;
  std::ifstream in("input.txt"); // окрываем файл для чтения
  std::set<std::string> uniq_set;
  getline(in, line);
  if (in.is_open()) {
    while (getline(in, line)) {
      if (uniq_set.find(line) != uniq_set.end())
        uniq_set.emplace(line);
      out<<line;
    }
  }
}