#include <string>
#include "room.hpp"
#include "map.hpp"

Room::Room(std::string title_, int capacity_){
	room_id = static_cast<unsigned int>(randomNum(1, 9999));
	title = title_;
	capacity = capacity_;	
}
unsigned int Room::ID(){ return room_id; }
std::string Room::Title(){ return title; }
int Room::Capacity(){ return capacity; }
