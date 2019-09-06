#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstring>
#include <map>
//#include <iterator>

using namespace std;

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

int main(){
	map <int, User> users;
	User user1(1000, "sungmook");
	User user2(1001, "hakchoen");
	User user3(1002, "hansu");
	users.insert( pair <int, User> (0, user1));
	users.insert( pair <int, User> (1, user2));
	users.insert( pair <int, User> (2, user3));
	map <int, User>::iterator it;
	for(it = users.begin(); it != users.end(); it++){
		cout << "id: " << it->second.ID() << " name: " << it->second.Name() << endl;
		sayHi(it->second);
	}
}


