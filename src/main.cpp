#include "kdtree.hpp"
#include <vector>

int main() {
  std::vector<kdtree::Point<2, double>> points = {
      {7.0, 2.0}, {5.0, 4.0}, {9.0, 6.0}, {2.0, 3.0}, {4.0, 7.0}, {8.0, 1.0}};
  kdtree::KDTree<2> tree(points);

  tree.printTree();

  kdtree::Point<2, double> p{6.0, 3.0};
  tree.findNearestNeighbor(p);
  /*
  std::cout << "Hello, kdtree world!" << std::endl;

  std::vector<kdtree::Point<3>> points = {{1, 2, 3}, {3, 2, 1}, {2, 3, 1}};

  kdtree::KDTree<3> tree(points);
  tree.printTree();

  std::cout << "\n\n";
  */

  /*
  std::vector<kdtree::Point<2>> ex1 = {{30, 40}, {5, 25},  {10, 12},
                                       {70, 70}, {50, 30}, {35, 45}};
  kdtree::KDTree<2> tree1(ex1);
  tree1.printTree();
  */

  /*
  std::vector<kdtree::Point<2>> points = {
      {51, 35}, {44, 43}, {84, 44}, {28, 10}, {43, 65}, {60, 30}, {88, 72},
      {14, 15}, {48, 0},  {42, 63}, {44, 79}, {59, 0},  {74, 0},  {54, 62},
      {95, 50}, {34, 15}, {49, 83}, {82, 20}, {75, 68}, {96, 56}};
  kdtree::KDTree<2> tree2(points);
  tree2.printTree();
  */
  /*
  std::vector<int> vec = {1, 5, 3, 9, 10, 12, 4, 5, 11, 13, 3, 2, 52};

  kdtree::quickSelect(vec.begin(), vec.end(), vec.begin() + 4,
                      std::less<int>());

  std::vector<int> vec2 = {1, 5, 3};
  kdtree::quickSelect(vec2.begin(), vec2.end(), vec2.begin() + 4,
                      std::less<int>());
  kdtree::quickSelect(vec2.begin(), vec2.end(), vec2.begin(), std::less<int>());
  */

  /*
  kdtree::Point<3> a{1, 2, 3};
  kdtree::Point<3> b{3, 2, 1};

  std::cout << kdtree::smallerDimensionValue(a, b, 0) << std::endl;
  std::cout << kdtree::smallerDimensionValue(a, b, 2) << std::endl;
  std::cout << kdtree::smallerDimensionValue(a, b, 1) << std::endl;
  */
  /*
  std::cout << "\n\n";

  kdtree::Point<3> target{1, 3, 5};
  kdtree::Point<3> currentBest1{1, 3, 2};
  kdtree::Point<3> possibleBest1{2, 4, 4};
  kdtree::Point<3> currentBest2{1, 3, 6};
  kdtree::Point<3> possibleBest2{2, 4, 4};
  kdtree::Point<3> currentBest3{0, 2, 4};
  kdtree::Point<3> possibleBest3{2, 4, 6};
  std::cout << shouldReplace(target, currentBest1, possibleBest1)
            << std::endl; // should print true
  std::cout << shouldReplace(target, currentBest2, possibleBest2)
            << std::endl; // should print false
  std::cout << shouldReplace(target, currentBest3, possibleBest3)
            << std::endl; // based on operator<, this should be false!!!
  */
}
