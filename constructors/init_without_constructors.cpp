//
// Reading the code for the GNU implementation of std::array,
//  https://code.woboq.org/gcc/libstdc++-v3/include/std/array.html
//
// I noticed that std::array is defined as a struct and not a class, thus the elements of the
// array are public, and there are no constructors within the struct. This is stated at,
//   https://en.cppreference.com/w/cpp/container/array
// where it states the constructors, destructors and operator= are 'implicitly declared'.
//
// Thus std::array relies on:
//
//  - default initialization (i.e. without an initializer or with an EMPTY initializer list).
//  - memberwise initialization
//  - copy initialization
//
// so we can do,
//
// std::array<int, 3> a {1,2,3};  // memberwise initialization
// std::array<int, 3> b {};       // default initialization with empty initializer list
// std::array<int, 3> c;          // no initialization
// std::array<int, 3> d {a};      // copy initialization
//
// but we cannot call a constructor explicitly like,
//
// std::array<int, 3> e (1,2,3);  // error no such constructor

//
// About that public member data...
//
// It turns out that you can actually access that data directly...once you peek at the source :)
// (since the name of the elements is not in the standard and so likely changes this is obviously
// not a good idea).
//

#include <array>
#include <iostream>
#include <algorithm>
#include <iterator>  // for the cool stream adaptor!

int main()
{
  std::array<int, 3> a {};
  a._M_elems[0] = 1;         // yup this actually works! Thus my header is likely the
  a._M_elems[1] = 2;         // GNU implementation, which makes sense since I am running arch
  a._M_elems[2] = 3;
  std::copy(a.begin(), a.end(), std::ostream_iterator<int>{std::cout, "\n"}); // because I still
                                                                              // think its cool! :)
}

//
// Q: Did the implementers make the elements public and use a struct specifically to take
// advantage of the memberwise initialization feature? Rather than adding uneccessary 
// constructors?
//
// A: hmmmmmm...maybe?
