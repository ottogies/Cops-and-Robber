#ifndef GAME_H
#define GAME_H

#include <map>
#include <vector>
#include "room.hpp"
#include "message.hpp"
#include "map.hpp"

typedef enum { Cop, Rob } Role;

class Player {
	unsigned int player_id;
	User user;
	Role role;
	int turn;
	vertex_id pos;
public:
	Player(User user_, Role role_){
		user = user_;
		role = role_;
	}
	User Usr();
	Role Rol();
};

class Game {
	unsigned int game_id;
	unsigned int room_id;
	int cop_num;
	int rob_num;
	Vertex* map;
	int map_size;
	//std::multimap <Role, User> users;
	std::vector <Player> players;
public:
	Game(unsigned int, int, int, int, int);
	unsigned int ID();
	unsigned int Room_id();
	int Cop_num();
	int Rob_num();
	Vertex* Map();
	int Map_size();
	//std::multimap <Role, User> Users();
	std::vector <Player> Players();
	void accept(User, Role);
	int size();
};

unsigned int startGame(unsigned int, int, int, int, int);
Game getGame(unsigned int);
Role stor(std::string);
std::string rtos(Role);
int selectRole(unsigned int, User, Role);
int allSelected(unsigned int);

#endif
