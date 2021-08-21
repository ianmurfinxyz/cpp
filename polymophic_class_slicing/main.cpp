#include <iostream>
#include <string>

/**
 * Slicing of Polymorphic Objects
 * ------------------------------
 *
 * The memory layout of c++ polymorphic objects is such that the base
 * class is layed out in RAM first, and then the derived classes of 
 * 'tacked-on' afterwards, like:
 *
 *    +---------------+
 *    | base mem      |
 *    +---------------+
 *    | derived0 mem  |
 *    +---------------+
 *    | derived1 mem  |
 *    +---------------+
 *    |   ...         |
 *    +---------------+
 *    | derivedN mem  |
 *    +---------------+
 *
 * To slice a polymorphic object, means to copy some of it's memory, but
 * not all of it, e.g. to copy the base memory but not the derived memories.
 *
 * When does this occur?
 *
 * Whenever you copy derived class into an instance of a base class. Like in
 * the example below.
 */

class B 
{
public:
  virtual char getChar() {return 'B';}
};

class D: public B
{
public:
  char getChar() override {return 'D';}
};

// 2. The copy operation will only copy the B portion of the memory.
void foo(B b)
{
  // 3. This will print 'B' not 'D' because the D portion of memory was
  // not copied.
  std::cout << b.getChar() << std::endl;
}

//int main()
//{
//  D d{};
//  foo(d); // 1. Here we copy an instance of D, to create an instance of B.
//}

//////////////////////////////////////////////////////////////////////////////////////////////////

// this test demonstrates slicing.

class C
{
public:
  C(const std::string& cstr): _cstr{cstr}{}
  virtual std::string str() {return _cstr;}
protected:
  std::string _cstr;
};

class E : public C
{
public:
  E(const std::string& cstr, const std::string& estr): _estr{estr}, C{cstr} {}
  std::string str() override {return _cstr + _estr;}
protected:
  std::string _estr;
};

// Because we copy an 'E' instance into boo, creating not another 'E' but a base
// class, we slice 'E', copying only the 'C' memory of the 'E' instance.
void boo(C c)
{
  std::cout << "enter boo" << std::endl;
  std::cout << "sizeof(local c)=" << sizeof(c) << std::endl; // 40 bytes
  std::cout << c.str() << std::endl;
  std::cout << "exit boo" << std::endl;
}

void boo_move(C c)
{
  std::cout << "enter boo_move" << std::endl;
  std::cout << "sizeof(local c)=" << sizeof(c) << std::endl; // 40 bytes
  std::cout << c.str() << std::endl;
  std::cout << "exit boo_move" << std::endl;
} // the 'C' portion will be deleted when it goes out of scope.

int main()
{
  C c{"hellooooooooooooooooooo   "};
  E e{"hellooooooooooooooooooo   ", "worlddddddddddddddddddddddddddddddddddddd "};

  std::cout << "sizeof(c)=" << sizeof(c) << std::endl; // 40 bytes
  std::cout << "sizeof(e)=" << sizeof(e) << std::endl; // 72 bytes

  std::cout << c.str() << std::endl;
  std::cout << e.str() << std::endl;

  boo(e);

  // we move the resources of e's base into boo_move, slicing 'e', since the 
  // resources in the derived portion are not moved.
  boo_move(std::move(e));

  // this now only prints "worlddddddddddddddddd..." because the memory of the base
  // class was freed when boo_move returned. Hence the base porition string '_cstr'
  // is now the empty string "".
  std::cout << e.str() << std::endl;
}

// Slicing is the reason for C.67 in the cpp core guidelines:
//  "A polymorphic class should suppress public copy/move"
//
// see: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c67-a-polymorphic-class-should-suppress-public-copymove
