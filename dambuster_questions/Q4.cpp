#include <string>
#include <iostream>
#include <math.h>

/* DONE! TESTED! EASY! */

int string_to_int(const std::string& str)
{
  int result {0};
  int sign {1};
  for(auto riter = str.rbegin(); riter != str.rend(); ++riter){
    if(*riter == '-'){ 
      sign = -1;
      break;
    }
    int digit = static_cast<int>(*riter - '0');
    result += digit * std::pow(10, static_cast<double>(riter - str.rbegin()));
  }
  return result * sign;
}

int main()
{
  std::string str0 {"2345"};
  std::cout << string_to_int(str0) << std::endl;
  std::string str1 {"-817420"};
  std::cout << string_to_int(str1) << std::endl;
}
