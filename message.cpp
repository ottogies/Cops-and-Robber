#include <iostream>
#include <string>
#include <sstream>
#include "message.hpp"
#include "map.hpp"
#include "room.hpp"

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
	 
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
		}else {
			response << "room_join_reject," << id << delim << err;
			res = response.str();
			s->send(hdl, res, msg->get_opcode());
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
