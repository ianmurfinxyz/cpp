
//
// On page 781 section 28.2 Bjarne defines a type functions as,
//
// "A type function is a function that takes at least on type as an argument or produces at
// least one type as a result. For example sizeof(T) is a built-in type function that given
// a type argument T returns the size of an object (measured in chars)."
//
// He then goes on to show how a template can be used to implement a metafunction and 
// specifically a type (meta) function.
//
// Within this chapter he also defines metaprogramming, which is worth noting to,
//
// "So, metaprogramming is acombinartion of "meta" and programming: a metaprogram is a comile-
// time computation yeilding types of functions to be used a run time."
//
// essentially a metaprogram is a program about a (or makes a) program, just like metadata is
// data about data.
//
// Thus a metafunction is a function which runs at compile time as part of a metaprogram. Note
// that the term metafunction is not something that Bjarne introduces, I have just made it up, 
// to illustrate to myself the difference between a compile time and run time function. Bjarne
// would like call the compile time functions constexpr functions which is probably a better
// term since a metafunction is not a function about a function.
//

//
// So how would you use a template as a function? Heres one that computes the max of two 
// integers.
//

#include <iostream>

//
// This is a compile-time function, it relies on the fact that std::max is a constexpr 
// function and thus can be executed by the compiler at compile time.
//
template<int N, int M>
struct mymax
{
  static constexpr int value = std::max(N, M);
};    //     ^-- our return value must be constexpr so it is computed at compile time.

int main()
{
  //      note that the args also have to be const so their values are availible at 
  // v--- compile time.
  const int n {4}, m {5};
  std::cout << "max of " << n << " and " << m << " is " << mymax<n, m>::value << std::endl;
  //              we pass args to a compile-time function -------^       ^
  //              implemented as a template via the                      |
  //              template args                            we get the return value by accessing
  //                                                       the member.
}

//
// note that bjarne would not call this function a type function since it does not take a 
// type as an argument nor does it return a type. Further this function is obviously pointless
// as it is just a wrapper around std::max which is a compile time function which does the
// same thing. This actually illustrates however that there are 2 ways to implement compile
// time functions which operate on values, either as constexpr functions like the std::max or
// as a template. For value functions though constexpr way is cleaner and so is the obvious 
// better alternative (which Bjarne states in the book). However for type functions we have
// to use the template form; type functions cannot be implemented as compile functions.
//
// This actually gives an alternative way of viewing the std::array header implementation which
// contains the array_traits template...
//
// could this be viewed as a compile time function? (a type function) or am I misunderstanding?
//
//template<typename _Tp, std::size_t _Nm>
//    struct __array_traits
//    {
//      typedef _Tp _Type[_Nm];
//      typedef __is_swappable<_Tp> _Is_swappable;
//      typedef __is_nothrow_swappable<_Tp> _Is_nothrow_swappable;
//      static constexpr _Tp&
//      _S_ref(const _Type& __t, std::size_t __n) noexcept
//      { return const_cast<_Tp&>(__t[__n]); }
//      static constexpr _Tp*
//      _S_ptr(const _Type& __t) noexcept
//      { return const_cast<_Tp*>(__t); }
//    };
// template<typename _Tp>
//   struct __array_traits<_Tp, 0>
//   {
//     struct _Type { };
//     typedef true_type _Is_swappable;
//     typedef true_type _Is_nothrow_swappable;
//     static constexpr _Tp&
//     _S_ref(const _Type&, std::size_t) noexcept
//     { return *static_cast<_Tp*>(nullptr); }
//     static constexpr _Tp*
//     _S_ptr(const _Type&) noexcept
//     { return nullptr; }
//   };
