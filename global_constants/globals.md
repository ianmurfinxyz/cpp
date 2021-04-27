## Global Constants

There are multiple ways to implement global constants within C++. In C++17 there is another;
inline variables. This document is the product of me learning about this new feature and its
motivation.

## Pre C++17 Methods

#### Header Only Constexpr

Create a seperate header and store all constants in said header as constexpr globals. Include 
header in all translation units that require the constants.

```c++
// file: some_file.hh

namespace constants
{
  constexpr int maxHealth {3};
  constexpr float gravity {9.81};
  constepxr float pi {3.14159};
}
```

**Cons:**

- const globals have internal linkage; each translation unit gets its own copy resulting in 
wasted memory. This is only really a problem for complex types that the compiler cannot optimise
away by replacing all references with literals. Creating constexpr std::arrays in a header like
this and then including in multiple translation units will almost certainly result in multiple
copies of the arrays.

- changes to any constants require recompiling all translation units where those constants are
used, increasing compilation times.

#### Header Forward Declerations to Extern Globals

Forward declare globals as extern in a header; define globals in their own translation unit.

```c++
// file: some_file.hh

namespace constants
{
  extern int maxHealth;
  extern float gravity;
  extern float pi;
}
```

```c++
// file: some_file.cc

namespace constants
{
  extern int maxHealth {3};
  extern float gravity {9.81};
  extern float pi {3.14159};
}
```

**Pros:**

- A single copy of each constant now exists, no duplication. 
- Changes require only recompiling the single translation unit where the constants are defined.

**Cons:**

- The cost of the pros is that the constants are now runtime constants. You know this since a
change in the constants does not require recompiling other translation units thus usage of said
constants must be runtime evaluated. The consequence? No compile time optimisation and these
global constants cannot be used where compile time constants are required such as in constexpr
functions or as array sizes.

#### Enter C++17 Inline Variables

The keyword ```inline``` has effectively evolved to mean "multiple definitions are allowed" [1].

Inline variables allow a global variable (yes one with external linkage) to be defined in a header 
and included in multiple translation units without flagging the multiple symbol definition
compilation error; without violating the one definition rule. The linker (yes linker not compiler)
will consolidate all definitions into a single definition.

Restrictions on inline variables are like those of inline functions:
- all definitions must be identical (else undefined behaviour. Is this caught by the compiler?)
- the definition must be present in all files that use the variable.

```c++
// file: some_file.hh

namespace constants
{
  inline constexpr int maxHealth {3};
  inline constexpr float gravity {9.81};
  inline constepxr float pi {3.14159};
}
```
**pros:**

- can include in multiple translation units and will generate only a single copy in the final image.
- are compile time constants so can utilise compile time optimisations and used in contexts that
require compile time constants.

**cons:**

- will require recompiling all translation units that use the constants, but this is unavoidable
for compile time constants.

#### Inline Variables as Static Class Members

Inline variables can also be used to define non-const (and non-constexpr) static class members
in the body of a class like,

```c++
class Thing {
public:
  
  inline static float thingy_bob {4.5f};

}
```

whereas before C++17 this would require a definition placed externally of the class body in a
seperate translation unit.

```c++
// file: some_file.hh

class Thing {
public:
  
  static float thingy_bob;

}
```

```c++
// file: some_file.cc

float Thing::thing_bob {4.0f};
```

## Experimental Verification

I verified these conclusions by running an experiment in which I created a large global array
which the compiler cannot optimise away, include it in 3 seperate translation units and print
a different element from the array in a function defined in each. From a common main I then
call the 3 print functions (see source in cpp/global_constants/duplicate_constexpr_array). 
Conlusions can be read here [results](duplicate_constexpr_array/results.md).

## References

1. https://www.learncpp.com/cpp-tutorial/global-constants-and-inline-variables/
