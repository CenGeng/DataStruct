#include "_deps/googletest-src/googletest/include/gtest/gtest.h"
#include "BinTree.h" // 假设BinTree.h是二叉树的头文件

TEST(BinTreeTest, UpdateHeightTest) {
  BinTree<int> tree;
  
  // 创建测试用的二叉树
  BinNodePosi(int) root = new BinNode<int>(1);
  BinNodePosi(int) node1 = new BinNode<int>(2);
  BinNodePosi(int) node2 = new BinNode<int>(3);
  BinNodePosi(int) node3 = new BinNode<int>(4);
  
  root->lc = node1;
  root->rc = node2;
  node2->rc = node3;
  
  tree.root = root;
  
  // 调用updateHeight函数
  int height = tree.updateHeight(root);
  
  // 验证结果是否符合预期
  EXPECT_EQ(height, 3);
  EXPECT_EQ(root->height, 3);
  EXPECT_EQ(node1->height, 1);
  EXPECT_EQ(node2->height, 2);
  EXPECT_EQ(node3->height, 1);
  
  // 释放内存
  delete root;
  delete node1;
  delete node2;
  delete node3;
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
