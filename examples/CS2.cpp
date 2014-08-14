#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int main(){
	unordered_map<string, int> papa;
	papa["Prog"] = 22;
	papa.insert(make_pair("Rock", 33));
	cout << papa["Prog"] << endl;
	auto f = papa.find("Rock");
	if( f == papa.end()){
		cout << "tostados" << endl;		
	}
	else{
		cout << f->second << endl;
		cout << f->first << endl;		
	}
	for(const auto& entry : papa){
		cout << entry.first << " lala " << entry.second << endl;
	}
	return 0;
	
}
