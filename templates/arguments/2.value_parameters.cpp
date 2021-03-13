//
// REFERENCES
// ----------
// [0] The C++ Programming Language by Bjarne Stroustrup
//
// VALUE ARGUMENTS
// ---------------
// 
// An argument for a template value parameter can be:
// 
//   - an integral constant expression
//   - a pointer or reference to an object or a function with external linkage
//   - a nonoverloaded pointer to member
//   - a null pointer
// 
// This means no bools and no floats or doubles. (actually you can use bool, see experiment 0)
// 
// All integral values must be constexpr, this is since the template is evaluated at compile thus
// the value must actually have a value at compile (hence constexpr).
// 
// All pointers must be pointers to an actual object or function (so lvalues?), so you cannot
// pass pointers to an rvalue? Bjarne on p724 of The C++ Programming Language 4 Edition does not
// state this explicitly thus I think I am wrong on this. He does state that,
// 
// "A pointer used as a template argument must be of the form &of, where of is the name of an
// object or a function, or of the form f, whre f is the name of a function... a string literal
// is not acceptable as a template argument"
// 
// - why external linkage?
// 
// - why allow nullptrs?
// 
// These restrictions on value arguments are imposed because,
// 
// "...exists to simplify implementation of separately compiled translation units. It is best to
// think of template value arguments as a mechanism for passing integers and pointers to functions. 
// Resist the temptation to try something more clever."
// 
// Lets try create some errors then...

#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////
// EXPERIMENT 0:

/*
template<typename T, bool b>
struct IsTrue
{
  T _value;
  bool isTruthy() const {return b;}
};

int main()
{
  IsTrue<int, true> _istrue;
  _istrue._value = 20;
  std::cout << _istrue.isTruthy() << std::endl; // outputs 1
  std::cout << _istrue._value << std::endl;     // outputs 20
}
*/

//
// Conlusion: you can use bool as a template argument, thus must be because it can be converted
// to an integer. This example does output 1 (i.e. an integer) after all.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// EXPERIMENT 1

/*
template<float f>
struct floaty
{
  float floatyval() const {return f;}
};

int main()
{
  floaty<20.f> f;
  std::cout << f.floatyval() << std::endl;
}
*/

// conlusion: gives error,
//  'float' is not a valid type for a template non-type parameter
//
//  gooooooood!
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// OPERATIONS AS ARGUMENTS
//
// In section 25.2.3 of ref[0] Bjarne talks about the design decision behind making STL
// containers accepts type parameters rather than function pointer parameters for comparison
// functions. It is worth going over.
//
// Consider a generic map...
//
// template<typename key, typename value>
// class map
// {
//   ...
// };
//
// Basically a container such as a map may desire a custom sorting criterion for its elements;
// to sort them based on the key value. For this you need some way of comparing keys.
//
// Because the element type of the container is generic (our container is a template) we cannot
// know what sorting criterion to use for all key types, thus we cannot hardwire the criterion
// into the container.
//
// Further because any given key may desire to be sorted in different ways we also cannot
// hardwire the criterion into the key, e.g. we may want to sort a string key by case sensistive
// in one map and case-insensitive in another.
//
// Thus we need to provide the sorting criterion as a seperate template argument.
//
// Bjarne states that from a design perspective we can add the sorting criterion either as a 
// value template argument or a type template argument. But which is the better choice?
//
// 1 - As a type template argument, 
//
// template<typename key, typename value, typename compare>
// class map
// {
//   ...
// };
//
// 2 - As a value template argument,
//
// template<typename Key, typename Value, bool (*Compare)(const Key&, const Key&)>
// class map
// {
//   ...
// };
//
// The standard library uses the former, so map is defined as,
//
// template<typename Key, Class V, typename Compare = std::less<Key>>
// class map {
// public:
//   map(){/*...*/}
//   map(Compare c) : cmp{c}{/*...*/}  // so we store the function object instance in the map
//   ...
//   Compare cmp {};  // our default comparison in the map
//                ^ 
//                |
//        the compiler will know to call the default constructor of std::less<Key>, since
//        the concrete specialisation of this template (assuming it uses the default Compare)
//        will have the std::less<Key> function object as the member. Thus a specialisation
//        like,
//
//           map<int, int>
//
//        would have the Compare member
//
//        std::less<int> cmp {};
// };
//
// note: std::less<> is a function object defined in the standard library.
//    https://en.cppreference.com/w/cpp/utility/functional/lesso
//  it invokes < on the template type.
//
// note: the the function object to a template like this is refered to as a 'policy', thus we
// are supplying a policy to the template. The same can be said of supplying function objects
// to the STL algorithms like the sorting algorithms, you supply them with policies to determine
// the sorting criterion.

//////////////////////////////////////////////////////////////////////////////////////////////////
// DEFAULT ARGUMENTS
//
// This example, 
//
// template<typename Key, Class V, typename Compare = std::less<Key>>
// class map {
// public:
//   map(){/*...*/}
//   map(Compare c) : cmp{c}{/*...*/}  // so we store the function object instance in the map
//   ...
//   Compare cmp {};  // our default comparison in the map
// };
//
// also demonstrates the use of default template arguments. There are a few things worth noting
// about default template arguments:
//
// Like default function arguments, default template arguments can only be used for trailing
// arguments. It is interesting that Bjarne noted that they could of allowed the syntax,
//    
//                       v----default 1st arg but required 2nd
//    template<typename T1=int, typename T2>
//    X{ ... };
//    X<,float> x;
//       ^-- a comma to miss the 1st argument and provide only the required 2nd.
//
// the standards commitee decided against this as they thought it could lead to hard to spot
// errors.
//
// The semantic checking for default template arguments is only actually done if the default
// argument is used. Thus if your map template is that shown above, and you provide a key
// for which std::less<Key> would not compile, it is not a problem as long as you provide a 
// Compare functor which will compile. It would be if you tried to use the default.
//         see P729 for more info.
//
