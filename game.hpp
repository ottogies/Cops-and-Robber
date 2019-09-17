#include <map>
#include "room.hpp"
#include "message.hpp"
#include "map.hpp"


typedef enum { Cop, Rob } Role;

class Game {
	unsigned int game_id;
	unsigned int room_id;
	int cop_num;
	int rob_num;
	Vertex* map;
	int map_size;
	std::multimap <Role, User> players;
public:
	Game(unsigned int, int, int, int, int);
	unsigned int ID();
	unsigned int Room_id();
	int Cop_num();
	int Rob_num();
	Vertex* Map();
	int Map_size();
	std::multimap <Role, User> Players();
	void accept(User, Role);
	int size();
};

unsigned int startGame(unsigned int, int, int, int, int);
Game getGame(unsigned int);
Role stor(std::string);
std::string rtos(Role);
int selectRole(unsigned int, User, Role);
int allSelected(unsigned int);
