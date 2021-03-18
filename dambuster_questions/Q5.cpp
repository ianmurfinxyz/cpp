#include <iostream>
#include <string>

class String
{
public:
  String() : _str{}
  {
    std::cout << "String()" << std::endl;
  }

  ~String()
  {
    std::cout << "~String()" << std::endl;
  }

  String(const char* str)
  {
    std::cout << "String(const char*)" << std::endl;
    _str = str;
  }

  String(const String& other)
  {
    std::cout << "String(const String&)" << std::endl;
    _str = other._str;
  }

  String& operator=(const String& other)
  {
    std::cout << "String& operator=(const String&)" << std::endl;
    _str = other._str;
    return *this;
  }

  //String(String&& other)
  //{
  //  std::cout << "String(String&&)" << std::endl;
  //  _str = other._str;
  //}

  //String& operator=(String&& other)
  //{
  //  std::cout << "String& operator=(String&&)" << std::endl;
  //  _str = other._str;
  //  return *this;
  //}

private:
  std::string _str;
};

String function(String str1)
{
  String str2;
  str2 = str1;
  return str2;
}

int main()
{                          // what did I learn here?
  String str3("hello");    // ----------------------
  str3 = function(str3);   // I learnt that all local vars are destroyed (so dtors called) when
}                          // the stack frame is popped (which I new) and that the frame is 
                           // popped after the return value is assigned (which I didnt know).
//
// 1) String(const char*) // str3
// 2) String(const String&) // str1
// 3) String() // str2
// 4) operator=(const String&) // str2 = str1
// 5) ~String() // str1
// 6) operator=(const String&) // str3=str2
// 7) ~String() // str2
// 8) ~String() // str3
//
// ANSWER:
//
// almost correct; ops 5 and 6 should be swapped.
//
// so both str1 and str2 are destroyed when the function stack frame is popped, and the stack
// frame is popped after the return value is assigned (i.e. after str3=function(str3), which
// does actually make more sense.
//
// so correct order is:
//
// 1) String(const char*) // str3
// 2) String(const String&) // str1
// 3) String() // str2
// 4) operator=(const String&) // str2 = str1
// 5) operator=(const String&) // str3=str2
// 6) ~String() // str1  // str1 and str2 destroyed when stack frame of call to 'function' popped
// 7) ~String() // str2
// 8) ~String() // str3
//
