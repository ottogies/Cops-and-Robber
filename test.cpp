#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
//#include <iterator>

using namespace std;

int main(){
	vector <int> vec = {1, 3, 10, 54, 11, 56, 39, 71, 22, 25, 24, 78, 36, 99, 28, 20, 24};
	
	vector <int>::iterator it;
	
	/*for(it=vec.begin(); it!=vec.end(); it++){
		if((*it) % 2 == 0){
			vec.erase(it);
			it--;
		}
	}*/
	
	it=vec.begin();
	while(it!=vec.end()){
		if((*it) % 2 == 0){
			vec.erase(it);
		} else {
			it++;
		}
	}
	
	for(int i=0; i<vec.size(); i++)
		cout << vec[i] << endl;
	
}

