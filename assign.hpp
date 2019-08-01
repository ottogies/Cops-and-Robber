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
	int Cop_size();
	int Rob_size();
	int Cop_num();
	int Rob_num();
	void setSize();
	void add_Cop_num();
	void add_Rob_num();
};

class Player{
	User user;
	Role role;
public:
	Player(User, Role);
};

bool sizeOver(Role);
void assign(User, Role);
