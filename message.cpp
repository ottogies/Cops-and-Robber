#include <iostream>
#include <string>
#include <sstream>
#include "message.hpp"
#include "map.hpp"
#include "room.hpp"

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

	if(req == "init"){
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
    
//    if(req == "start_game") {
//    	int vertex_size;
//    	Vertex* vertices = makeMap(&vertex_size);
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
//	}
	
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
				response.str("");
				response << "room_player_join," << it->second.ID() << delim << it->second.User_name() << delim 
						 << room.Index(it->second) << delim << it->second.local(hdl) << delim << room.Owner(it->second);
				res = response.str();
				if(!(it->second == user)){
					s->send(user.Hdl(), res, msg->get_opcode());
				}else {
					std::map <int, User>::iterator itr;
					for(itr = users.begin(); itr != users.end(); itr++)
						s->send(itr->second.Hdl(), res, msg->get_opcode());
				}
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
		std::string id_;
		unsigned int id;
		getline(request, id_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		Room room = getRoom(id);
		if(room.Owner(user)){
			response << "game_start," << randomNum(1, 1000) << delim << randomNum(1, 2) << delim << randomNum(1, 2);
			res = response.str();
			std::map <int, User> users = room.Users();
			std::map <int, User>::iterator it;
			for(it = users.begin(); it != users.end(); it++){
				s->send(it->second.Hdl(), res, msg->get_opcode());
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
