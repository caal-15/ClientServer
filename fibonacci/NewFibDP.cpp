#include<iostream>
#include<thread>
#include<chrono>
#include<cmath>
#include<cstring>
#include<mutex>
using namespace std;

const int MN = 100000;
long long dp[MN];

mutex cool_mutex;

void fibo(int n, long long *ans){
    if( n < MN){
        if (dp[n] != -1){
            *ans = dp[n];
            return;
        }
    }
    if(n <= 2){
        *ans = 1;
        return;
    }
    else{
        long long x, y;
        fibo(n-1, &x);
        fibo(n-2, &y);
        if (n < MN){
            cool_mutex.lock();
            dp[n] = *ans = x + y;
            cool_mutex.unlock();
        }        
    }
}

void fiboCon(int n, long long *ans, int tn=1){
    if(n < MN){
        if(dp[n] != -1){
            *ans = dp[n];
            return; 
        }
    }    
    if( tn <= 0 || n <= 2){
        fibo(n, ans);
    }
    else{
        long long x, y;
        thread th1(fiboCon, n-1, &x, tn - 1);
        thread th2(fiboCon, n-2, &y, tn - 1);
        th1.join();
        th2.join();
        if(n < MN){
            cool_mutex.lock();
            dp[n] = *ans = x + y;
            cool_mutex.unlock();
        }
    }
}

int main(){
    int n, tn;
    long long ans;
    chrono::time_point<chrono::system_clock> start, end;
    chrono::duration<double> elapsed;
    
    memset(dp, -1, sizeof dp);
    
    
    cout << "Ingrese el número para calcular su valor en la sucesión de Fibonacci: ";
    cin >> n;
    start = chrono::system_clock::now();
    fibo(n, &ans);
    end = chrono::system_clock::now();
    elapsed = end - start;
    cout << "El Fibonnacci de " << n << " es " << ans << " (Calculo secuencial)" << endl;
    cout << "El tiempo de ejecucion fue de " << elapsed.count() << " segundos" << endl << endl;
    
    cout << "Ingrese el número de hilos (n, se lanzarán 2^n hilos): ";
    cin >> tn;
    start = chrono::system_clock::now();
    fiboCon(n, &ans, tn);
    end = chrono::system_clock::now();
    elapsed = end - start;
    cout << "El Fibonnacci de " << n << " es " << ans << " (Calculo Concurrente con " << pow(2, tn) << " hilos)" << endl;
    cout << "El tiempo de ejecucion fue de " << elapsed.count() << " segundos" << endl << endl;
    return 0;
}
