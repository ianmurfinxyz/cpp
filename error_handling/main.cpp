#include <iostream>
#include <string>

int main()
{
  std::string s { "hello" };
  int n = std::stoi(s);
  std::cout << n << std::endl;
}
