#include<iostream>
#include<thread>
#include<chrono>
#include<cmath>
using namespace std;

void fibo(int n, int *ans){
    if(n <= 2){
        *ans = 1;
        return;
    }
    else{
        int x, y;
        fibo(n-1, &x);
        fibo(n-2, &y);
        *ans = x + y;        
    }
}

void fiboCon(int n, int *ans, int tn=1){
    if( tn <= 0 || n <= 2){
        fibo(n, ans);
    }
    else{
        int x, y;
        thread th1(fiboCon, n-1, &x, tn - 1);
        thread th2(fiboCon, n-2, &y, tn - 1);
        th1.join();
        th2.join();
        *ans = x + y;
    }
}

int main(){
    int ans, n, tn;
    chrono::time_point<chrono::system_clock> start, end;
    chrono::duration<double> elapsed;
    
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
