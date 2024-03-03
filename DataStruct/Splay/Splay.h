#include "../BST/BST.h"  //基于BST实现Splay

template <typename T>
class Splay : public BST<T> {  // 由BST派生的Splay树模板类
 protected:
  BinNodePosi(T) splay(BinNodePosi(T) v);  // 将节点v伸展至根
 public:
  BinNodePosi(T) & search(const T& e);  // 查找(重写)
  BinNodePosi(T) insert(const T& e);    // 插入(重写)
  bool remove(const T& e);              // 删除(重写)
};

template <typename NodePosi>  // 在节点*p与*lc(可能为空)之间建立父(左)子关系
inline void attachAsLChild(NodePosi p, NodePosi lc) {
  p->lc = lc;
  if (lc) lc->parent = p;
}

template <typename NodePosi>  // 在节点*p与*rc(可能为空)之间建立父(右)子关系
inline void attachAsRChild(NodePosi p, NodePosi rc) {
  p->rc = rc;
  if (rc) rc->parent = p;
}

template <typename T>  // Splay树伸展算法:从节点v出发逐层伸展
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T)
                                   v) {  // v为因最近访问而需伸展的节点位置
  if (!v) return NULL;
  //*v的父亲与祖父
  BinNodePosi(T) p;
  BinNodePosi(T) g;
  while ((p = v->parent) && (g = p->parent)) {  // 自下而上,反复对*v左双层伸展
    BinNodePosi(T) gg =
        g->parent;  // 每轮过后*v都以原曾祖父(great- grand parent)为父
    if (IsLChild(*v))
      if (IsLChild(*p)) {  // zig-zig
        attachAsLChild(g, p->rc);
        attachAsLChild(p, v->rc);
        attachAsRChild(p, g);
        attachAsRChild(v, p);
      } else {  // zig-zag
        attachAsLChild(p, v->rc);
        attachAsRChild(g, v->lc);
        attachAsLChild(v, g);
        attachAsRChild(v, p);
      }
    else if (IsRChild(*p)) {  // zag-zag
      attachAsRChild(g, p->lc);
      attachAsRChild(p, v->lc);
      attachAsLChild(p, g);
      attachAsLChild(v, p);
    } else {  // zag-zig
      attachAsRChild(p, v->lc);
      attachAsLChild(g, v->rc);
      attachAsRChild(v, g);
      attachAsLChild(v, p);
    }
    if (!gg)
      v->parent = NULL;  // 若*v原先的曾祖父*gg不存在,则*v现在应为树根
    else                 // 否则,*gg此后应该以*v作为左或右孩子
      (g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
    updateHeight(g);
    updateHeight(p);
    updateHeight(v);
  }                     // 双层伸展结束时,必有g==NULL,但p可能非空
  if (p = v->parent) {  // 若p果真非空,则额外再做一次单旋
    if (IsLChild(*v)) {
      attachAsLChild(p, v->rc);
      attachAsRChild(v, p);
    } else {
      attachAsRChild(p, v->lc);
      attachAsLChild(v, p);
    }
    updateHeight(p);
    updateHeight(v);
  }
  v->parent = NULL;
  return v;
}

template <typename T>
    BinNodePosi(T) & Splay<T>::search(const T& e) {  // 在伸展树中查找e
  BinNodePosi(T) p = searchIn(_root, e, _hot = NULL);
  _root = splay(p ? p : _hot);  // 将最后一个被访问的节点伸展至根
  return _root;
}  // 与其他BST不同,无论查找成功与否,_root都指向最后被访问的节点

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {  // 将关键码e插入伸展树中
  if (!_root) {
    _size++;
    return _root = new BinNode<T>(e);
  }  // 处理原树为空树的退化情况
  if (e == search(e)->data) return _root;  // 确认目标节点不存在
  // 创建新节点.一下调整<=7个指针以完成局部重构
  _size++;
  BinNodePosi(T) t = _root;
  if (_root->data < e) {  // 插入新根,以t和t->rc为左、右孩子
    t->parent = _root = new BinNode<T>(e, NULL, t, t->rc);  // 2 + 3个
    if (HasRChild(*t)) {
      t->rc->parent = _root;
      t->rc = NULL;
    }       //<= 2个
  } else {  // 插入新根,以t->lc和t为左、右孩子
    t->parent = _root = new BinNode<T>(e, NULL, t.lc, t);  // 2 + 3个
    if (HasLChild(*t)) {
      t->lc->parent = _root;
      t->lc = NULL;
    }  //<= 2个
  }
  updateHeightAbove(t);  // 更新t及其祖先(实际上只有_root一个)的高度
  return _root;
}  // 无论e是否存在于原树中,返回时总有_root->data == e

template <typename T>
bool Splay<T>::remove(const T& e) {  // 从伸展树中删除关键码e
  if (!_root || (e != search(e)->data))
    return false;            // 若树空或目标不存在,则无法删除
  BinNodePosi(T) w = _root;  // assert:经search()后节点e已被伸展至树根
  if (!HasLChild(*_root)) {  // 若无左子树,则直接删除
    _root = _root->rc;
    if (_root) _root->parent = NULL;
  } else if (!HasRChild(*_root)) {  // 若无右子树,也直接删除
    _root = root->lc;
    if (_root) _root->parent = NULL;
  } else {  // 若左右子树同时存在.则
    BinNodePosi(T) lTree = _root->lc;
    // 暂时将左子树切除
    lTree->parent = NULL;
    _root->lc = NULL;
    // 只保留右子树
    _root = _root->rc;
    _root->parent = NULL;
    search(w->data);  // 以原树根为目标,做一次(必定失败的)查找
    // assert:至此,右子树中最小节点必伸展至根,且(因无雷同节点)其左子树必空,于是
    // 只需将左子树接回原位即可
    _root->lc = lTree;
    lTree->parent = _root;
  }
  // 释放节点,更新规模
  release(w->data);
  release(w);
  _size--;
  if (_root) updateHeight(_root);  // 此后,若树非空,则树根的高度需要跟新
  return true;                     // 返回成功标志
}  // 若目标节点存在且被删除,返回true;否则返回false