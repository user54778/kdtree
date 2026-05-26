#include "kdtree.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Basic KD-tree", "[kdtree]") {
  std::vector<kdtree::Point<2>> points = {
      {51, 35}, {44, 43}, {84, 44}, {28, 10}, {43, 65}, {60, 30}, {88, 72},
      {14, 15}, {48, 0},  {42, 63}, {44, 79}, {59, 0},  {74, 0},  {54, 62},
      {95, 50}, {34, 15}, {49, 83}, {82, 20}, {75, 68}, {96, 56}};
  kdtree::KDTree<2> tree2(points);
}
