#include "../Bitmap/Bitmap.h"          //引入位图
#include "../Dictionary/Dictionary.h"  //引入词典ADT
#include "../Entry/Entry.h"
#include "../General/release.h"
using namespace std;

template <typename K, typename V>  // key、value
class Hashtable
    : public Dictionary<K, V> {  // 符合Dictionary接口的Hashtable模板类
 private:
  Entry<K, V>** ht;     // 桶数组,存放词条指针
  int M;                // 桶数组容量
  int N;                // 词条数量
  Bitmap* lazyRemoval;  // 懒惰删除标记
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
 protected:
  int probe4Hit(const K& k);  // 沿关键码k对应的查找链,找到词条匹配的桶
  int probe4Free(const K& k);  // 沿关键码k对应的查找链,找到首个可用的空桶
  void rehash();  // 重散列算法:扩充桶数组,保证装填因子在警戒线以下
 public:
  Hashtable(int c = 5);  // 创建一个容量不小于c的散列表
  ~Hashtable();  // 释放桶数组及其中各(非空)元素所指向的词条
  int size() const { return N; }
  bool put(K, V);    // 插入(禁止雷同词条,故可能失败)
  V* get(K k);       // 读取
  bool remove(K k);  // 删除
};
int primeNLT(int c, int n, char* file);
template <typename K, typename V>
Hashtable<K, V>::Hashtable(int c) {  // 创建散列表,容量为
  M = primeNLT(c, 1048576,
               "../../_input/prime-1048576-bitmap.txt");  // 不小于c的素数M
  // 开辟桶数组(还需核对申请成功),初始装填因子为N/M = 0%
  N = 0;
  ht = new Entry<K, V>*[M];
  memset(ht, 0, sizeof(Entry<K, V>*) * M);  // 初始化各桶
  lazyRemoval = new Bitmap(M);              // 懒惰删除标记比特图
}
int primeNLT(int c, int n,
             char* file) {  // 根据file文件中的记录,在[c,n]内取最小的素数
  Bitmap B(file, n);  // file已经按位图格式,记录了n以内所有的素数,因此只要
  while (c < n)  // 从c开始,逐位地
    if (B.test(c))
      c++;  // 测试,即可
    else
      return c;  // 返回首个发现的素数
  return c;  // 若没有这样的素数,返回n(实用中不能如此简化处理)
}
template <typename K, typename V>
Hashtable<K, V>::~Hashtable() {  // 析构前释放痛数组及非空词条
  for (int i = 0; i < M; i++)    // 逐一检查各桶
    if (ht[i]) release(ht[i]);   // 释放非空的桶
  release(ht);                   // 释放桶数组
  release(lazyRemoval);          // 释放懒惰删除标记
}

template <typename K, typename V>
V* Hashtable<K, V>::get(K k) {  // 散列表词条查找算法
  int r = probe4Hit(k);
  return ht[r] ? &(ht[r]->value) : NULL;
}

static int hashCode(char c) { return (int)c; }  // 字符
static int hashCode(int k) { return (int)k; }   // 整数以及长长整数
static int hashCode(long long i) { return (int)((i >> 32) + (int)i); }
static int hashCode(
    char s[]) {  // 生成字符串的循环移位散列码（cyclic shift hash code）
  int n = strlen(s);
  int h = 0;                     // 散列码
  for (int i = 0; i < n; i++) {  // 自左向右，逐个处理每一字符
    h = (h << 5) |
        (h >> 27);  // 左移5位即乘以32；右移27位作为扰动（通常与最高位有关）
    h += s[i];  // 累计上当前字符的贡献
  }
  return h;  // 如此所得的散列码，实际上可理解为近似的“多项式散列码”
}  // 32接近于26，故此方法针对于英文字符串；对于中文等其他字符串，需做相应的调整

/*
 * 沿关键码k对应的查找链,找到与之匹配的桶(供查找和删除词条时调用)
 * 试探策略多种多样,可灵活选取;这里仅以线性试探策略为例
 */
template <typename K, typename V>
int Hashtable<K, V>::probe4Hit(const K& k) {
  int r = hashCode(k) % M;  // 从起始桶(按除余法确定)出发
  while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r)))
    r = (r + 1) %
        M;  // 沿查找链线性试探:跳过所有冲突的桶,以及带懒惰删除标记的桶
  return r;  // 调用者根据ht[r]是否为空,即可判断查找是否成功
}

template <typename K, typename V>
bool Hashtable<K, V>::remove(K k) {  // 散列表词条删除算法
  int r = probe4Hit(k);
  if (!ht[r]) return false;  // 对应词条不存在时,无法删除
  // 否则释放桶中词条,设置懒惰删除标记,并更新词条总数
  release(ht[r]);
  ht[r] = NULL;
  markAsRemoved(r);
  N--;
  return true;
}

template <typename K, typename V>
bool Hashtable<K, V>::put(K k, V v) {  // 散列表词条插入
  if (ht[probe4Hit(k)]) return false;  // 雷同元素不必重复插入
  int r = probe4Free(k);  // 为新词条找个空桶(只要装填因子控制得当,必然成功)
  // 插入(注意:懒惰删除标记无需复位)
  ht[r] = new Entry<K, V>(k, v);
  ++N;
  if (N * 2 > M) rehash();  // 装填因子高与50%后重散列
  return true;
}

/*
 * 沿关键码k对应的查找链,找到首个可用的空桶(仅供插入词条时调用)
 * 试探策略多种多样,可灵活选取;这里仅以线性试探策略为例
 */
template <typename K, typename V>
int Hashtable<K, V>::probe4Free(const K& k) {
  int r = hashCode(k) % M;  // 从起始桶(按除余数法确定)出发
  while (ht[r])
    r = (r + 1) % M;  // 沿查找链逐桶试探,直到首个空桶(无论是否带有懒惰删除标记)
  return r;  // 为保证空桶总能找到,装填因子及散列表长需要合理设置
}

/*
 * 重散列算法:装填因子过大时,采取“逐一取出再插入”的朴素策略,对桶数组扩容
 * 不可简单地(通过memcpy())将原桶数组复制到新桶数组(比如前端),否则存在两个问题:
 * 1)会继承原有冲突;2)可能导致查找链在后端断裂——即便为所有扩充桶设置懒惰删除标志也无济于事
 */
template <typename K, typename V>
void Hashtable<K, V>::rehash() {
  int old_capacity = M;
  Entry<K, V>** old_ht = ht;
  M = primeNLT(2 * M, 1048576,
               "../../_input/prime-1048576-bitmap.txt");  // 容量至少加倍
  N = 0;
  ht = new Entry<K, V>*[M];
  memset(ht, 0, sizeof(Entry<K, V>*) * M);  // 新桶数组
  release(lazyRemoval);
  lazyRemoval = new Bitmap(M);            // 新开懒惰删除标记比特图
  for (int i = 0; i < old_capacity; i++)  // 扫描原桶数组
    if (old_ht[i])                        // 将非空桶中的词条逐一
      put(old_ht[i]->key, old_ht[i]->value);  // 插入至新的桶数组
  release(
      old_ht);  // 释放原桶数组——由于其中原先存放的词条均以转移,故只需释放桶数组本身
}