#include <iostream>

// #include "../Queue/Queue.h"
#include "BinNodeMacro.h"
#define BinNodePosi(T) BinNode<T>*  // 节点位置
#define stature(p) \
  ((p) ? (p)->height : -1)  // 节点高度(与“空树高度为-1”的约定相统一)
typedef enum { RB_RED, RB_BLACK } RBColor;  // 节点颜色

template <typename T>
struct BinNode {  // 二叉树节点模板
  // 成员(为简化描述起见统一开放,读者可根据需要进一步封装)
  T data;  // 数值
  BinNodePosi(T) parent;
  BinNodePosi(T) lc;
  BinNodePosi(T) rc;  // 父节点及左、右孩子
  int height;         // 高度(通用)
  int npl;        // Null Path Length(左式堆, 也可直接用height代替)
  RBColor color;  // 颜色(红黑树)
  // 构造函数
  BinNode()
      : parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
  BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL,
          BinNodePosi(T) rc = NULL, int h = 0, int l = 1, RBColor c = RB_RED)
      : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
  // 操作接口
  int size();  // 统计当前节点后代总数, 亦即以其为跟的子树的规模
  BinNodePosi(T) insertAsLc(T const&);  // 作为当前节点的左孩子插入新节点
  BinNodePosi(T) insertAsRc(T const&);  // 作为当前节点的右孩子插入新节点
  BinNodePosi(T) succ();                // 取当前节点的直接后继
  template <typename VST>
  void travLevel(VST&);  // 子树层次遍历
  template <typename VST>
  void travPre(VST&);  // 子树先序遍历
  template <typename VST>
  void travIn(VST&);  // 子中序遍历
  template <typename VST>
  void travPost(VST&);  // 子后序遍历
  // 比较器、判等器(各列其一,其余自行补充)
  bool operator<(BinNode const& bn) { return data < bn.data; }    // 小于
  bool operator==(BinNode const& bn) { return data == bn.data; }  // 等于
};
/// 遍历服务支持函数的声明
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit);
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit);
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST& visit);

/// BinNode类中函数声明的实现
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLc(T const& e) {
  return lc = new BinNode(e, this);  // 将e作为当前节点的左孩子插入二叉树
}
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRc(T const& e) {
  return rc = new BinNode(e, this);  // 将e作为当前节点的右孩子插入二叉树
}
/*template <typename T>
template <typename VST>                // 元素类型、操作器
void BinNode<T>::travIn(VST& visit) {  // 二叉树中序遍历算法统一入口
  switch (rand() % 5) {  // 此处暂随机选择做测试,共五种选择
    case 1:
      travIn_I1(this, visit);  // 迭代版#1
      break;
    case 2:
      travIn_I2(this, visit);  // 迭代版#2
      break;
    case 3:
      travIn_I3(this, visit);  // 迭代版#3
      break;
    case 4:
      travIn_I4(this, visit);  // 迭代版#4
      break;
    default:
      travIn_R(this, visit);  // 递归版
      break;
  }
}*/
template <typename T>
int BinNode<T>::size() {    // 统计当前节点后代总数
  int s = 1;                // 计入本身
  if (lc) s += lc->size();  // 递归计入左子树规模
  if (rc) s += rc->size();  // 递归计入右子树规模
  return s;
}
template <typename T>
template <typename VST>  // 元素类型、操作器
void BinNode<T>::travPre(VST& visit) {
  travPre_R(this, visit);
}
template <typename T>
template <typename VST>  // 元素类型、操作器
void BinNode<T>::travPost(VST& visit) {
  travPost_R(this, visit);
}
template <typename T>
template <typename VST>  // 元素类型、操作器
void BinNode<T>::travIn(VST& visit) {
  travIn_R(this, visit);
}
template <typename T>
BinNodePosi(T) BinNode<T>::succ() {  // 定位节点v的直接后继
  BinNodePosi(T) s = this;           // 记录后继的临时变量
  if (rc) {  // 若有右孩子,则直接后继必在右子树中,具体地就是
    s = rc;                           // 右子树中
    while (HasLChild(*s)) s = s->lc;  // 最靠左(小)的节点
  } else {  // 否则,直接后继应是“将当前节点包含于其左子树中最低祖先”,具体地就是
    while (IsRChild(*s)) s = s->parent;  // 逆向地沿右向分支,不断朝左上方移动
    s = s->parent;  // 最后再朝右上方移动一步, 即抵达直接后继(如果存在)
  }
  return s;
}

/// 遍历服务支持函数的声明的实现
template <typename T, typename VST>             // 元素类型、操作器
void travPre_R(BinNodePosi(T) x, VST& visit) {  // 二叉树先序遍历算法(递归版)
  if (!x) return;
  visit(x->data);
  travPre_R(x->lc, visit);
  travPre_R(x->rc, visit);
}
template <typename T, typename VST>              // 元素类型、操作器
void travPost_R(BinNodePosi(T) x, VST& visit) {  // 二叉树后序遍历算法(递归版)
  if (!x) return;
  travPost_R(x->lc, visit);
  travPost_R(x->rc, visit);
  visit(x->data);
}
template <typename T, typename VST>            // 元素类型、操作器
void travIn_R(BinNodePosi(T) x, VST& visit) {  // 二叉树中序遍历算法(递归版)
  if (!x) return;
  travIn_R(x->lc, visit);
  visit(x->data);
  travIn_R(x->rc, visit);
}

// template <typename T>
// template <typename VST>  // 元素类型、操作器
// void BinNode<T>::travLevel(VST& visit) {
//   Queue<BinNodePosi(T)> Q;  // 辅助队列
//   Q.enqueue(this);          // 根节点入队
//   while (!Q.empty) {        // 在队列再次变空之前,反复迭代
//     // 取出队首节点并访问之
//     BinNodePosi(T) x = Q.dequeue;
//     visit(x->data);
//     if (HasLChild(*x)) Q.enqueue(x->lc);  // 左孩子入队
//     if (HasRChild(*x)) Q.enqueue(x->rc);  // 右孩子入队
//   }
// }
