#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl user;

class Game {
	user player1;
	user player2;
public:
	Game(user, user);
	user getPlayer1();
	user getPlayer2();
};

void game(server*, websocketpp::connection_hdl);
void user_scan(Game);
void createGame();
