#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstring>
#include <map>
//#include <iterator>

using namespace std;

typedef enum { Cop, Rob } Role;

class User{
	int id;
	string name;
public:
	User(int id_, string name_){
		id = id_;
		name = name_;
	}
	int ID(){
		return id;
	}
	string Name(){
		return name;
	}
};

void sayHi(User user){
	cout << "hi" << user.Name() << endl;
}

Role stor(std::string role){
	try{
		if(role == "cop")
			return Cop;
		else if(role == "rob")
			return Rob;
		else
			throw "Wrong string format";
	}catch(const char* err){
		std::cout << err << std::endl;
	}
}

int main(){
	unsigned int game_id;
	game_id = 4563;
	stringstream ss;
	ss << "make_map," << game_id;
	string str = ss.str();
	cout << str << endl;
	
}


