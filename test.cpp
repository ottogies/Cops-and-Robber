#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstring>
//#include <iterator>

using namespace std;

stringstream roomList(stringstream);

int main(){

	stringstream ss;
	stringstream st;
	string s;
	st << "hi";
	ss = roomList(st);
	s = ss.str();
	cout << s;
	
}

stringstream roomList(stringstream ss){
	stringstream ss;
	int a = 5;
	ss << "abc" << a;
	return ss; 
}
