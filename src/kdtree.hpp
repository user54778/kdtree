#include <algorithm>
#include <array>
#include <cuchar>
#include <iostream>
#include <memory>
#include <vector>

namespace kdtree {

// Point defines a simple `Point` type that is really a wrapper
// over a `std::array`. The type defines a size N
// and the type of data T, where the Nth value represents a point
// in N-dimensional space.
template <std::size_t N, typename T = double> struct Point {
  std::array<T, N> data_;

  Point(const std::array<T, N> &data) { data = data_; }

  // Called "Perfect Forwarding", used to pass a function's parameters to
  // another function while preserving its reference category. I.e., pass
  // parameter to another function (a constructor)
  template <typename... Args>
  Point(Args &&...args)
      : data_{static_cast<double>(std::forward<Args>(args))...} {}

  // Define the `[]` operator for coordinate access
  T &operator[](std::size_t i) { return data_[i]; }
  const T &operator[](std::size_t i) const { return data_[i]; }

  // Overload `<` for comparison between points
  // (underlying `std::already` is already defined so this just works)
  bool operator<(const Point &other) const { return data_ < other.data_; }
};

template <std::size_t Dim, typename T>
std::ostream &operator<<(std::ostream &out, const Point<Dim, T> &p) {
  out << "(";
  for (auto i = 0; i < Dim; i++) {
    out << p.data_[i];
    if (i < Dim - 1) {
      out << ", ";
    }
  }
  out << ")";
  return out;
}

// Divide a container into two partitions
template <typename RandIter, typename Comparator>
RandIter partitionSelect(RandIter begin, RandIter end, Comparator cmp) {
  using std::swap;

  RandIter pivot = end; // always choose last element as pivot
  --pivot;              // remember end points to 1 PAST the end

  RandIter pivotIndex = begin; // temporary pivot index

  for (auto it = begin; it != pivot; ++it) {
    if (!cmp(*pivot, *it)) {
      // swap A[i] with A[j]
      // i.e., curr ele w/ ele at temp pivotIndex
      std::swap(*it, *pivotIndex);
      ++pivotIndex;
    }
  }
  std::swap(*pivotIndex, *pivot);
  return pivotIndex;
}

// quickSelect finds the kth smallest element in an unordered list.
// It partitions the given range such that the k-th element is in the k-th
// position, and all elements that compare as LESS
// are to the left, and all LARGER elements to the right.
template <typename RandIter, typename Comparator>
void quickSelect(RandIter begin, RandIter end, RandIter k, Comparator cmp) {
  // RandIter and Comparator are simply the template names.
  // The idea is, RandIter is a (should be) an iterator type itself.
  // You pass in three separate iterators, a `begin`, `end` and `k`.
  // These represent that start of the range, end, and element s.t. k
  // is the position where the kth element should end up.
  //
  // Cmp is simply the comparison operation; for our case, should strictly be
  // `less`, but can really be any custom comparator.
  if (begin >= end) {
    std::cout << "k is invalid" << "\n";
    return;
  }

  RandIter pivotIndex = partitionSelect(begin, end, cmp);
  if (pivotIndex == k) {
    return;
  } else if (pivotIndex < k) {
    quickSelect(pivotIndex + 1, end, k, cmp);
  } else {
    quickSelect(begin, pivotIndex, k, cmp);
  }
}

// `smallerDimensionValue` returns a bool of if the first `Point` has a
// smaller value than the second in the dimension `currDim`.
template <std::size_t Dim, typename T = double>
bool smallerDimensionValue(const Point<Dim, T> &first,
                           const Point<Dim, T> &second, std::size_t currDim) {

  if (first.data_[currDim] < second.data_[currDim]) {
    return true;
  } else if (first.data_[currDim] > second.data_[currDim]) {
    return false;
  } else {
    return first < second;
  }
}

// Take three `Point`s in and return `true` if the `potential` point is closer
// to `target` than the `currentBest` point.
template <std::size_t Dim, typename T = double>
bool shouldReplace(const Point<Dim, T> &target,
                   const Point<Dim, T> &currentBest,
                   const Point<Dim, T> &potential) {
  return computeDist(potential, target) < computeDist(currentBest, target);
}

// computeDist is a helper function to compute the Euclidean distance between
// two `Point`s.
template <std::size_t Dim, typename T = double>
T computeDist(const Point<Dim, T> &p, const Point<Dim, T> &q) {
  T dist = 0;

  for (std::size_t i = 0; i < Dim; i++) {
    T diff = p.data_[i] - q.data_[i];
    T sq = diff * diff;
    dist += sq;
  }
  return dist;
}

template <std::size_t Dim, typename T = double> class KDTree {
private:
  struct Node {
    Point<Dim, T> point;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };

  std::unique_ptr<Node> root_;

  // Print a KDTree.
  void printTree(const std::unique_ptr<Node> &node, int depth, char label,
                 std::ostream &out = std::cout) const {
    if (node == nullptr) {
      return;
    }
    printTree(node->left, depth + 1, 'L', out);
    out << std::string(depth * 4, ' ') << label << ": " << node->point << "\n";
    printTree(node->right, depth + 1, 'R', out);
  }

  // A helper for deep copying a KDTree.
  std::unique_ptr<Node> copyTree(const std::unique_ptr<Node> &node) {
    if (node == nullptr) {
      return nullptr;
    }

    auto currNode = std::make_unique<Node>();
    currNode->point = node->point;
    currNode->left = node->left;
    currNode->right = node->right;
    return currNode;
  }

  // A helper to perform the actual creation of a KDTree.
  // `depth` is the recursion depth we're on, used to properly cycle the axis
  // through all `k` dimensions.
  std::unique_ptr<Node> createTree(std::vector<Point<Dim, T>> &pointList,
                                   int left, int right, int depth) {
    // base case
    if (pointList.empty() || left > right) {
      return nullptr;
    }

    // leaf node
    if (left == right) {
      auto leaf = std::make_unique<Node>();
      leaf->point = pointList[left];
      return leaf;
    }

    int axis = depth % Dim; // dimension to cmp on

    // select MEDIAN by axis from pointList
    int k = (left + right) / 2;

    // comparator for quickSelect to call on the given axis
    auto cmp = [axis](Point<Dim, T> &a, Point<Dim, T> &b) {
      return smallerDimensionValue(a, b, axis);
    };

    // find the median
    quickSelect(pointList.begin() + left, pointList.begin() + right + 1,
                pointList.begin() + k, cmp);

    // recurse
    std::unique_ptr<Node> node = std::make_unique<Node>();
    node->point = pointList[k];
    node->left = createTree(pointList, left, k - 1, depth + 1);
    node->right = createTree(pointList, k + 1, right, depth + 1);

    return node;
  }

  // Returns a leaf of the lowest node within the same splitting plane as
  // target.
  Node *searchLowestNode(const Point<Dim, T> &query,
                         const std::unique_ptr<Node> &node, int depth) const {
    std::cout << node->point << std::endl;
    // initial best
    int axis = depth % Dim; // cmp on this dim
    // TODO: implement me!
  }

public:
  // Construct the KDTree by walking the tree from its `root` and constructing
  // it recursively. A valid KDTree should always be built from a collection of
  // points; an "empty" KDTree doesn't make sense.
  KDTree(const std::vector<Point<Dim, T>> &newPoints) {
    // cycle through `k` dims to split the planes
    // points inserted by `quickSelect`ing the median of the points in
    // that subtree, *based on the k-th dimension we're in*.
    // It should look very similar to constructing a BST, as this is a
    // generalized version of one.
    auto points = newPoints;
    root_ = createTree(points, 0, points.size() - 1, 0);
  }

  // Uses a `unique_ptr` so the tree will automatically cleanup itself upon
  // exiting scope.
  ~KDTree() = default;

  // Copy; no need for move since we use `unique_ptr`
  KDTree(const KDTree &other) { root_ = copyTree(other.root_); }

  KDTree &operator=(const KDTree &other) {
    if (this != &other) {
      root_ = copyTree(other.root_);
    }
    return *this;
  }

  // Find the closest point to the point `query` in the `KDTree`.
  // The closest here is the minimum Euclidean distance between elements.
  Point<Dim, T> findNearestNeighbor(const Point<Dim, T> &query) const {
    // TODO: implement me!

    // starting at the root node, perform dfs on the kd-tree to determine where
    // to start based on the `query`; i.e., dfs to find the lowest node with the
    // *same* splitting plane. [call smallerDimensionValue] This determines
    // which subtree to search.
    //
    // Upon reaching a leaf node, compute the distance (Euclidean) [call
    // shouldReplace] Then *search points within the radius*. If the hypersphere
    // crosses the plane, there could be closer points to the other side of the
    // plane. If not, walk up the tree. I.e., If the distance from the target to
    // split the plane is still within the current radius, search the other
    // subtree.

    auto leaf = searchLowestNode(query, root_, 0);
    std::cout << "leaf: " << leaf->point << std::endl;

    return Point<Dim, T>();
  }

  void printTree(std::ostream &out = std::cout) const {
    printTree(root_, 0, '*', out);
  }
};
} // namespace kdtree
