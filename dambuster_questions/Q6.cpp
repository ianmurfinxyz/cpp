#include <iostream>
#include <cstring>
#include <cassert>

class String
{
public:
  String()
  { 
    std::cout << "String()" << std::endl;
    _str = new char[1]; 
    *_str = 0; 
  }

  ~String()
  { 
    std::cout << "~String()" << std::endl;
    if(_str) delete[] _str; 
  }

  String(const char* str)
  {
    std::cout << "String(const char*)" << std::endl;

    size_t len = std::strlen(str);
    if(len == 0){
      _str = new char[1];
      *_str = 0;
      return;
    }
    _str = new char[len + 1];
    memcpy(_str, str, len + 1);
    return;
  }


// section 1 ////////////////////////////////////////////////////////////////////////////////////

  //String(String&& other)
  //{
  //  std::cout << "String(String&&)" << std::endl;
  //  if(&other == this)
  //    return;

  //  _str = other._str;
  //  other._str = new char[1];
  //  other._str[0] = 0;
  //}

  //String& operator=(String&& other)
  //{
  //  std::cout << "String& operator=(String&&)" << std::endl;
  //  if(&other == this)
  //    return *this;

  //  _str = other._str;
  //  other._str = new char[1];
  //  other._str[0] = 0;
  //  return *this;
  //}

//////////////////////////////////////////////////////////////////////////////////////////////////



// section 2 ////////////////////////////////////////////////////////////////////////////////////

  //String(String&&) = delete;
  //String& operator=(String&&) = delete;
 
//////////////////////////////////////////////////////////////////////////////////////////////////



  String(const String& other)
  {
    std::cout << "String(const String&)" << std::endl;

    assert(other._str != nullptr);

    size_t len = std::strlen(other._str);
    if(len == 0){
      _str = new char[1];
      *_str = 0;
    }
    else {
      _str = new char[len + 1];
      std::memcpy(_str, other._str, len + 1); // +1 so we copy the terminator
    }
  }

  String& operator=(const String& other)
  {
    std::cout << "String& operator=(const String&)" << std::endl;

    //
    // assume the constructor of String establishes this invariant.
    //
    assert(other._str != nullptr);

    size_t len = std::strlen(other._str);
    if(len == 0){
      *_str = 0;
      return *this;
    }
    if(len > std::strlen(_str)){
      delete[] _str;
      _str = new char[len + 1];
    }
    std::memcpy(_str, other._str, len + 1); // +1 so we copy the terminator
    return *this;
  }
  
  friend std::ostream& operator<<(std::ostream& os, const String& str)
  { os << str._str; return os;}

private:
  char* _str;
};

String function(String str1)
{
  String str2;
  str2 = str1;
  return str2;
}

int main()
{
  String str1{"hello world"};
  str1 = function(str1);
  std::cout << str1 << std::endl;

  String str2{"goodbye world"};
  String str3 = str2; // copy elision, so will simply call String(const String&)
  std::cout << str2 << std::endl;
  std::cout << str3 << std::endl;
}

// 
// Results:
//
// If I include a move assignment in my class GCC will use it for line 118, as shown below.
//
// note: results from compilation with GCC 10.2.0
//
// [runtime results from uncommenting section 1]
//
// String(const char*)
// String(const String&)
// String()
// String& operator=(const String&)
// String& operator=(String&&)
// ~String()
// ~String()
// hello world
// String(const char*)
// String(const String&)
// goodbye world
// goodbye world
// ~String()
// ~String()
// ~String()
//
// If I simply dont add a move assignment in my class GCC will simply use the copy assignment,
// as shown below.
//
// [runtime results from leaving both sections 1 & 2 commented]
//
// String(const char*)
// String(const String&)
// String()
// String& operator=(const String&)
// String& operator=(const String&)
// ~String()
// ~String()
// hello world
// String(const char*)
// String(const String&)
// goodbye world
// goodbye world
// ~String()
// ~String()
// ~String()
//
// yet, if I explicitly delete the move assignent
//
// [compilation results from uncommenting section 2]
//
// Q6.cpp: In function ‘int main()’:
// Q6.cpp:131:23: error: use of deleted function ‘String& String::operator=(String&&)’
//   131 |   str1 = function(str1);
//       |                       ^
// Q6.cpp:69:11: note: declared here
//    69 |   String& operator=(String&&) = delete;
//      |           ^~~~~~~~
//
