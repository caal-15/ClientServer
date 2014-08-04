#include <iostream>
#include <unordered_map>
using namespace std;

class Point{
private:
	double x;
	double y;
public:
	Point(){
		x = 0.0;
		y = 0.0;
	}
	
	Point(double p, double q){
		x=p;
		y=q;
	}
	
	void setPoints(double p, double q){
		x=p;
		y=q;
	}
	
	void print(){
		cout << '<' << x << ',' << y << '>' << endl; 
	}
};

int main(){
	Point p, q(3.2, 4.54);
	unordered_map<int, Point> antonio;
	antonio[1] = p;
	antonio[2] = q; 
	cout << "Size: " << antonio.size() << endl;
	antonio[1].print();
	return 0;
}
