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
// Ah WHY?!
//
// The answer I found here: 
//   https://stackoverflow.com/questions/23000243/c-deleted-move-assignment-operator-compilation-issues
//
// Basically the line, 
//
//   String& operator=(String&&) = delete;
//
// is actually a decleration of the move assignment, thus with that line I am telling the compiler
// that my class has a move assignment...then I am telling it that it is deleted and attempted
// use of it is an error.
//
// This is different to simply not declaring the line, not doing so tells the compiler that
// the class does not have a move assignment, so the compiler will use the copy assignment
// instead.
//
// I thus misunderstood what the = delete meant. I thought it was simply used to prevent a 
// default move assignment being created. Thus by adding the line or not adding the line I was
// thinking it would not make a difference. Thus my mental model was wrong. 
//
// In conclusion, explicitly deleting a move assignment is used to prevent the class being used
// in assignment opertions in which the compiler would select the move assignment as the most 
// appropriate assignment function. Thus the compiler will create an error upon any such attempt.
//
// Whereas, simply not adding a move assignment, means when the compiler chooses which assignment
// function to call within an assignment operation, assuming the compiler determined that the
// move assignment was the best option, it will see the class has no move assignment and instead
// choose the copy assignment.
//
// So, short summary,
//
//  String& operator=(String&&) != 
//                                        ^-- i.e. nothing :)
//
