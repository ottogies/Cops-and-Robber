#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstring>
#include <map>
//#include <iterator>

using namespace std;
typedef unsigned int vertex_id;
typedef enum { Cop, Rob } Role;

int randomNum(int min, int max){		/*min~max 사이의 랜덤값 반환*/ 
	static bool first_call = true;
	if(first_call){						/*srand() 한번만 실행해주어야 한다*/ 
		srand(static_cast<unsigned int>(time(NULL)));
		first_call = false;
	}
	return rand() % (max-min+1) + min;
}

class Player {
	unsigned int player_id;
	//User user;
	Role role;
	int turn;
	vertex_id pos;
public:
	Player(Role role_){
		player_id = static_cast<unsigned int>(randomNum(1, 9999));
		//user = user_;
		role = role_;
	}
	//User Usr();
	Role Rol();
	vertex_id Pos();
	void setPos(unsigned int);
};

//User Player::Usr() { return user; }
Role Player::Rol(){ return role; }
vertex_id Player::Pos(){ return pos; }
void Player::setPos(unsigned int pos_){
	pos = pos_;	
}

movePlayer(int *p){
	*p = 2348;
}


int main(){
	int a;
	movePlayer(&a);
	cout << a;
}



