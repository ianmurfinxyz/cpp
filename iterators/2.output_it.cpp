

//
// OUTPUT ITERATORS
//
// Output iterators, as the name implies, allow you to output to a range. However this range
// may not have an end and the goal is simply to write a value into a 'black hole'.
//
// With an output iterator you can only step forward with write access. You cannot step
// backward and you cannot read. For this reason you can only 'dereference' an output
// iterator as an lvalue (i.e. on the left side of an assignment) and not as an rvalue.
//
// Further no comparison operators are required for output iterators, thus a pure output
// iterator makes no guarantees an iterator is valid or whether writing was successful or 
// whether one output iterator is equal to another.
//
// You can copy construct, copy assign and destruct output iterators. However even copied
// output iterators make no guarantees to be 'equal'. 
//
// With an output_stream output iterator, for example, you can create multiple copies but
// all copies will advance the same stream.
//
// Thus in summary output iterators can be used with the following operations.
//
//  expression     effect
//  ----------     ------
//  *iter=val      write to where the iterator refers (note that iter is an lvalue).
//  ++iter         step forward (return new position)
//  iter++         step forward (return old position)
//  TYPE(iter)     copy construct new iterator
//
// note: the prefix increment operation is always a little faster than the postfix because the
// postfix requires the old position of the iterator to be returned which requires a temporary
// to be created. Thus is you don't need the old position, prefer the prefix operator.
//
// The best example of a pure output iterator I can find is that of the std::ostream_iterator
// iterator adaptor. This iterator allows writing to general output streams via STL algorithms.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>    // for the ostream_iterator
#include <fstream>

//
// A possible implementation of the copy algorithm from the STL.
//   source: https://en.cppreference.com/w/cpp/algorithm/copy
//
// note how the algorithm is written directly to the InputIterator and OutputIterator
// specification where InputIterator is a LegacyInputIterator and OutputIterator is a 
// LegacyOutputIterator, which are named requirements (i.e. iterator specifications) see,
//    https://en.cppreference.com/w/cpp/named_req/OutputIterator
//    https://en.cppreference.com/w/cpp/named_req/InputIterator
//    https://en.cppreference.com/w/cpp/named_req
//
template<typename InputIterator, typename OutputIterator>
OutputIterator mycopy(InputIterator from_pos, 
                      InputIterator from_end, 
                      OutputIterator to_pos)
{
  while(from_pos != from_end)
    *to_pos++ = *from_pos++;
  return to_pos;
}

int main()
{

  // see, 
  //  https://en.cppreference.com/w/cpp/iterator/ostream_iterator
  //
  // ostream iterators are defined for a certain element type as,
  //
  // template<typename T,
  //          typename charT = char,
  //          typename traits = char_traits<charT>>
  // class ostream_iterator;
  //
  //                     v--- the type to write to the stream
  std::ostream_iterator<int> iwriter(std::cout, " < ");
  //              the stream to write to --^      ^--- the delimiter between writes
  //
  // ostream_iterators use the LegacyOutputIterator interface in an interesting way as most
  // of the operations are actually no-ops. All the iterator does is implement the assignment
  // operation of the iterator as a write to the stream. The expression are detailed below,
  //
  // expression                          effect
  // ----------                          ------
  // ostream_iterator<T>(ostream)        constructs an ostream iterator for the stream ostream
  // ostream_iterator<T>(ostream, delim) same except delim is output between writes.
  // *iter                               no-op (just returns iter)
  // iter=value                          does ostream<<value
  // ++iter                              no-op (just returns iter)
  // iter++                              no-op (just returns iter)
  //
  // Note that the reason the no-ops are required and why they must return iter is because the
  // STL algorithms will use these operations, thus they must be present. For example, for 
  // general iterators, assignments to the element an iterator refers to is done like,
  //    *iter=value
  //      ^--- deref iter to get element
  // thus any algorithm which does this, in order to work also for ostream_iterators, said
  // iterators must return iter, so *iter=value becomes iter=value, which does ostream<<value.

  //
  // Now to some code...
  //
  // some of this code is from The C++ Standard Library by Nicolai M. Josuttis, P460.
  //

  //
  // note that all this code really does is,
  //   std::cout << 1 << " < ";
  //   std::cout << 2 << " < ";
  //   std::cout << 3 << " < ";
  //
  *iwriter = 1;
  ++iwriter;     // <-- these are no-ops that do nothing. Remeber some algorithms like our
  *iwriter = 2;  //     copy will need to do this for other non-ostream iterators thus why
  ++iwriter;     //     we need the no-op
  *iwriter = 3;

  std::vector<int> collection = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  //
  // the compiler will instantiate our copy template function for our vector iterator and the
  // iwrite iterator. The vector iterator satisfies the requirements of LegacyInputIterator and
  // the iwriter iterator satisfies the requirements of LegacyOutputIterator, thus this will
  // compile without error.
  //
  mycopy(collection.begin(), collection.end(), iwriter);

  //
  // we can also do this for other ostreams like file streams.
  //

  std::ofstream ofs {"junk.txt"};
  std::ostream_iterator<int> fwriter {ofs, " ## :) ## "};
  mycopy(collection.begin(), collection.end(), fwriter);
}
