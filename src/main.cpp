#include "kdtree.hpp"
#include <iostream>
#include <vector>

int main() {
  std::cout << "Hello, kdtree world!" << std::endl;

  std::vector<int> vec = {1, 5, 3, 9, 10, 12, 4, 5, 11, 13, 3, 2, 52};

  kdtree::quickSelect(vec.begin(), vec.end(), vec.begin() + 4,
                      std::less<int>());

  std::vector<int> vec2 = {1, 5, 3};
  kdtree::quickSelect(vec2.begin(), vec2.end(), vec2.begin() + 4,
                      std::less<int>());
  kdtree::quickSelect(vec2.begin(), vec2.end(), vec2.begin(), std::less<int>());
}
