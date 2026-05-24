#include <array>
#include <cuchar>
#include <iostream>
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
  // (underlying `std::array` is already defined so this just works)
  bool operator<(const Point &other) const { return data_ < other.data_; }
};

// Divide a container into two partitions
template <typename RandIter, typename Comparator>
RandIter partition(RandIter begin, RandIter end, Comparator cmp) {
  RandIter pivot = end; // always choose last element as pivot
  --pivot;              // remember end points to 1 PAST the end

  RandIter pivotIndex = begin; // temporary pivot index

  for (auto it = begin; it != pivot; ++it) {
    if (cmp(*it, *pivot) || *it == *pivot) {
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

  RandIter pivotIndex = partition(begin, end, cmp);
  if (pivotIndex == k) {
    std::cout << "k-th smallest value is: " << *k << "\n";
    return;
  } else if (pivotIndex < k) {
    quickSelect(pivotIndex + 1, end, k, cmp);
  } else {
    quickSelect(begin, pivotIndex, k, cmp);
  }
}

// `smallerDimensionValue` returns a truth value of if the first `Point` has a
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
  KDTree(const std::vector<Point<Dim, T>> &newPoints);
  ~KDTree();
  // TODO: What other operations on a KDTree should there be (Rule of 0 or 5?)

  // TODO: helpers
  void insert();
  void remove();

  // Find the closest point to the point `query` in the `KDTree`.
  Point<Dim, T> findNearestNeighbor(const Point<Dim, T> &query) const;
  // Print the KDTree.
  void printTree(std::ostream &out = std::cout) const;

private:
};
} // namespace kdtree
