using namespace std;
#include<bits/stdc++.h>


void fib(int n, int *ans) {
  if (n < 2) {
    *ans = n;
    return;
  }
  int a,b;
  fib(n - 1, &a);
  fib(n - 2, &b);
  *ans = a + b;
}

int main() {
  int n;
  chrono::time_point<chrono::system_clock> start, end;
  chrono::duration<double> elapsed; 
  cout << "Ingrese un número: ";
  cin >> n;
  
  
  int a, b;
  start = chrono::system_clock::now();
  thread t1(fib, n - 1, &a);
  thread t2(fib, n - 2, &b);
  t1.join();
  t2.join();
  end = chrono::system_clock::now();
  elapsed = end - start;
  cout<< a + b <<endl;
  cout<< "El tiempo de cálculo fue: " << elapsed.count() << " segundos" << endl;
  
  return 0;
}
