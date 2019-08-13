#include <iostream>
#include <list>
#include <string>
#include "room.hpp"
#include "map.hpp"

std::list <Room> rooms;
std::set <unsigned int> room_ids;

Room::Room(){}
Room::Room(std::string title_, int capacity_){
	room_id = static_cast<unsigned int>(randomNum(1, 9999));
	title = title_;
	capacity = capacity_;
}
unsigned int Room::ID(){ return room_id; }
std::string Room::Title(){ return title; }
int Room::Capacity(){ return capacity; }
void Room::enter(User user){
	users.push_back(user);
}
int Room::size(){
	return users.size();
}
void Room::printUsers(){
	std::list <User>::iterator it2;
		for(it2 = users.begin(); it2 != users.end(); it2++){
			std::cout << (*it2).lock() << " ";
		} 
}

unsigned int createRoom(std::string title, int capacity){
	Room room(title, capacity);
	rooms.push_back(room);
	room_ids.insert(room.ID());
	std::cout << room.ID() << room.Title() << room.Capacity() << std::endl;
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

void check(){
	std::list <Room>::iterator it2;
		for(it2 = rooms.begin(); it2 != rooms.end(); it2++){
			std::cout << (*it2).ID() << " ";
			(*it2).printUsers();
			std::cout << std::endl;
		} 
	
}
