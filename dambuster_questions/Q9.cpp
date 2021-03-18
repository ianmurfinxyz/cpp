

//
// I have decided to write this question out in full here before I solve it. Why? Because 
// somewhat coincidentally I actually read what the problem was and the solution only last
// week in "The C++ Programming Language 4th Edition by Bjarne Stroustrup". Bjarne discussed
// this in chapter 25 (section 25.3).
//

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// The question:

template<typename T>
void bubbleSort(T* ptr, unsigned int num)
{
  unsigned int change;
  do{
    change = 0;
    for(int i = 0; i < num - 1; ++i){
      if(ptr[i] > ptr[i + 1]){
        T tmp = ptr[i];
        ptr[i] = ptr[i + 1];
        ptr[i + 1] = tmp;
        change = 1;
      }
    }
  }
  while(change);
}

// bubbleSort<unsigned int>(someNumbers, 100);
// bubbleSort<enumT>(someEnums, 100);
// bubbleSort<enum2T>(someEnum2s, 100);
// bubbleSort<unsigned char*>(some32bitPointers, 100);
//
// Ignoring the ingerent problems with bubble sort, why is this wasteful?
// (If you are stuck consider why templates can be bad, but also the advantages they offer.)
//
// How would you go about writing a generic replacement?
//
//////////////////////////////////////////////////////////////////////////////////////////////////

//
// So why is it wasteful? Because as discussed by Bjarne in 25.3, for templates in which the
// templated type T is used as a pointer type, you will be creating a lot of specialisations
// for different pointer types. So in the example specialisations in the question, the template
// will be instantiated for unsigned int*, enumT*, enum2T*, and unsigned char**. The more
// efficient solution, to reduce code bloat, is to create a base complete specialisation for
// a void* pointer type and use that for all pointer specialisations.
//
// Thus that is how I would implement a generic replacement. This will reduce code bloat 
// because a single specialisation can be used for all pointer types. 
//
// Note that to do this will will have to provide a custom comparison policy becuase we cannot
// dereference void* pointers to compare their values.
//


//////////////////////////////////////////////////////////////////////////////////////////////////

//
// my (better?) solution 1...
//

#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>

//
// This is not pretty. We need a policy to both compare and swap because we cannot do either
// with void pointers. Thus this reduced code bloat to a certain extent but kinda just shifts
// the problem and now the user has to manually create said policies.
//

template<typename T>
void mybubble(T* ptr, unsigned int num, bool (*compare)(T* a, T *b), void (*swap)(T* a, T* b))
{
  bool change;
  do{
    change = false; 
    for(int i{0}; i < num - 1; ++i){
      if(!compare(ptr + i, ptr + i + 1)){
        swap(ptr + i, ptr + i + 1);
        change = true;
      }
    }
  }
  while(change);
  for(int i = 0; i < num; ++i)
    std::cout << ptr[i] << std::endl;
}

template<typename T>
bool less_ptr(T* a, T* b)
{ return *a < *b; }

template<typename T>
void swap_ptr(T* a, T* b)
{
  T tmp = *a;
  *a = *b;
  *b = tmp;
}

int main()
{
  constexpr int count {20};
  std::array<int, count> numbers;
  for(int i{0}; i < count; ++i)
    numbers[i] = count - i;

  mybubble<int>(numbers.data(), count, &less_ptr<int>, &swap_ptr<int>);

  std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>{std::cout, "\n"});
  std::cout << std::endl;
}

//
// yeah okay this actually didn't get me anywhere. All I have done is added policies. I was
// going to create a partial spetialisation but then I remembered you cant do that for functions.
//
// note that the method I am trying to employ here doesn't even need a generic function. All
// I really needed was to create a complete specialisation for T as a void* and use that for
// all sorts.
//
// In fact I cannot even spetialise mybubble with void* because that will require policies with
// signitures
//    bool (*)(void*, void*) and void(*)(void*, void*)
//
// which defeats the entire point.
//
// Massive fail! :) Go away learn more about templates and try again...
//

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// TODO: a solution that isn't dumb!
//
