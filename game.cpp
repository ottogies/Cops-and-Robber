#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <string>
#include "game.hpp"
#include "map.hpp"

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

Game::makeMap(){
	this->map = Map();
}

Map Game::getMap(){
	return map;
}

Map::Map(){
	srand(static_cast<unsigned int>(time(NULL)));
	width = rand() % 6 + 5;			/* 5~10 사이의 난수 생성 */ 
	height = rand() % 6 + 5;
}

int Map::getWidth(){
	return width;
}

int Map::getHeight(){
	return height;
}

void game(server* s, user hdl, message_ptr msg){

	//waiting_list.push(hdl);
	
	//createGame();
	//std::string str = "vertex_create,";
	//std::string str2;
	//char str[128];
	//char str[15] = "vertex_create,";;
	char str2[128];
	
	Vertex* vertices;
	
	//vertices = makeMap();
	
//	for(int i=0; i<N; i++){
//		char str[100] = "vertex_create,";
//		sprintf(str, "vertex_create,%d,%d,%d",
//			vertices[i].ID(),vertices[i].X(),vertices[i].Y());
////		sprintf(str2, "%d", vertices[i].ID());
////		strcat(str, str2);
////		strcat(str, ",");
////		sprintf(str2, "%d", vertices[i].X());
////		strcat(str, str2);
////		strcat(str, ",");
////		sprintf(str2, "%d", vertices[i].Y());
////		strcat(str, str2);
////		strcat(str, ",");
//		std::cout << str << std::endl;
//		s->send(hdl, str, msg->get_opcode());
//	}
	/*
	len = sprintf(buf, "Hello,\n");

    for (i=0; i<5; i++)
        len += sprintf(buf + len, "%d ",i);
	*/
	//s->send(hdl, msg->get_payload(), msg->get_opcode());
}

void createGame() {
	user user1, user2;
	if(waiting_list.size() >= 2){
		user1 = waiting_list.front();
		waiting_list.pop();
		user2 = waiting_list.front();
		waiting_list.pop();
		Game game(user1, user2);
		game.makeMap();
		user_scan(game);
	}
}

void user_scan(Game game) {
	std::cout << game.getPlayer1().lock() << std::endl;
	std::cout << game.getPlayer2().lock() << std::endl;
	std::cout << game.getMap().getWidth() << "x" << game.getMap().getHeight() << std::endl;
}
