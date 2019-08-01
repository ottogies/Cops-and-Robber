#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::connection_hdl User;

enum Role { Cop, Rob };

class Room {
	int user_size;
	int cop_size;
	int rob_size;
	int cop_num;
	int rob_num;
public:
	Room();
	Cop_size();
	Rob_size();
	Cop_num();
	Rob_num();
	setSize();
	add_Cop_num();
	add_Rob_num();
};

class Player{
	User user;
	Role role;
public:
	Player(User, Role);
};

bool sizeOver(Role);
void assign(User, Role);
