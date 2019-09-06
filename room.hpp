#include <websocketpp/config/asio_no_tls.hpp>

#include <list>

#include "message.hpp"

//typedef websocketpp::connection_hdl User;

class User{
	websocketpp::connection_hdl id;
	std::string user_name;
public:
	User();
	User(websocketpp::connection_hdl);
	//websocketpp::connection_hdl Hdl() const;
	std::shared_ptr<void> ID() const;
	websocketpp::connection_hdl Hdl() const;
	std::string User_name();
	bool operator==(const User&);
	void name(std::string);
	int local(websocketpp::connection_hdl);
};

class Room {
	unsigned int room_id;
	std::string title;
	int capacity;
	//std::list <User> users;
	std::map <int, User> users;
	User owner;
public:
	Room();
	Room(std::string, int, User);
	unsigned int ID();
	std::string Title();
	int Capacity();
	//std::list <User> Users();
	std::map <int, User> Users();
	int Index(User);
	void enter(User);
	int leave(User);
	void setOwner(User);
	int size();
	int Owner(User);
	void printUsers();
};

void access(websocketpp::connection_hdl);
void exit(websocketpp::connection_hdl);
void users();
void setName(websocketpp::connection_hdl, std::string);
User getUser(websocketpp::connection_hdl);
Room getRoom(unsigned int);
unsigned int createRoom(std::string, int, User);
Room joinRoom(unsigned int, User);
int checkRoom(unsigned int);
//std::list <Room>& roomList();
//Room leaveRoom(unsigned int, User);
std::map <int, User> leaveRoom(unsigned int, User);
std::stringstream roomList();
void check();
