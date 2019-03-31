#include <iostream>
#include <limits>
#include <safe/safe_cast.hpp>

int64_t fl (int count) {
  using namespace safe;

  int64_t sum = 0;
  for(int i = 0; i < count; i++) {
    sum += safe_cast<unsigned char>(i);
  }
  return sum;
}


int main()
{
  int count = rand() % 500000;

  int64_t a = fl(count);
  std::cout << a + 5 << std::endl;

  return 0;
}
