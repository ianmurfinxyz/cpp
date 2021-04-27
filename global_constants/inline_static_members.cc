#include <iostream>

class Thing {
public:

  // pre c++17 constexpr was required to have in class initialization.
  //static float pi;
  
  // c++17 inline variables allow in-class initialization of non-constant static members
  // without need to externally define the member.
  //
  // requires c++17 so currently (as of GCC version 10.2.0) requires compilation with flag:
  // -std=c++17 (for GCC and CLANG, VS flag is different).
  inline static float pi {3.14159};

  static inline void print_pi()
  {
    std::cout << "pi=" << pi << std::endl;
  }

  inline void print_x()
  {
    std::cout << "x=" << x << std::endl;
  }

public:
  float x;
};

// pre c++17 this was required.
//float Thing::pi {3.14159f};

int main()
{
  Thing t{32.f};
  t.print_pi();
  t.print_x();
}
