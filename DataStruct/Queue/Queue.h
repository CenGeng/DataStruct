#include "../List/List.h"  // 以List为基类
struct Customer {
  int window;
  unsigned int time;
};  // 顾客类:所属窗口(队列)、服务时长

template <typename T>
class Queue : public List<T> {  // 队列模板类(继承List原有接口)
 public:
  void enqueue(T const& e) { List<T>::insertAsLast(e); }  // 入队:尾部插入
  T dequeue() { return List<T>::remove(List<T>::first()); }        // 出队:首部删除
  T& front() { return List<T>::first()->data; }           // 队首
};

/// 队列应用
void simulate(int nWin, int servTime);
int bestWindow(Queue<Customer>*, int nWin);

void simulate(int nWin, int servTime) {  // 按指定窗口数、服务总时间模拟银行业务
  Queue<Customer>* windows =
      new Queue<Customer>[nWin];  // 为每一窗口创建一个队列
  for (int now = 0; now < servTime; now++) {  // 在下班之前,每间隔一个时间单位
    if (rand() % (1 + nWin)) {  // 新顾客以nWin/(nWin+1)的概率到达
      Customer c;
      c.time = 1 + rand() % 98;  // 新顾客到达,服务时长随机确定
      c.window = bestWindow(windows, nWin);  // 找出最佳(最短)的服务窗口
      windows[c.window].enqueue(c);          // 新顾客加入对应的队列
    }
    for (int i = 0; i < nWin; i++)  // 分别检查
      if (!windows[i].empty())      // 各非空队列
        if (--windows[i].front().time <= 0)  // 队首顾客的服务时长减少一个单位
          windows[i].dequeue();  // 服务完毕的顾客出列,由后继顾客接替
  }                              // for
  delete[] windows;  // 释放所有队列(此前,~List()会自动清空队列)
}
int bestWindow(Queue<Customer>* windows, int nWin) {
  int minSize = windows[0].size(), optiWin = 0;  // 最优队列(窗口)
  for (int i = 0; i < nWin; i++) {               // 在所有窗口中
    if (minSize > windows[i].size()) {           // 挑选出
      minSize = windows[i].size();
      optiWin = i;
    }  // 队列最短者
  }
  return optiWin;
}