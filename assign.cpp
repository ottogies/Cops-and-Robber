#include "assign.hpp"

//enum Role { Cop, Rob };

//Room::Room(){
//	cop_num = 0;
//	rob_num = 0;
//}
int Room::Cop_size(){ return cop_size; }
int Room::Rob_size(){ return rob_size; }
int Room::Cop_num(){ return cop_num; }
int Room::Rob_num(){ return rob_num; }
void Room::add_Cop_num(){ cop_num++; }
void Room::add_Rob_num(){ rob_num++; }
void Room::setSize(){
	user_size = 2;
	cop_size = 1;
	rob_size = 1;
}

Player::Player(User user_, Role role_){
	user = user_;
	role = role_;
}

std::string assignRole(std::string msg, Room room){
	std::string str;
	Role role;
	
	/*
	role = role_select(msg);
	
	if(!sizeOver(role)){
		assign(user, role);
		sprintf(str, "You are %s", changeToString(role));
	}else {
		sprintf(str, "%s is Full", changeToString(role));
	}
	*/
	
	return str;
}

Role role_select(std::string msg){
	if(msg == "Cop")
		return Cop;
	else{
		return Rob;
	}
}

bool sizeOver(Role role){
	/*
	static int cop_size;
	static int rob_size;
	static int cop_num;
	static int rob_num;
	
	Room room();
	room.setSize();
	*/
	/*
	if(role == Cop){
		if(room.Cop_num == room.Cop_size())
			return true;
		else{
			room.add_Cop_num();
			return false;
		}
	}else {		//role == Rob
		if(room.Rob_num == room.Rob_size())
			return true;
		else{
			room.add_Rob_num();
			return false;
		}
	}
	*/
}

void assign(User user, Role role){
	Player player(user, role);
}

std::string changeToString(Role role){
	if(role == Cop)
		return "Cop";
	else
		return "Robber";
}
