#include <iostream>
using namespace std;
/*
int match(char* P, char* T) {  // 串匹配算法(Brute-force-1)
  size_t n = strlen(T), i = 0;  // 文本串长度、当前接受比对字符的位置
  size_t m = strlen(P), j = 0;  // 模式串长度、当前接受比对字符的位置
  while (j < m && i < n)        // 自左向右逐个比对字符
    if (T[i] == P[j]) {  // 若匹配,则转到下一对字符
      i++;
      j++;
    } else {  // 否则,文本串回退,模式串复位
      i -= j - 1;
      j = 0;
    }
  return i - j;
}
*/

int math(char* P, char* T) {  // 串匹配算法(Brute-force-2)
  size_t n = strlen(T), i = 0;  // 文本串长度、当前接受比对字符的位置
  size_t m = strlen(P), j;  // 模式串长度、当前接受比对字符的位置
  for (i = 0; i < n - m + 1; i++) {  // 文本串从第i个字符起,与
    for (j = 0; j < m; j++)          // 模式串中对应的字符逐个比对
      if (T[i + j] != P[j]) break;  // 若失败,模式串整体右移一个字符,再做一轮比对
    if (j >= m) break;  // 找到匹配字符串
  }
  return i;
}