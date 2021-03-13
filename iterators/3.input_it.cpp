//
// INPUT ITERATORS
//
// Input iterators can only step forward element-by-element with read access. They are the 
// opposite of output iterators.
//
// However unlike output iterators input iterators can be compared. But, these comparisons
// are only provided to allow comparison with a past-the-end iterator. There is no requirement
// that two non-past-the-end input iterators compare equal if they are direct copies, or unequal
// if they refer to different positions.
//
// Input iterators can only read a value once (think of input streams like std::cin), thus
// if you create a copy of an input iterator and let the copy and the original read values
// they may read different values.
//
// All operations for input iterators are as follows,
//
// expression     effect
// ----------     ------
// *iter          read the element refered to by iter (must be an rvalue)
// iter->member   read a member of the element refered to (must be an rvalue)
// ++iter         step forward
// iter++         step forward
// iter1==iter2   equality comparison (guarantees only made for compare with past-the-end)
// iter1!=iter2   no equal comparison (guarantees only made for compare with past-the-end)
// TYPE(iter)     copy constructor
//
// note that there are more operations for input iterators than output iterators because reading
// requires you to know when the source of data is exhausted. You also may need to read members
// of the data.
//
// Like output iterators there is a pure input iterator in the form of an iterator adaptor, 
// for input iterators said adaptor is the std::istream_iterator adaptor (the counterpart to
// std::ostream_iterator).

#include <iostream>
#include <iterator> // for std::istream_iterator
#include <vector>

int main()
{
  //
  // An important difference between std::ostream_iterator and std::istream_iterator is that
  // the latter has a default constructor which constructs a past-the-end iterator used to
  // test when the data source is exhausted.
  //
  // Further there are no no-ops with istream_iterators; the increment operations do actaully
  // advance the stream and should be used and the dereference operation does actually read
  // a value.
  //
  std::istream_iterator<int> ireader {std::cin};

  //
  // make a past-the-end iterator with the default constructor.
  //
  std::istream_iterator<int> end {};

  std::vector<int> idata {};

  //
  // if a read fails then every istream_iterator becomes an end-of-stream iterator, thus
  // the following loop will end when either we get to the end of the input source or we
  // get an error in the reading.
  //
  // Thus running this program just on its own like (from a terminal),
  //
  // ./<name-of-program>
  //
  // will cause the program to loop infinitely asking for an integer. This loop will continue
  // until we provide a non-integer, e.g. entering 1 then 2 then 3 then hello, will cause the
  // program to output 1 then 2 then 3
  //
  // If I pipe some integers into the program like,
  //
  // echo 1 | ./<name-of-program>
  //
  // it will just print 1, i.e. it reaches the end of the input source.
  //
  while(ireader != end){
    idata.push_back(*ireader);
    ++ireader;
  }

  for(auto& i : idata)
    std::cout << i << std::endl;
}

