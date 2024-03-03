#include "../BST/BST.h"  //基于BST实现RedBlack

#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color))  // 外部节点也视作黑节点
#define IsRed(p) (!isblank(p))                         // 非黑即红
#define BlackHeightUpdated(x) /*RedBlack高度更新条件*/ \
  ((stature((x).lc)) == stature((x).rc) &&                   \
   ((x).height == (IsRed(&x) ? stature((x).lc) : stature((x).lc) + 1)))
template <typename T>
class RedBlack : public BST<T> {  // RedBlack树模板类
 protected:
  using BST<T>::_hot;
  using BST<T>::_size;
  using BST<T>::_root;
  void solveDoubleRed(BinNodePosi(T) x);    // 双红修正
  void solveDoubleBlack(BinNodePosi(T) x);  // 双黑修正
  int updateHeight(BinNodePosi(T) x);       // 更新节点x的高度
 public:
  BinNodePosi(T) insert(const T& e);  // 插入(重写)
  bool remove(const T& e);            // 删除(重写)
  // BST::search()等其余接口可直接沿用
};

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x) {  // 更新节点高度
  x->height =
      max(stature(x->lc), stature(x->rc));  // 孩子一般黑高度相等,除非出现双黑
  return IsBlack(x) ? x->height++ : x->height;  // 若当前节点为黑,则计入黑深度
}  // 因统一定义stature(NULL) =-1,
   // 故height比黑高度少一,好在不致影响到各种算法中的比较判断

template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T& e) {  // 将e插入红黑树
  // 确认目标不存在(留意对_hot的设置)
  BinNodePosi(T)& x = search(e);
  if (x) return x;
  // 创建红节点x:以_hot为父,黑高度-1
  x = new BinNode<T>(e, _hot, NULL, NULL, -1);
  _size++;
  // 经双红修正后,即可返回
  solveDoubleRed(x);
  return x ? x : _hot->parent;
}  // 无论e是否存在于原树中,返回时总有x->data == e
/******************************************************************************************
 * RedBlack双红调整算法：解决节点x与其父均为红色的问题。分为两大类情况：
 *    RR-1：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 *    RR-2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
 ******************************************************************************************/
template <typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x) {  // x当前必为红
  if (IsRoot(*x))  // 若已（递归）转至树根，则将其转黑，整树黑高度也随之递增
  {
    _root->color = RB_BLACK;
    _root->height++;
    return;
  }  // 否则，x的父亲p必存在
  BinNodePosi(T) p = x->parent;
  if (IsBlack(p)) return;  // 若p为黑，则可终止调整。否则
  BinNodePosi(T) g = p->parent;  // 既然p为红，则x的祖父必存在，且必为黑色
  BinNodePosi(T) u = uncle(x);  // 以下，视x叔父u的颜色分别处理
  if (IsBlack(u)) {             // u为黑色（含NULL）时
    if (IsLChild(*x) == IsLChild(*p))  // 若x与p同侧（即zIg-zIg或zAg-zAg），则
      p->color = RB_BLACK;             // p由红转黑，x保持红
    else                    // 若x与p异侧（即zIg-zAg或zAg-zIg），则
      x->color = RB_BLACK;  // x由红转黑，p保持红
    g->color = RB_RED;      // g必定由黑转红
    ///// 以上虽保证总共两次染色，但因增加了判断而得不偿失
    ///// 在旋转后将根置黑、孩子置红，虽需三次染色但效率更高
    BinNodePosi(T) gg = g->parent;  // 曾祖父（great-grand parent）
    BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);  // 调整后的子树根节点
    r->parent = gg;                                     // 与原曾祖父联接
  } else {                                              // 若u为红色
    p->color = RB_BLACK;
    p->height++;  // p由红转黑
    u->color = RB_BLACK;
    u->height++;  // u由红转黑
    g->color = RB_RED;  // 在B-树中g相当于上交给父节点的关键码，故暂标记为红
    solveDoubleRed(
        g);  // 继续调整：若已至树根，接下来的递归会将g转黑（尾递归，不难改为迭代）
  }
}

template <typename T>
bool RedBlack<T>::remove(const T& e) {  // 从红黑树中删除关键码e
  BinNodePosi(T)& x = search(e);
  if (!x) return false;  // 确认目标存在(留意hot的设置)
  BinNodePosi(T) r = removeAt(x, _hot);
  if (!(--_size)) return true;  // 实施删除
  // assert:_hot某一孩子刚被删除,且被r所指节点(可能是NULL)接替.以下检查是否平衡,并做必要调整
  if (!_hot) {  // 若刚被删除的是根节点,则将其置黑,并更新黑高度
    _root->color = RB_BLACK;
    updateHeight(_root);
    return true;
  }
  // assert:以下,原x(现r)必非根,_hot必非空
  if (BlackHeightUpdated(*_hot))
    return true;   // 若所有祖先的黑深度依然平衡,则无序调整
  if (IsRed(r)) {  // 否则,若r为红,则只需要令其转黑
    r->color = RB_BLACK;
    r->height++;
    return true;
  }
  // assert:以下,原x(现r)均为黑色
  // 经双黑调整后返回
  solveDoubleBlack(r);
  return true;
}  // 若目标节点存在且被删除,返回true;否则返回false

/******************************************************************************************
 * RedBlack双黑调整算法：解决节点x与被其替代的节点均为黑色的问题
 * 分为三大类共四种情况：
 * BB-1 ：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 * BB-2R：2次颜色翻转，2次黑高度更新，0次旋转，不再递归
 * BB-2B：1次颜色翻转，1次黑高度更新，0次旋转，需要递归
 * BB-3 ：2次颜色翻转，2次黑高度更新，1次旋转，转为BB-1或BB2R
 ******************************************************************************************/
template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r) {
  BinNodePosi(T) p = r ? r->parent : _hot;
  if (!p) return;                                   // r的父亲
  BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;  // r的兄弟
  if (IsBlack(s)) {                                 // 兄弟s为黑
    BinNodePosi(T) t =
        NULL;  // s的红孩子（若左、右孩子皆红，左者优先；皆黑时为NULL）
    if (IsRed(s->rc)) t = s->rc;  // 右子
    if (IsRed(s->lc)) t = s->lc;  // 左子
    if (t) {                      // 黑s有红孩子：BB-1
      RBColor oldColor =
          p->color;  // 备份原子树根节点p颜色，并对t及其父亲、祖父
      // 以下，通过旋转重平衡，并将新子树的左、右孩子染黑
      BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);  // 旋转
      if (HasLChild(*b)) {
        b->lc->color = RB_BLACK;
        updateHeight(b->lc);
      }  // 左子
      if (HasRChild(*b)) {
        b->rc->color = RB_BLACK;
        updateHeight(b->rc);
      }  // 右子
      b->color = oldColor;
      updateHeight(b);  // 新子树根节点继承原根节点的颜色
    } else {            // 黑s无红孩子
      s->color = RB_RED;
      s->height--;            // s转红
      if (IsRed(p)) {         // BB-2R
        p->color = RB_BLACK;  // p转黑，但黑高度不变
      } else {                // BB-2B
        p->height--;          // p保持黑，但黑高度下降
        solveDoubleBlack(p);  // 递归上溯
      }
    }
  } else {  // 兄弟s为红：BB-3
    s->color = RB_BLACK;
    p->color = RB_RED;                                // s转黑，p转红
    BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc;  // 取t
    _hot = p;
    FromParentTo(*p) = rotateAt(t);  // 对t及其父亲、祖父做平衡调整
    solveDoubleBlack(r);             // 继续修正r处双黑
  }
}
