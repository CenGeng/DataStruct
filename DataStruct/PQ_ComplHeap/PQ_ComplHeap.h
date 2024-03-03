#define InHeap(n, i) (((-1) < (i)) && ((i) < (n)))  // 判断PQ[i]是否合法
#define Parent(i) ((i - 1) >> 1)  // PQ[i]的父节点(floor((i-1)/2),i无论正负)
#define LastInternal(n) Parent(n - 1)  // 最后一个内部节点(即末节点的父亲)
#define LChild(i) (1 + ((i) << 1))              // PQ[i]的左孩子
#define RChild(i) ((1 + (i)) << 1)              // PQ[i]的右孩子
#define ParentValid(i) (0 < i)                  // 判断PQ[i]是否有父亲
#define LChildValid(n, i) InHeap(n, LChild(i))  // 判断PQ[i]是否有一个(左)孩子
#define RChildValid(n, i) InHeap(n, RChild(i))  // 判断PQ[i]是否有两个孩子
#define Bigger(PQ, i, j) (lt(PQ[i], PQ[j]) ? j : i)  // 取大者(等时前者优先)
#define ProperParent(PQ, n, i) /*父子(至多)三者中的最大者*/ \
  (RChildValid(n, i)                                                   \
       ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i))               \
       : (LChildValid(n, i) ? Bigger(PQ, i, LChild(i))                 \
                            : i))  // 相等时父节点优先,如此可避免不必要的交换
#include "../PQ/PQ.h"          //按照优先级队列ADT实现的
#include "../Vector/Vector.h"  //借助多重继承机制,基于向量
using namespace std;
template <typename T>
class PQ_ComplHeap : public PQ<T>, public Vector<T> {  // 完全二叉堆
 protected:
  Rank percolateDown(Rank n, Rank i);  // 下滤
  Rank percolateUp(Rank i);            // 上滤
  void heapify(Rank n);                // Floyd建堆算法
  using Vector<T>::copyFrom;
  using Vector<T>::_size;
  using Vector<T>::_elem;
  using Vector<T>::empty;

 public:
  PQ_ComplHeap() {}             // 默认构造
  PQ_ComplHeap(T* A, Rank n) {  // 批量构造
    copyFrom(A, 0, n);
    heapify(n);
  }
  void insert(T);  // 按照比较器确定的优先级次序,插入词条
  T getMax();      // 读取优先级最高的词条
  T delMax();      // 删除优先级最高的词条
};

template <typename T>
T PQ_ComplHeap<T>::getMax() {  // 取优先级最高的词条
  return _elem[0];
}

template <typename T>
void PQ_ComplHeap<T>::insert(T e) {  // 将词条插入完全二叉堆中
  Vector<T>::insert(e);              // 首先将新词条接至向量末尾
  percolateUp(_size - 1);            // 再对该词条实施上滤调整
}

// 对向量中的第i个词条实施上滤操作,i < _size
template <typename T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i) {
  while (ParentValid(i)) {  // 只要i有父亲(尚未抵达堆顶),则
    Rank j = Parent(i);     // 将i之父记作j
    if (lt(_elem[i], _elem[j])) break;  // 一旦当前父子不再逆序,上滤旋即完成
    // 否则,父子交换位置,并继续考察上一层
    swap(_elem[i], _elem[j]);
    i = j;
  }
  return i;  // 返回上滤最终抵达的位置
}

template <typename T>
T PQ_ComplHeap<T>::delMax() {  // s删除非空完全二叉堆中优先级最高的词条
  T maxElem = _elem[0];
  _elem[0] = _elem[--_size];  // 摘除队顶(首词条),代之以末词条
  percolateDown(_size, 0);    // 对新堆顶实施下滤
  return maxElem;             // 返回此前备份的最大词条
}

// 对向量前n个词条中的第i个实施下滤,i < n
template <typename T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
  Rank j;  // i及其(至多两个)孩子在,堪为父者
  while (i != (j = ProperParent(_elem, n, i))) {  // 只要i非j,则
    // 二者换位,并继续考察下降后的i
    swap(_elem[i], _elem[j]);
    i = j;
  }
  return i;  // 返回下滤抵达的位置(亦i亦j)
}

template <typename T>
void PQ_ComplHeap<T>::heapify(Rank n) {  // Floyd建堆算法,O(n)时间
  for (int i = LastInternal(n); InHeap(n, i); i--)  // 自底而上,依次
    percolateDown(n, i);                            // 下滤各内部节点
}

template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {  // 0<= lo < hi <= size
  PQ_ComplHeap<T> H(_elem + lo, hi - lo);  // 将待排序区间建成一个完全二叉堆,O(n)
  while (!H.empty())  // 反复地摘除最大元并归入已排序的后缀,直至堆空
    _elem[--hi] = H.delMax();  // 等效于堆顶与末元素对换后下滤
}