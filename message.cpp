#include <iostream>
#include <string>
#include <sstream>
#include "message.hpp"
#include "map.hpp"
#include "room.hpp"

//std::map<websocketpp::connection_hdl, Connection> connection_map;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
           
//	if (connection_map.count(hdl) == 0) {
//		// make
//		conn = 
//	}
//	else {
//		conn = connection_map.find(hdl)->second;
//	}
	 
	std::stringstream request(msg->get_payload()), response;
	std::string req, res;
	getline(request, req, delim);

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }
    
    if(req == "start_game") {
    	int vertex_size;
    	Vertex* vertices = makeMap(&vertex_size);
    	for(int i=0; i<vertex_size; i++){
    		if(vertices[i].ID() == ERASED) continue;
			response.str("");
    		response << "vertex_create," << vertices[i].ID() << delim << vertices[i].X() << delim << vertices[i].Y();
			res = response.str();
			s->send(hdl, res, msg->get_opcode());
		}
		for(int i=0; i<vertex_size; i++){
			for(int j=0; j<vertices[i].EdgeSize(); j++){
				response.str("");
				response << "edge_create," << (vertices[i].getEdge(j)).Str() << delim << (vertices[i].getEdge(j)).Dst();
				res = response.str();
				s->send(hdl, res, msg->get_opcode());
			}
		}
	}
	
	if(req == "request_room_list") {
		response = roomList();
		res = response.str();
		std::cout << res << std::endl;
		s->send(hdl, res, msg->get_opcode());
	}
	
	if(req == "request_room_join") {
		std::string id_;
		unsigned int id;
		int err;
		Room room;
		getline(request, id_, delim);
		id = static_cast<unsigned int>(stoi(id_));
		if(!(err = checkRoom(id))){
			room = joinRoom(id, hdl);
			response << "room_join_accept," << room.ID() << delim << room.Title() << delim << room.Capacity();
			res = response.str();
			s->send(hdl, res, msg->get_opcode());
			std::list <User> users = room.Users();
			std::list <User>::iterator user;
			int count = 0;
			std::stringstream newUsr, orgUsrs;
			std::string newU, orgU;
			for(user = users.begin(); user != users.end(); user++){
				std::cout << (*user).lock() << " ";
				if(hdl.lock() == (*user).lock()){
					newUsr << "room_player_join," << (*user).lock() << delim << count << delim << 0;
					newU = newUsr.str();
				}else {
					orgUsrs << "room_player_join," << (*user).lock() << delim << count << delim << 0 << '/';
				}
				count++;
			}
			for(user = users.begin(); user != users.end(); user++){
				if(hdl.lock() == (*user).lock()){
					for(int i=0; i<users.size()-1; i++){
						getline(orgUsrs, orgU, '/');
						s->send(*user, orgU, msg->get_opcode());
					}
				}else {
					s->send(*user, newU, msg->get_opcode());
				}
			}
			std::cout << std::endl;
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
			Room room = leaveRoom(id, hdl);
			response << "room_player_leave," << hdl.lock();
			res = response.str();
			std::list <User> users = room.Users();
			std::list <User>::iterator user;
			for(user = users.begin(); user != users.end(); user++){
				s->send(*user, res, msg->get_opcode());
			}
			s->send(hdl, res, msg->get_opcode());	//자기가 없는 방에 나가기를 요청했을때도 보내짐. 자기한테만 
		}
	}
	
	if(req == "request_room_create") {
		std::string title, capacity_;
		int capacity;
		unsigned int id;
		Room room;
		getline(request, title, delim);
		getline(request, capacity_, delim);
		capacity = stoi(capacity_);
		id = createRoom(title, capacity);
		room = joinRoom(id, hdl);
		response << "room_join_accept," << room.ID() << delim << room.Title() << delim << room.Capacity();
		res = response.str();
		s->send(hdl, res, msg->get_opcode());
		check();
	}

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}
