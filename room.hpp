#include <websocketpp/config/asio_no_tls.hpp>

#include <list>

typedef websocketpp::connection_hdl User;

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
	void enter(User user);
	int size();
	void printUsers();
};

unsigned int createRoom(std::string, int);
Room joinRoom(unsigned int, User);
int checkRoom(unsigned int);
void check();
