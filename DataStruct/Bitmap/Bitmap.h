#include <iostream>
using namespace std;
class Bitmap {
 private:
  unsigned char* M;  // 存放比特图的空间
  int N;             // 规模（以比特为单位）
 protected:
  void init(int n) {  // 比特图空间开辟
    M = new unsigned char[N = (n + 7) /
                              8];  // 按字节为单位分配比特图空间，并初始化为全0
    memset(M, 0, N);
  }

 public:
  Bitmap(int n = 8) { init(n); }  // 创建规模为n的比特图（默认为8）
  Bitmap(char* file, int n = 8) {  // 从指定文件中读取规模为n的比特图
    init(n);                       // 开辟空间并初始化为全0
    FILE* fp = fopen(file, "r");
    fread(M, sizeof(char), N, fp);
    fclose(fp);
    for (int i = 0; i < n; i++)  // 计数：统计其中为1的实际比特数目
      if (test(i)) {
        _size++;
      }
  }
  ~Bitmap() {
    delete[] M;
    M = NULL;
  }                  // 释放比特图空间
  void set(int k) {  // 将第k位转为1
    expand(k);
    M[k / 8] |= (0x80 >> (k % 8));
    _size++;
  }
  void clear(int k) {  // 将第k位转为0
    expand(k);
    M[k / 8] &= ~(0x80 >> (k % 8));
    _size--;
  }
  bool test(int k) {  // 测试第k位是否为1
    expand(k);
    return M[k / 8] & (0x80 >> (k % 8));
  }
  void dump(char* file) {  // 将当前位图导出到指定文件
    FILE* fp = fopen(file, "w");
    fwrite(M, sizeof(char), N, fp);
    fclose(fp);
  }
  char* bits2string(int n) {  // 将前n位转成字符串
    char* s = new char[n + 1];
    s[n] = '\0';                   // 为n+1个字符动态分配空间
    for (int i = 0; i < n; i++) {  // 逐位搬运
      s[i] = test(i) ? '1' : '0';
    }
    return s;  // 返回字符串基地址（由上层调用者释放）
  }
  int size() { return _size; }  // 返回实际被标记出来的比特数目
 private:
  int _size;  // 实际被标记出来的比特数目
  void expand(
      int k) {  // 若被访问的比特已经出界则扩容，并且按需要扩容（倍增策略）地将整个规模提升到2的整数倍
    if (k < 8 * N) {
      return;
    }  // 尚未越界
    int oldN = N;
    unsigned char* oldM = M;
    init(2 * k);  // 以2倍于所需规模，重新创建内存空间
    memcpy(M, oldM,
           oldN * sizeof(char));  // 整体复制原空间内容（memcpy指针的复制）
    delete[] oldM;                // 释放原空间
  }
};
