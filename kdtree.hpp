/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include "point.h"

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
     if (first[curDim] < second[curDim]) {
       return true;
     }
     if (first[curDim] == second[curDim]) {
       return first < second;
     }
    return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
     double sum1 = 0;
     double sum2 = 0;
     for (unsigned i = 0; i < Dim; i++) {
       sum1 += pow((target[i] - currentBest[i]), 2);
       sum2 += pow((target[i] - potential[i]), 2);
     }
     if (sum1 > sum2) {
       return true;
     }
     if (sum1 == sum2) {
       return potential < currentBest;
     }
     return false;
}

template <int Dim>
int KDTree<Dim>::goPartition(int a, int b, int count, vector<Point<Dim>>& newPoints) {

  int start = a;
  int pivot = a;
  for (int i = a + 1; i <= b; i++) {
    if (smallerDimVal(newPoints[i], newPoints[start], count % Dim)) {
      pivot++;
      swap(newPoints[i], newPoints[pivot]);
    }
  }
  swap(newPoints[pivot], newPoints[start]);
  return pivot;
}

template <int Dim>
Point<Dim> KDTree<Dim>::goQuickSelect(int a, int b, int k, int count, vector<Point<Dim>>& newPoints) {
  if (a == b) {
    return newPoints[a];
  }
  int toCompare = goPartition(a, b, count, newPoints);
  if (k == toCompare) {
    return newPoints[k];
  }
  if (k > toCompare) {
    return goQuickSelect(toCompare + 1, b, k, count, newPoints);
  }
  return goQuickSelect(a, toCompare - 1, k, count, newPoints);
}

template <int Dim>
void KDTree<Dim>::buildTree(int a, int b, int count, KDTreeNode *& node, vector<Point<Dim>>& newPoints) {
  if (a > b) {
    // node = new KDTreeNode(newPoints[a]);
    //node->point = newPoints[a];
    return;
  }
  node = new KDTreeNode(goQuickSelect(a, b, floor((double(a + b)) / 2), count, newPoints));
  size++;
  count++;
  buildTree(a, floor((double(a + b)) / 2) - 1, count, node->left, newPoints);
  buildTree(floor((double(a + b)) / 2) + 1, b, count, node->right, newPoints);
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     if (!newPoints.empty()) {
       vector<Point<Dim>> go;
       go = newPoints;
       size =0;
       int a = 0;
       int b = newPoints.size() - 1;
       buildTree(a, b, 0, root, go);

     } else {
       root = NULL;
       size = 0;
   }
}

template <int Dim>
void KDTree<Dim>::rebuild(KDTreeNode *cNode, KDTreeNode *node) {
  cNode = new KDTreeNode(node->point);
  rebuild(cNode->left, node->left);
  rebuild(cNode->right, node->right);
}

template <int Dim>
void KDTree<Dim>::copy(const KDTree<Dim>& rhs) {
  size = rhs.size;
  delete root;
  rebuild(root, rhs.root);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   copy(other);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
   if (this != &rhs) {
     copy(rhs);
   }

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   delete root;
   root = NULL;
}

template <int Dim>
double KDTree<Dim>::distance(const Point<Dim>& a, const Point<Dim>& b) const
{
  double tmp = 0;
  for (int i = 0; i < Dim; i++) {
        tmp += pow((a[i] - b[i]), 2);
      }
  double distance = pow(tmp, 0.5);
  return distance;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    Point<Dim> currentBest = root->point;
    find_helper(root, query, currentBest, 0);
    return currentBest;
}


template <int Dim>
void KDTree<Dim>::find_helper(KDTreeNode* subroot, const Point<Dim>& target, Point<Dim>& currentBest, int d)const
{
  if (subroot == NULL) {
    return;
  } else if (shouldReplace(target, currentBest, subroot->point)) {
    currentBest = subroot->point;
  }
  if (smallerDimVal(target,subroot->point, d)) {
    find_helper(subroot->left, target, currentBest, (d+1)%Dim);
    double radius = distance(currentBest, target);
    if (abs(target[d] - subroot->point[d]) <= radius) {
      find_helper(subroot->right, target, currentBest, (d+1)%Dim);
    }
  } else {
    find_helper(subroot->right, target, currentBest, (d+1)%Dim);
    double radius = distance(currentBest, target);
    if (abs(target[d] - subroot->point[d]) <= radius) {
      find_helper(subroot->left, target, currentBest, (d+1)%Dim);
    }
  }
}
