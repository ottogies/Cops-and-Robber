#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <queue>
#include "game.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl user;

std::queue <user> waiting_list;

Game::Game(user user1, user user2){
	player1 = user1;
	player2 = user2;
}

user Game::getPlayer1(){
	return player1;
}

user Game::getPlayer2(){
	return player2;
}

void game(server* s, user hdl){

	waiting_list.push(hdl);
	
	createGame();

}

void createGame() {
	user user1, user2;
	if(waiting_list.size() >= 2){
		user1 = waiting_list.front();
		waiting_list.pop();
		user2 = waiting_list.front();
		waiting_list.pop();
		Game game(user1, user2);
		user_scan(game);
	}
}

void user_scan(Game game) {
	std::cout << game.getPlayer1().lock() << std::endl;
	std::cout << game.getPlayer2().lock() << std::endl;
}
