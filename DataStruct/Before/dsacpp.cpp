#include <iostream>
using namespace std;

bool canConstruct(string ransomNote, string magazine);

class Sort
{
   public:
      void bubblesort1A( int A[], int n); // 起泡排序算法（版本1A）：0 <= n

};

void Sort::bubblesort1A( int A[], int n)
{
   bool sorted = false;  // 整体排序标志，首先假定尚未排序
   while ( !sorted ) // 在尚未确认已全局排序之前，逐趟进行扫描交换
   {
      sorted = true; // 假定已经排序
      for (int i = 1; i < n; i++)   // 自左向右逐对检查当前范围A[0,n)内的各相邻元素
      {
         if ( A[i - 1] > A[i])   // 一旦A[i - 1]与A[i]逆序，则
         {
            swap( A[i - 1], A[i] ); // 交换之，并
            sorted = false;   // 因整体排序不能保证，需要清楚排序标志
         }
      }
      n--;  // 至此末元素必然就位, 故可以缩短待排序序列的有效长度
      
   }
} // 借助布尔型标志位sorted, 可即使提前退出, 而不致总是蛮力地做n - 1趟扫描交换

/*int sum( int A[], int n)
{
   if (1 > n)
   {
      return 0;
   }else
   {
      return sum( A, n - 1) + A[ n - 1 ];
   }

}*/
int sum( int A[], int n)
{
    while ( 0 < n )
    {
        return sum( A, n - 1) + A[ n - 1];
    }
    return 0;
}
int sum( int A[], int lo, int hi)   // 数组求和,二分递归版
{
   if (lo == hi)  // 如遇递归基(区间长度以降为1),则
      return A[lo];  // 直接返回该元素
   else  //否则(一般情况下lo < hi),则
   {
      int mi = ( lo + hi ) >> 1; // 以居中单元为界,将区间一分为二
      return sum( A, lo, mi ) + sum( A, mi + 1, hi);
   }
   
}
void reverse( int*, int, int);   // 重载的倒置算法原型
void reverse( int* A, int n)  // 数组倒置(算法的初始入口,调用的可能是reverse()的递归版或迭代版)
{
   reverse( A, 0, n - 1);
}
/* void reverse( int* A, int lo, int hi)  // 数组倒置(多递归基递归版)
 {
    if ( lo < hi)
    {
       swap( A[lo], A[hi]); // 交换A[lo]和A[hi]
       reverse ( A, lo + 1, hi - 1); // 递归倒置A(lo, hi)
    }  // else隐含了两种递归基

 }*/
/*void reverse( int* A, int lo, int hi)  // 数组倒置(直接改造而得的迭代版)
{
   next: // 算法起始位置添加跳转标志
      if( lo < hi)
      {
         swap( A[lo], A[hi]); // 交换A[lo]和A[hi]
         lo++; hi--; // 收缩待倒置区间
         goto next;  //跳转至算法体起始位置,迭代地倒置A(lo,hi)
      }  // else隐含了迭代的终止
}*/
void reverse ( int* A, int lo, int hi)  // 数组倒置(规范整理之后的迭代版)
{
    while( lo < hi )    //用while替换跳转标志和if,完全等效
        swap( A[lo++], A[hi--]);    // 交换A[lo]和A[hi],收缩待倒置区间
}

inline __int64_t sqr(__int64_t a){ return a * a;}
__int64_t power2 ( int n ) //幂函数2^n算法(优化递归版), n >= 0
{
   if (0 == n) return 1;   //递归基;否则,视n的奇偶性分别递归
   return ( n & 1 ) ? sqr( power2( n >> 1 )) << 1 : sqr( power2( n >> 1));
   
}

int64_t fib(int n) // 计算Fibonacci数列的第n项(二分递归版) : O(2^n)
{
   return ( 2 > n ) ?
          ( int64_t ) n // 若到达递归基,直接取值
          : fib ( n - 1 ) + fib ( n - 2 );   // 否则, 递归计算前两项,其和是正解(数据量达到一定时效率及其低下)
}
int64_t fib(int n, int64_t& prev)    // 计算Fibonacci数列的第n项(线性递归版):入口形式fib(n,prev)
{
    if ( 0 == n )   // 若到达递归基,则
    {               // 直接取值:fib(-1) = 1,fib(0) = 0
        prev = 1;
        return 0;
    }else   // 否则
    {
        int64_t prevPrev ;prev = fib(n - 1, prevPrev);   // 递归计算前两项
        return  prev + prevPrev;    // 其和为正解
    }   // 用辅助变量记录前一项,返回数列的当前项, O(n)
} 
int64_t fibI(int n)  // 计算Fibonacci数列的第n项(迭代版) : O(n)
{
   int64_t f = 1, g = 0; // 初始化:fib(-1)、 fib(0)
   while ( 0 < n-- ) // 根据原始定义,通过n次加法和减法计算fib(n)
   {
      g += f;
      f = g - f;
   }
   return g;
   
}
template <typename T> void show(T text){
    cout << "called show(T text):" << text << endl;
}
void show(int num){
    cout << "called show(int num):" << num << endl;

}

int main()
{
   int  arr[] = {2, 5, 6, 2, 1, 0, 11, 24, 9};
   // Sort sort1;
   // sort1.bubblesort1A(arr, 9);

   //  for (int i : arr) {
   //      cout << i << endl;
   //  }
//   reverse(arr, 9);
//   for (int i : arr)
//   {
//      cout << i << endl;
//   }
   // cout << sum(arr, 0, 8) << endl;
   // cout << arr[9];

/*
   int64_t  pre;
   double start, finish;
   start = (double)clock();
//   cout << fib(50,pre) << endl;

  cout << fibI(50) << endl;
   finish = (double)clock();
   cout << "Runtime: " << finish - start << "ms" << endl;
   return (int)((finish - start) / 60 );
*/

//    show("The text");
//    show(1);
//

//cout << canConstruct("ajjjb", "aaajb") << endl;
//

int i = 42, *p = &i;
  decltype(p) c = &i;
cout << *c << endl;
}


