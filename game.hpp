#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl user;

typedef server::message_ptr message_ptr;

class Map {
	int width;
	int height;
public:
	Map();
	int getWidth();
	int getHeight();
};

class Game {
	user player1;
	user player2;
	Map map; 
public:
	Game(user, user);
	user getPlayer1();
	user getPlayer2();
	makeMap();
	Map getMap();
};

void game(server*, websocketpp::connection_hdl, message_ptr);
void user_scan(Game);
void createGame();
