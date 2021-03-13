

//
// typedefs are actually more interesing than what I was previously aware. You can actually
// declare multiple typedefs in a single line.
//
// refereces:
// [0] https://en.cppreference.com/w/cpp/language/typedef
// [1] https://riptutorial.com/cplusplus/example/28887/declaring-multiple-types-with-typedef
//

//
// int_t == int
// intp_t == int*
// fp == (&)(int,long)  <-- a reference to a function? Yup thats a thing! https://stackoverflow.com/questions/480248/function-references
// arr_t == int[10]
//
typedef int int_t, *intp_t, (&fp)(int, long), arr_t[10];

int main()
{
  //
  // declerations actually follow the same rules too, so...
  //
  int x, *px, (*pf)(), arr[10];
  //
  // x has type int
  // px has type int*
  // pf has type int(*)()
  // arr has type int[10]
  //

  //
  // here (the GNU std::array implementation),
  //  https://code.woboq.org/gcc/libstdc++-v3/include/std/array.html
  //
  // is the template struct...
  //

template<typename _Tp, std::size_t _Nm>
struct __array_traits
{
  typedef _Tp _Type[_Nm]; // WHAT IS THIS!!
  typedef __is_swappable<_Tp> _Is_swappable;
  typedef __is_nothrow_swappable<_Tp> _Is_nothrow_swappable;
  static constexpr _Tp&
  _S_ref(const _Type& __t, std::size_t __n) noexcept
  { return const_cast<_Tp&>(__t[__n]); }
  static constexpr _Tp*
  _S_ptr(const _Type& __t) noexcept
  { return const_cast<_Tp*>(__t); }
};

// 
// it is like 
//
// typedef int int_t[N];
//
// where,
//  int_t == int[N]
//
// thus
//
// _Type == _Tp[_Nm]
