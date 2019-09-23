#ifndef GAME_H
#define GAME_H

#include <map>
#include <vector>
#include "room.hpp"
#include "message.hpp"
#include "map.hpp"

typedef enum { Cop, Rob } Role;
typedef enum { Free, Arrested } State;

class Player {
	unsigned int player_id;
	User user;
	Role role;
	vertex_id pos;
	State state;
	int turn;
public:
	Player(User user_, Role role_){
		player_id = static_cast<unsigned int>(randomNum(1, 9999));
		user = user_;
		role = role_;
		state = Free;
		turn = 1;
	}
	unsigned int ID();
	User Usr();
	websocketpp::connection_hdl Hdl() const;
	Role Rol();
	vertex_id Pos();
	State Stat();
	int Turn();
	void setPos(unsigned int);
	int operator++(int);
	void arrested();
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
	int turn;
	int limit;
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
	int Turn();
	int Limit();
	void accept(User, Role);
	int size();
	void initPos();
	void setPos(int, unsigned int);
	void incTurn(int);
	int operator++(int);
	void arrest(int);
};

unsigned int startGame(unsigned int, int, int, int, int);
Game getGame(unsigned int);
Role stor(std::string);
std::string rtos(Role);
int selectRole(unsigned int, User, Role);
int allSelected(unsigned int);
int createPlayers(unsigned int);
unsigned int checkCondition(unsigned int, unsigned int, User, unsigned int);
int movePlayer(unsigned int, unsigned int, vertex_id*, unsigned int);
int arrested(unsigned int, std::vector <Player>*);
int gameWon(unsigned int, Role*);
#endif
