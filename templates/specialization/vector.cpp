#include <iostream>

template<typename T>
struct Vector
{
public:

  Vector(T x, T y) : _x{x}, _y{y}
  {}

  friend std::ostream& operator<<(std::ostream& os, const Vector& v)
  { os << "Vector&={" << v._x << "," << v._y << "}"; return os;}

  friend const Vector operator+(Vector a, Vector b)
  { return {a._x + b._x, a._y + b._y};}

  const T& getX() const
  { return _x; }

  const T& getY() const
  { return _y;}


private:
  T _x;
  T _y;
};

//
// Explicit specialisation with float; both an interface specialization as it changes the
// interface (25.3.1 BOOK[0]) and an implementation specialization as it changes the 
// implementation (25.3.1.1 BOOK[0]).
//
// We change the implementation for floats so it returns member data by value rather than
// reference which is more optimal for simple built-in data types.
//
template<>
struct Vector<float>
{
public:

  Vector(float x, float y) : _x{x}, _y{y}
  {}

  friend std::ostream& operator<<(std::ostream& os, Vector v)
  { os << "Vector={" << v._x << "," << v._y << "}"; return os;}

  friend const Vector operator+(Vector a, Vector b)
  { return {a._x + b._x, a._y + b._y};}

  float getX() const
  { return _x; }

  float getY() const
  { return _y; }

private:
  float _x;
  float _y;
};


int main()
{
  Vector<float> fv {2.f, 4.f};
  Vector<int> iv {6, 7};

  std::cout << fv << std::endl; // outputs Vector={2,4} i.e. uses the specialization
  std::cout << iv << std::endl; // outputs Vector&={6,7} i.e. uses the primary template
}
