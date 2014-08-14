#include<iostream>
#include<thread>
#include<chrono>
using namespace std;

int fibo(int n, int *v){
    if(n <= 2 && n > 0){
	    return *v = 1;
	    cout << v << endl;
    }
    else{
        return *v = fibo(n-1, v) + fibo(n-2, v);
    }

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
    int n, ans;
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
    
    start = chrono::system_clock::now(); 
    ans = fiboCon(n);
    end = chrono::system_clock::now();
    elapsed = end - start;
    cout << "El Fibonnacci de " << n << " es " << ans << " (Calculo Concurrente)" << endl;
    cout << "El tiempo de ejecucion fue de " << elapsed.count() << " segundos" << endl;
    
    return 0;
}
