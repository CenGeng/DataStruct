#include <iostream>

// #include "./List/list.h"
#include "./BinTree/BinTree.h"
using namespace std;
void printfList(int data);

int main() {
  //  BinTree<int> tree;
  //  tree.insertAsLC(tree.root(),5);
  //  cout << tree.empty() << endl;
  BinTree<int> tree;
  tree.insertAsRoot(1);
  tree.insertAsLC(tree.root(), 2);
  tree.insertAsRC(tree.root(), 3);
  tree.insertAsLC(tree.root()->lc, 4);
  tree.insertAsLC(tree.root()->rc, 5);
  //    tree.travLevel(printfList);
  tree.root()->travPre(printfList);

  return 0;
}
void printfList(int data) { cout << data << endl; }
