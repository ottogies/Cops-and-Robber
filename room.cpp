#include <iostream>
#include <list>
#include <string>
#include "room.hpp"
#include "map.hpp"

std::list <User> user_list;

std::list <Room> rooms;
std::set <unsigned int> room_ids;

User::User(){}
User::User(websocketpp::connection_hdl hdl){
	id = hdl;
}
std::shared_ptr<void> User::ID() const {
	return id.lock();
}
websocketpp::connection_hdl User::Hdl() const {
	return id;
}
std::string User::User_name(){
	return user_name;
}
bool User::operator==(const User& user){
	if(id.lock() == user.ID())
		return true;
	else
		return false;
}
void User::name(std::string user_name_){
	user_name = user_name_;
}
int User::local(websocketpp::connection_hdl hdl){
	if(id.lock() == hdl.lock())
		return 1;
	else
		return 0;
}

Room::Room(){}
Room::Room(std::string title_, int capacity_, User owner_){
	room_id = static_cast<unsigned int>(randomNum(1, 9999));
	title = title_;
	capacity = capacity_;
	owner = owner_;
}
unsigned int Room::ID(){ return room_id; }
std::string Room::Title(){ return title; }
int Room::Capacity(){ return capacity; }
//std::list <User> Room::Users(){ return users; }
std::map <int, User> Room::Users(){ return users; }
int Room::Index(User user){
	std::map <int, User>::iterator it;
	for(it = users.begin(); it != users.end(); it++){
		if(it->second == user){
			return it->first;
		}
	}
	return -1; 	//user not in the room 
}
void Room::enter(User user){
	//users.push_back(user);
	std::map <int, User>::iterator it = users.begin();
	int i = -1;
	do{
		i++;
		it = users.find(i);
	}while(it != users.end());
	users.insert( std::pair<int, User>(i, user) );
}
int Room::leave(User user){
//	std::list <User>::iterator it;
//	for(it = users.begin(); it != users.end(); it++){
//		//if((*it).lock() == user.lock()){
//		if(*it == user){
//			users.erase(it);
//			return 0;
//		}
//	}
//	return 1;
	std::map <int, User>::iterator it;
	for(it = users.begin(); it != users.end(); it++){
		if(it->second == user){
			users.erase(it);
			//if user가 방장이라면 
			return 0;
		}
	}
	return 1;
}
void Room::setOwner(User user){
	owner = user;
}
int Room::size(){
	return users.size();
}
int Room::Owner(User user){
	if(user == owner)
		return 1;
	else
		return 0;
}
void Room::printUsers(){
//	std::list <User>::iterator user;
//	for(user = users.begin(); user != users.end(); user++)
//		std::cout << (*user).ID() << " ";
//	std::cout << std::endl;
	std::map <int, User>::iterator user;
	for(user = users.begin(); user != users.end(); user++)
		std::cout << user->second.ID() << " ";
	std::cout << std::endl;
}

void access(websocketpp::connection_hdl hdl){
	User user(hdl);
	user_list.push_back(user);
}

void exit(websocketpp::connection_hdl hdl){
	std::list <User>::iterator user;
	for(user = user_list.begin(); user != user_list.end(); user++){
		if((*user).ID() == hdl.lock()){
			user_list.erase(user);
			return;
		}
	}
}

void users(){
	std::list <User>::iterator user;
	std::cout << user_list.size() << " ";
	for(user = user_list.begin(); user != user_list.end(); user++){
		std::cout << (*user).ID() << " ";
	}
	std::cout << std::endl;
}

void setName(websocketpp::connection_hdl hdl, std::string user_name){
	std::list <User>::iterator user;
	for(user = user_list.begin(); user != user_list.end(); user++){
		if((*user).ID() == hdl.lock()){
			(*user).name(user_name);
			return;
		}
	}
}

User getUser(websocketpp::connection_hdl hdl){
	std::list <User>::iterator user;
	for(user = user_list.begin(); user != user_list.end(); user++){
		if((*user).ID() == hdl.lock()){
			return *user;
		}
	}
}

Room getRoom(unsigned int room_id){
	std::list <Room>::iterator room;
	for(room = rooms.begin(); room != rooms.end(); room++){
		if((*room).ID() == room_id){
			return *room;
		}
	}
}

unsigned int createRoom(std::string title, int capacity, User owner){
	Room room(title, capacity, owner);
	rooms.push_back(room);
	room_ids.insert(room.ID());
	std::cout << room.ID() <<  room.Title() << room.Capacity() << std::endl;
	std::cout << rooms.size();
	return room.ID();
}

Room joinRoom(unsigned int room_id, User user){
	std::list <Room>::iterator room;
	for(room = rooms.begin(); room != rooms.end(); room++){
		if((*room).ID() == room_id){
			(*room).enter(user);
			return *room;
		}
	}
}

int checkRoom(unsigned int id){
	try{
		std::set <unsigned int>::iterator it;
		it = room_ids.find(id);
		if(it == room_ids.end())
			throw "No room number";
		std::list <Room>::iterator itr;
		for(itr = rooms.begin(); itr != rooms.end(); itr++){
			if((*itr).ID() == id){
				if((*itr).size() >= (*itr).Capacity())
					throw "Room is full";
				break;
			}
		}
		return 0;
	}catch(const char* err){
		if(err == "No room number")
			return 1;
		if(err = "Room is full")
			return 2;
	}
}

//Room leaveRoom(unsigned int room_id, User user){
//	std::list <Room>::iterator room;
//	for(room = rooms.begin(); room != rooms.end(); room++){
//		if((*room).ID() == room_id){
//			if(!(*room).leave(user))
//				return *room;
//			else{					//user not in the room
//				Room emptyRoom;
//				return emptyRoom;
//			}
//		}
//	}
//}

std::map <int, User> leaveRoom(unsigned int room_id, User user){
	std::map <int, User> users; 
	std::list <Room>::iterator room;
	for(room = rooms.begin(); room != rooms.end(); room++){
		if((*room).ID() == room_id){
			if(!(*room).Owner(user)){
				if(!(*room).leave(user)){
					users.insert( std::pair<int, User>(0, user) );
				}
				else{
					//user not in the room 
				}
			}else{
				users = (*room).Users();
				users.insert( std::pair<int, User>(0, user) );
				room_ids.erase(room_id);
				rooms.erase(room);
			}
			break;
		}
	}
	return users;
}

std::stringstream roomList(){
	std::stringstream room_list;
	room_list << "room_list," << rooms.size();
	std::list <Room>::iterator room;
	for(room = rooms.begin(); room != rooms.end(); room++)
		room_list << delim << (*room).ID() << delim << (*room).Title() << delim << (*room).Capacity() << delim << (*room).size();
	return room_list;
}

void check(){
	std::list <Room>::iterator room;
	for(room = rooms.begin(); room != rooms.end(); room++){
		std::cout << (*room).ID() << " ";
		(*room).printUsers();
	}
}
