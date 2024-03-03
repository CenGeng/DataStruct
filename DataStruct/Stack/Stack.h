#include "../Vector/Vector.h"  // 以向量为基类, 派生出栈模板类
template <typename T>
class Stack : public Vector<T> {  // 将向量的首/末端作为栈底/顶
 public:  // size()、empty()以及其他开放接口,均可直接沿用
  void push(T const& e) {  // 入栈:等效于将新元素作为向量的末元素插入
    Vector<T>::insert(this->size(), e);
  }
  T pop() {  // 出栈:等效于删除向量的末元素
    return Vector<T>::remove(this->size() - 1);
  }
  T& top() {  // 取顶:直接返回向量的末元素
    return (*this)[this->size() - 1];
  }
};
/*
void convert(Stack<char>& S, int64_t n,
             int base) {  // 十进制正整数n到base进制的转换(递归版)
  static char digit[]     // 0 < b, 1 < base < 16
                       // ,新进制下的数位符号,可视base取值范围适当扩充
      = {'0', '1', '2', '3', '4', '5', '6', '7',
         '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  if (0 < n) {                   // 在尚有余数之前,反复地
    S.push(digit[n % base]);     // 逆向记录当前最低位,再
    convert(S, n / base, base);  // 通过递归得到所有更高位
  }
}  // 新进制下由高到低的各数位,自顶而下保存与栈P中
*/
void convert(Stack<char>& S, int64_t n,
             int base) {  // 十进制正整数n到base进制的转换(迭代版)
  static char digit[]     // 0 < b, 1 < base < 16
                       // ,新进制下的数位符号,可视base取值范围适当扩充
      = {'0', '1', '2', '3', '4', '5', '6', '7',
         '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  while (n > 0) {  // 由低到高,逐一计算出新进制下的各数位
    int remainder = (int)(n % base);
    S.push(digit[remainder]);  // 余数(当前位)入栈
    n /= base;                 // n更新为其对base的除商
  }
}  // 新进制下由高到低的各数位,自顶而下保存与栈S中

/*  括号匹配,递归算法
void trim(const char exp[], int& lo,
          int& hi) {  // 删除exp[lo, hi]不含括号的最长前缀、后缀
  while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')'))
    lo++;  // 查找第一个和
  while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')'))
    hi--;  // 最后一个括号
}
int divide(const char exp[], int lo,
           int hi) {  // 切分exp[lo, hi], 使exp匹配仅当子表达式匹配
  int mi = lo;
  int crc = 1;  // crc为[lo, mi]范围内左、右括号数目之差
  while ((0 < crc) &&
         (++mi < hi)) {  // 逐个检查各字符, 直到左、右括号数目相等,或者越界
    if (exp[mi] == ')') crc--;
    if (exp[mi] == '(') crc++;
  }  // 左、右括号分别计数
  return mi;
}
bool paren(const char exp[], int lo,
           int hi) {  // 检查表达式exp[lo,hi]是否括号匹配(递归版)
  trim(exp, lo, hi);
  if (lo > hi) return true;          // 清除不含括号的前缀、后缀
  if (exp[lo] != '(') return false;  // 首自符非左括号,则必不匹配
  if (exp[hi] != ')') return false;  // 首自符非右括号,则必不匹配
  int mi = divide(exp, lo, hi);      // 确定适当的切分点
  if (mi > hi) return false;  // 切分点不合法,意味着局部以至整体不匹配
  return paren(exp, lo + 1, mi - 1) &&  // 分别检查左
         paren(exp, mi + 1, hi);        // 右子表达式
}
*/

bool paren(const char exp[], int lo,
           int hi) {  // 表达式括号匹配检查,可兼顾三种括号
  Stack<char> S;      // 使用栈记录已发现单尚未匹配的左括号
  for (int i = lo; i <= hi; i++)  // 逐一检查当前字符
    switch (exp[i]) {  // 左括号直接进栈; 右括号若与栈顶失配, 则表达式必不匹配
      case '(':
      case '{':
      case '[':
        S.push(exp[i]);
        break;
      case ')':
        if ((S.empty()) || ('(' != S.pop())) return false;
        break;
      case ']':
        if ((S.empty()) || (']' != S.pop())) return false;
        break;
      case '}':
        if ((S.empty()) || ('}' != S.pop())) return false;
        break;
      default:
        break;  // 非括号字符一律忽略
    }
  return S.empty();  // 整个表达式扫描过后, 栈中若仍残留(左)括号,则不匹配;
                     // 否则(栈空)匹配
}