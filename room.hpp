#include <websocketpp/config/asio_no_tls.hpp>

#include <list>

#include "message.hpp"

typedef websocketpp::connection_hdl User;

//class User{
//	websocketpp::connection_hdl hdl;
//	std::string user_name;
//};

class Room {
	unsigned int room_id;
	std::string title;
	int capacity;
	std::list <User> users;
public:
	Room();
	Room(std::string, int);
	unsigned int ID();
	std::string Title();
	int Capacity();
	std::list <User> Users();
	void enter(User);
	int leave(User);
	int size();
	void printUsers();
};

unsigned int createRoom(std::string, int);
Room joinRoom(unsigned int, User);
int checkRoom(unsigned int);
//std::list <Room>& roomList();
Room leaveRoom(unsigned int, User);
std::stringstream roomList();
void check();
