#include <iostream>
#include <string>
#include <sstream>
#include "message.hpp"
#include "map.hpp"
#include "room.hpp"
#include "game.hpp"

void on_open(server* s, websocketpp::connection_hdl hdl) {
	std::cout << hdl.lock() << " joined the page" << std::endl;
	access(hdl);
	users();
}
void on_close(server* s, websocketpp::connection_hdl hdl) {
	std::cout << hdl.lock() << " closed the page" << std::endl;
	exit(hdl);
	users();
}

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
	
	std::stringstream request(msg->get_payload()), response;
	std::string req, res;
	getline(request, req, delim);
	User user;

	if(req == "request_username"){
		std::string user_name;
		getline(request, user_name, delim);
		setName(hdl, user_name);
		user = getUser(hdl);
		response << "player_init," << user.ID() << delim << user.User_name();
		res = response.str();
		s->send(hdl, res, msg->get_opcode());
	}else {
		user = getUser(hdl);
		std::cout << user.User_name() << std::endl;
	}
	
	if(req == "request_room_list") {
		response = roomList();
		res = response.str();
		std::cout << res << std::endl;
		s->send(hdl, res, msg->get_opcode());
	}
	
	if(req == "request_room_create") {
		std::string title, capacity_;
		int capacity;
		unsigned int id;
		Room room;
		getline(request, title, delim);
		getline(request, capacity_, delim);
		capacity = stoi(capacity_);
		id = createRoom(title, capacity, user);
		room = joinRoom(id, user);
		response << "room_join_accept," << room.ID() << delim << room.Title() << delim << room.Capacity();
		res = response.str();
		s->send(hdl, res, msg->get_opcode());
		response.str("");
		response << "room_player_join," << user.ID() << delim << user.User_name() << delim 
		         << room.Index(user) << delim << user.local(hdl) << delim << room.Owner(user);
		res = response.str();
		s->send(hdl, res, msg->get_opcode());
		check();
	}
	
	if(req == "request_room_join") {
		std::string id_;
		unsigned int id;
		int err;
		getline(request, id_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		if(!(err = checkRoom(id))){
			Room room = joinRoom(id, user);
			response << "room_join_accept," << room.ID() << delim << room.Title() << delim << room.Capacity();
			res = response.str();
			s->send(hdl, res, msg->get_opcode());
			/*원래코드*/ 
//			std::list <User> users = room.Users();
//			std::list <User>::iterator user;
//			int count = 0;
//			std::stringstream newUsr, orgUsrs;
//			std::string newU, orgU;
//			for(user = users.begin(); user != users.end(); user++){
//				std::cout << (*user).ID() << " ";
//				if(hdl.lock() == (*user).ID()){
//					newUsr << "room_player_join," << (*user).ID() << delim << count << delim << 0;
//					newU = newUsr.str();
//					orgUsrs << "room_player_join," << (*user).ID() << delim << count << delim << 1 << '/'; /*추가*/ 
//				}else {
//					orgUsrs << "room_player_join," << (*user).ID() << delim << count << delim << 0 << '/';
//				}
//				count++;
//			}
//			for(user = users.begin(); user != users.end(); user++){
//				if(hdl.lock() == (*user).ID()){
//					for(int i=0; i<users.size(); i++){
//						getline(orgUsrs, orgU, '/');
//						s->send((*user).hdl(), orgU, msg->get_opcode());
//					}
//				}else {
//					s->send((*user).hdl(), newU, msg->get_opcode());
//				}
//			}
//			std::cout << std::endl;
			/*새 코드*/
			std::map <int, User> users = room.Users();
			std::map <int, User>::iterator it;
			for(it = users.begin(); it != users.end(); it++){
				if(!(it->second == user)){
					response.str("");
					response << "room_player_join," << it->second.ID() << delim << it->second.User_name() << delim 
						     << room.Index(it->second) << delim << user.local(it->second.Hdl()) << delim << room.Owner(it->second);
					res = response.str();
					s->send(user.Hdl(), res, msg->get_opcode());
				}else {
					std::map <int, User>::iterator itr;
					for(itr = users.begin(); itr != users.end(); itr++){
						response.str("");
						response << "room_player_join," << it->second.ID() << delim << it->second.User_name() << delim 
							 	 << room.Index(it->second) << delim << itr->second.local(it->second.Hdl()) << delim << room.Owner(it->second);
						res = response.str();
						s->send(itr->second.Hdl(), res, msg->get_opcode());
					}
				}
//				response.str("");
//				response << "room_player_join," << it->second.ID() << delim << it->second.User_name() << delim 
//						 << room.Index(it->second) << delim << it->second.local(hdl) << delim << room.Owner(it->second);
//				res = response.str();
//				if(!(it->second == user)){
//					s->send(user.Hdl(), res, msg->get_opcode());
//				}else {
//					std::map <int, User>::iterator itr;
//					for(itr = users.begin(); itr != users.end(); itr++)
//						s->send(itr->second.Hdl(), res, msg->get_opcode());
//				}
			}
			/*여까지*/ 
		}else {
			response << "room_join_reject," << id << delim << err;
			res = response.str();
			s->send(hdl, res, msg->get_opcode());
		}
	}
	
	if(req == "request_room_leave") {
		std::string id_;
		unsigned int id;
		getline(request, id_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		if(checkRoom(id) != 1){		// 1 = No room number
			/*원래코드*/ 
//			Room room = leaveRoom(id, user);
//			response << "room_player_leave," << user.ID();
//			res = response.str(); 
//			std::list <User> users = room.Users();
//			std::list <User>::iterator user;
//			for(user = users.begin(); user != users.end(); user++){
//				s->send((*user).hdl(), res, msg->get_opcode());
//			}
//			s->send(hdl, res, msg->get_opcode());	//자기가 없는 방에 나가기를 요청했을때도 보내짐. 자기한테만 
			/*새코드*/
			Room room = getRoom(id);
			std::map <int, User> users = room.Users();
			std::map <int, User> leaving = leaveRoom(id, user);
			std::map <int, User>::iterator it;
			for(it = leaving.begin(); it!= leaving.end(); it++){
				response.str("");
				response << "room_player_leave," << it->second.ID();
				res = response.str();
				std::map <int, User>::iterator itr;
				for(itr = users.begin(); itr!= users.end(); itr++)
					s->send(itr->second.Hdl(), res, msg->get_opcode());
			}
			/*여까지*/ 
		}
	}
	
	if(req == "request_game_start") {
		std::string room_id_, width_, height_, cop_num_, rob_num_;
		unsigned int room_id, game_id;
		int width, height, cop_num, rob_num;
		getline(request, room_id_, delim);
		getline(request, width_, delim);
		getline(request, height_, delim);
		getline(request, cop_num_, delim);
		getline(request, rob_num_, delim);
		room_id = static_cast<unsigned int>(stoi(room_id_));
		width = stoi(width_);
		height = stoi(height_);
		cop_num = stoi(cop_num_);
		rob_num = stoi(rob_num_);
		Room room = getRoom(room_id);
		if(room.Owner(user)){
			game_id = startGame(room_id, cop_num, rob_num, width, height);
			Game game = getGame(game_id);
			response << "game_start," << game.ID() << delim << game.Cop_num() << delim << game.Rob_num() << delim << user.ID();
			res = response.str();
			std::map <int, User> users = room.Users();
			std::map <int, User>::iterator it;
			for(it = users.begin(); it != users.end(); it++)
				s->send(it->second.Hdl(), res, msg->get_opcode());
		}
	}
	
	if(req == "request_game_role_select"){
		std::string id_, role_;
		unsigned int id;
		getline(request, id_, delim);
		getline(request, role_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		Role role = stor(role_);
		if(!selectRole(id, user, role)){
			Game game = getGame(id);
			Room room = getRoom(game.Room_id());
			response << "role_select," << user.ID() << delim << rtos(role);
			res = response.str();
			std::map <int, User> users = room.Users();
			std::map <int, User>::iterator it;
			for(it = users.begin(); it != users.end(); it++)
				s->send(it->second.Hdl(), res, msg->get_opcode());
			if(allSelected(id)){
//				std::multimap <Role, User> users_with_role = game.Users();
//				std::multimap <Role, User>::iterator itr;
//				response.str("");
//				response << "game_role_data," << users_with_role.size();
//				for(itr = users_with_role.begin(); itr != users_with_role.end(); itr++)
//					response << delim << itr->second.ID() << delim << itr->second.User_name() << delim << rtos(itr->first);
//				res = response.str();
//				for(it = users.begin(); it != users.end(); it++)
//					s->send(it->second.Hdl(), res, msg->get_opcode());
				std::vector <Player> players = game.Players();
				response.str("");
				response << "game_role_data," << players.size();
				for(int i=0; i<players.size(); i++)
					response << delim << players[i].Usr().ID() << delim << players[i].Usr().User_name() << delim << rtos(players[i].Rol());
				res = response.str();
				for(it = users.begin(); it != users.end(); it++)
					s->send(it->second.Hdl(), res, msg->get_opcode());
			}
		}
	}
	
	if(req == "request_map_data") {
		std::string id_;
		unsigned int id;
		getline(request, id_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		Game game = getGame(id);
		//Room room = getRoom(game.Room_id());
		//std::map <int, User> users = room.Users();
		//std::map <Role, User> users = game.Users();
		//std::map <Role, User>::iterator it;
		//std::multimap <Role, User> users = game.Users();
		//std::multimap <Role, User>::iterator it;
		std::vector <Player> players = game.Players();
		response << "game_map_data_start";
		res = response.str();
		//for(it = users.begin(); it != users.end(); it++)
		//	s->send(it->second.Hdl(), res, msg->get_opcode());
		for(int i=0; i<players.size(); i++)
			s->send(players[i].Hdl(), res, msg->get_opcode());
    	//int vertex_size;
    	//Vertex* vertices = makeMap(&vertex_size);
    	int map_size = game.Map_size();
    	Vertex* map = game.Map();
    	for(int i=0; i<map_size; i++){
    		if(map[i].ID() == ERASED) continue;
			response.str("");
    		response << "vertex_create," << map[i].ID() << delim << map[i].X() << delim << map[i].Y();
			res = response.str();
			std::cout << res << std::endl;
			//for(it = users.begin(); it != users.end(); it++)
			//	s->send(it->second.Hdl(), res, msg->get_opcode());
			for(int i=0; i<players.size(); i++)
				s->send(players[i].Hdl(), res, msg->get_opcode());
		}
		for(int i=0; i<map_size; i++){
			for(int j=0; j<map[i].EdgeSize(); j++){
				response.str("");
				response << "edge_create," << (map[i].getEdge(j)).Str() << delim << (map[i].getEdge(j)).Dst();
				res = response.str();
				std:: cout << res << std::endl;
				//for(it = users.begin(); it != users.end(); it++)
				//	s->send(it->second.Hdl(), res, msg->get_opcode());
				for(int i=0; i<players.size(); i++)
					s->send(players[i].Hdl(), res, msg->get_opcode());
			}
		}
//    	for(int i=0; i<vertex_size; i++){
//    		if(vertices[i].ID() == ERASED) continue;
//			response.str("");
//    		response << "vertex_create," << vertices[i].ID() << delim << vertices[i].X() << delim << vertices[i].Y();
//			res = response.str();
//			s->send(hdl, res, msg->get_opcode());
//		}
//		for(int i=0; i<vertex_size; i++){
//			for(int j=0; j<vertices[i].EdgeSize(); j++){
//				response.str("");
//				response << "edge_create," << (vertices[i].getEdge(j)).Str() << delim << (vertices[i].getEdge(j)).Dst();
//				res = response.str();
//				s->send(hdl, res, msg->get_opcode());
//			}
//		}
		response.str("");
		response << "game_map_data_end";
		res = response.str();
		//for(it = users.begin(); it != users.end(); it++)
		//	s->send(it->second.Hdl(), res, msg->get_opcode());
		for(int i=0; i<players.size(); i++)
			s->send(players[i].Hdl(), res, msg->get_opcode());	
	}
	
	if(req == "request_agent_create") {
		std::string id_;
		unsigned int id;
		getline(request, id_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		if(!createPlayers(id)){
			Game game = getGame(id);
			std::vector <Player> players = game.Players();
			for(int i=0; i<players.size(); i++){
				response.str("");
				response << "agent_create," << players[i].Usr().ID() << delim << players[i].ID() << delim 
						 << rtos(players[i].Rol()) << delim << players[i].Pos(); 
				res = response.str();
				for(int j=0; j<players.size(); j++)
					s->send(players[j].Hdl(), res, msg->get_opcode());
			}
			response.str("");
			response << "agent_move_turn," << players[0].Usr().ID() << delim << players[0].ID();
			res = response.str();
			for(int j=0; j<players.size(); j++)
				s->send(players[j].Hdl(), res, msg->get_opcode());
		}
	}
	
	if(req == "request_agent_turn") {
		
	}
	
	if(req == "request_agent_move") {
		std::string game_id_, player_id_, pos_;
		unsigned int game_id, player_id;
		vertex_id pos, cur_pos;
		getline(request, game_id_, delim);
		getline(request, player_id_, delim);
		getline(request, pos_, delim);
		game_id = static_cast<unsigned int>(stoi(game_id_));
		player_id = static_cast<unsigned int>(stoi(player_id_));
		pos = static_cast<unsigned int>(stoi(pos_));
		std::cout << "out" << std::endl;
		if(!checkCondition(game_id, player_id, user, pos)){
			std::cout << "check" << std::endl;
			if(!movePlayer(game_id, player_id, &cur_pos, pos)){
				std::cout << "move" << std::endl;
				Game game = getGame(game_id);
				std::vector <Player> players = game.Players();
				int i;
				for(i=0; i<players.size(); i++){
					if(players[i].ID() == player_id){
						response << "agent_move," << players[i].Usr().ID() << delim << players[i].ID() << delim 
								 << players[i].Pos() << delim << cur_pos;
						break;
					}
				}
				res = response.str();
				for(int j=0; j<players.size(); j++)
					s->send(players[j].Hdl(), res, msg->get_opcode());
				
				if(++i == players.size())
					i = 0;
				response.str("");
				response << "agent_move_turn," << players[i].Usr().ID() << delim << players[i].ID();
				res = response.str();
				for(int j=0; j<players.size(); j++)
					s->send(players[j].Hdl(), res, msg->get_opcode());
			}
		}
	}

//	// check for a special command to instruct the server to stop listening so
//    // it can be cleanly exited.
//    if (msg->get_payload() == "stop-listening") {
//        s->stop_listening();
//        return;
//    }
//	
//    try {
//        s->send(hdl, msg->get_payload(), msg->get_opcode());
//    } catch (websocketpp::exception const & e) {
//        std::cout << "Echo failed because: "
//                  << "(" << e.what() << ")" << std::endl;
//    }
}
