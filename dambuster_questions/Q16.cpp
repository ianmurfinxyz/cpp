#include <cassert>
#include <chrono>
#include <iostream>
#include <fstream>

struct vec3
{
  float x, y, z;
};

void addToAll_slow(vec3 *array, vec3 *toAdd, unsigned int num)
  //
  // Question:
  //
  // This function looks fine but there is a performance problem with it. What is it and 
  // what ways are there around it?
  //
{
  for(int i = 0; i < num; ++i){
    array->x += toAdd->x;
    array->y += toAdd->y;
    array->z += toAdd->z;
    ++array;
  }
}

//
// the performance issue I think is the uneccessary repeated derefencing of the toAdd pointer
// and the array. So lets fix that and time the difference...
//

//
// this is no faster, in fact it appears to be (a smidge) slower.
//
void addToAll_fast1(vec3 *array, vec3 toAdd, unsigned int num)
{                                   // ^-- copy in since only one being added
  for(int i = 0; i < num; ++i){
    vec3& e = *array;      // 1 deref rather than 3
    e.x += toAdd.x;
    e.y += toAdd.y;
    e.z += toAdd.z;
    ++array;
  }
}

//
// This appears to be a smidge faster.
//
void addToAll_fast2(vec3 *array, vec3 toAdd, unsigned int num)
{                                   // ^-- copy in since only one being added
  for(int i = 0; i < num; ++i){
    array->x += toAdd.x;
    array->y += toAdd.y;
    array->z += toAdd.z;
    ++array;
  }
}

void addToAll_fast3(vec3 *array, vec3 toAdd, unsigned int num)
{                                   // ^-- copy in since only one being added
  float x {toAdd.x}, y {toAdd.y}, z {toAdd.z};
  for(int i = 0; i < num; ++i){
    array->x += x;
    array->y += y;
    array->z += z;
    ++array;
  }
}


int main()
{
  //std::ofstream file {"dump"};
  //assert(file);

  constexpr int count {1'000'000};
  vec3* array = new vec3[count];
  assert(array != nullptr);
  for(int i = 0; i < count; ++i)
    array[i] = vec3{i, i * 2, i * 3};

  vec3 toAdd {0.5f, 0.6f, 0.7f};

  auto now0 = std::chrono::high_resolution_clock::now();
  addToAll_slow(array, &toAdd, count);
  auto now1 = std::chrono::high_resolution_clock::now();
  auto dt = std::chrono::duration_cast<std::chrono::microseconds>(now1 - now0);
  std::cout << "addToAll_slow exec time: " << dt.count() << "us" << std::endl;

  //for(int i = 0; i < count; ++i)
  //  file << array[i].x << ", " << array[i].y << ", " << array[i].z << std::endl;
  
  now0 = std::chrono::high_resolution_clock::now();
  addToAll_fast1(array, toAdd, count);
  now1 = std::chrono::high_resolution_clock::now();
  dt = std::chrono::duration_cast<std::chrono::microseconds>(now1 - now0);
  std::cout << "addToAll_fast1 exec time: " << dt.count() << "us" << std::endl;

  now0 = std::chrono::high_resolution_clock::now();
  addToAll_fast2(array, toAdd, count);
  now1 = std::chrono::high_resolution_clock::now();
  dt = std::chrono::duration_cast<std::chrono::microseconds>(now1 - now0);
  std::cout << "addToAll_fast2 exec time: " << dt.count() << "us" << std::endl;

  now0 = std::chrono::high_resolution_clock::now();
  addToAll_fast3(array, toAdd, count);
  now1 = std::chrono::high_resolution_clock::now();
  dt = std::chrono::duration_cast<std::chrono::microseconds>(now1 - now0);
  std::cout << "addToAll_fast3 exec time: " << dt.count() << "us" << std::endl;

  //for(int i = 0; i < count; ++i)
  //  file << array[i].x << ", " << array[i].y << ", " << array[i].z << std::endl;
}

//
// results: the fast function is not faster, in fact it appears to be slower! This is with 
// all optimisations disabled with the -O0 options to GCC. Hmmm...back to the drawing board on
// that one.
//
// fast2 and fast3 appear to be consistently a tad faster than slow and fast1 but there is little
// difference between them. My conclusion is that there must be another answer, and that... 
// this is not the performance problem you are looking for.
//
// addToAll_slow exec time: 3497us
// addToAll_fast1 exec time: 3580us
// addToAll_fast2 exec time: 2831us
// addToAll_fast3 exec time: 2852us
// addToAll_slow exec time: 4169us
// addToAll_fast1 exec time: 4726us
// addToAll_fast2 exec time: 3612us
// addToAll_fast3 exec time: 3471us
// addToAll_slow exec time: 4122us
// addToAll_fast1 exec time: 4694us
// addToAll_fast2 exec time: 3544us
// addToAll_fast3 exec time: 3403us
// addToAll_slow exec time: 4160us
// addToAll_fast1 exec time: 4783us
// addToAll_fast2 exec time: 3623us
// addToAll_fast3 exec time: 3544us
// addToAll_slow exec time: 3272us
// addToAll_fast1 exec time: 3573us
// addToAll_fast2 exec time: 2839us
// addToAll_fast3 exec time: 2826us
// addToAll_slow exec time: 4028us
// addToAll_fast1 exec time: 4678us
// addToAll_fast2 exec time: 3594us
// addToAll_fast3 exec time: 3448us
// addToAll_slow exec time: 3573us
// addToAll_fast1 exec time: 3573us
// addToAll_fast2 exec time: 3208us
// addToAll_fast3 exec time: 3143us
// addToAll_slow exec time: 3406us
// addToAll_fast1 exec time: 3579us
// addToAll_fast2 exec time: 2840us
// addToAll_fast3 exec time: 2829us
// addToAll_slow exec time: 3478us
// addToAll_fast1 exec time: 3599us
// addToAll_fast2 exec time: 2840us
// addToAll_fast3 exec time: 2829us
// addToAll_slow exec time: 3501us
// addToAll_fast1 exec time: 3583us
// addToAll_fast2 exec time: 2838us
// addToAll_fast3 exec time: 2832us
// addToAll_slow exec time: 3483us
// addToAll_fast1 exec time: 3572us
// addToAll_fast2 exec time: 2840us
// addToAll_fast3 exec time: 3054us
// addToAll_slow exec time: 3689us
// addToAll_fast1 exec time: 3866us
// addToAll_fast2 exec time: 3584us
// addToAll_fast3 exec time: 3445us
// addToAll_slow exec time: 2984us
// addToAll_fast1 exec time: 3614us
// addToAll_fast2 exec time: 2838us
// addToAll_fast3 exec time: 2875us
// addToAll_slow exec time: 3314us
// addToAll_fast1 exec time: 3584us
// addToAll_fast2 exec time: 2886us
// addToAll_fast3 exec time: 2826us
// addToAll_slow exec time: 3378us
// addToAll_fast1 exec time: 3587us
// addToAll_fast2 exec time: 2837us
// addToAll_fast3 exec time: 2843us
// addToAll_slow exec time: 4134us
// addToAll_fast1 exec time: 3579us
// addToAll_fast2 exec time: 2833us
// addToAll_fast3 exec time: 2896us
// addToAll_slow exec time: 4061us
// addToAll_fast1 exec time: 3732us
// addToAll_fast2 exec time: 2846us
// addToAll_fast3 exec time: 2827us
// addToAll_slow exec time: 3142us
// addToAll_fast1 exec time: 3669us
// addToAll_fast2 exec time: 2875us
// addToAll_fast3 exec time: 2825us
