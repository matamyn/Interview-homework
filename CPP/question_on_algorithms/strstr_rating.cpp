#include <iostream>

const char *strstr(const char *haystack, const char *needle) {
  char *cp_str = const_cast<char *>(haystack);
  char *str1, *str2;
  while (*cp_str) {
    str1 = cp_str;
    str2 = const_cast<char *>(needle);
    while (*str1 && *str2 && !(*str1 - *str2))
      str1++, str2++;
    if (!*str2)
      return (cp_str);
    ++cp_str;
  }

  return (nullptr);
}
int main() {
  char haystack[] =
      "This is a very strange task. In a real project, I would use QString, "
      "since it works adequately with Russian characters.";
  char needle[] = "QString";
  auto test = strstr(haystack, needle);
  return 0;
}
