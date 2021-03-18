#include <iostream>
#include <cassert>

//
// What does the following function accomplish?
//
int foo(long long val)
{
  int n = 0; 
  while(val){
    val &= val-1;
    ++n;
  }
  return n;
}

//
// Pre-test. What is my guess?
//
// val-1 will flip the least-significant bit (LSB), i.e. the 2^0 bit.
//
// thus val &= val-1 will at least set the LSB to 0.
//
// however more generally val &= val-1 will leave val only with those bits that did not
// flip in the -1 op. Thus whenever a bit flips we are removing it from val.
//
// ...okay I get it,
//
// for a binary number such as 1000, 1000-1=0111, thus 1000&(1000-1)=0000
//
// for 1100&(1100-1)=1100&1011=1000, then 1000&(1000-1)=0000 (as above)
//
// thus because of the general pattern that, 1000-1=0111, or 1<n0s>-1=0<n1s>, then
// 1<n0s>&(1<n0s>-1)=0
//
// thus the effect of val&=val-1 is to remove the least significant 1 from val. 
//
// The function foo thus repeats this until there are no more 1s to remove.
//
// n is then the count of the 1s removed.
//
// In conclusion, foo counts the number of bits set in val.
//

int main()
{
  assert(foo(0b0110) == 2);
  assert(foo(0b011110) == 4);
  assert(foo(0b0111001100110) == 7);
  assert(foo(0b011101100100110011001100101001100110) == 18);

  //
  // results: no asserts thus yup foo counts bits. QED
  //
}
