#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstring>
//#include <iterator>

using namespace std;

int main(){

	try{
		throw "hi";
	}catch(const char * err){
		if(err == "he"){
		//if(!strcmp(err, "he")){
			cout << "yes";
		}else {
			cout << "no";
		}
	}
	
}

