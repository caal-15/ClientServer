#include<iostream>
#include<thread>
using namespace std;

void fibo(int n, int *v){
    *v = n*n;
}

int fiboCon(int n){
    int x, y;
    thread th1(fibo, n-1, &x);
    thread th2(fibo, n-2, &y);
    th1.join();
    th2.join();
    return x + y;
    }

int main(){
    int ans;
    ans = fiboCon(10);
    cout << ans << endl;
    return 0;
}
