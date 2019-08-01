#include <iostream>
#include <string>
#include "message.hpp"
#include "map.hpp"
#include "room.hpp"

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }
    
    if(msg->get_payload() == "start_game") {
    	int vertex_size;
    	Vertex* vertices = makeMap(&vertex_size);
    	for(int i=0; i<vertex_size; i++){
    		char str[30];
    		if(vertices[i].ID() == ERASED) 
				continue;
    		sprintf(str, "vertex_create,%d,%d,%d", vertices[i].ID(), vertices[i].X(), vertices[i].Y());
			s->send(hdl, str, msg->get_opcode());
		}
		for(int i=0; i<vertex_size; i++){
			char str[30];
			for(int j=0; j<vertices[i].EdgeSize(); j++){
				sprintf(str, "edge_create,%d,%d", (vertices[i].getEdge(j)).Str(), (vertices[i].getEdge(j)).Dst());
				s->send(hdl, str, msg->get_opcode());
			}
		}
	}
	
	/*if(msg->get_payload() == "cop") {
		Room room();
		//char str[30];
		string str;
		str = assignRole(msg->get_payload(), room);
	}*/
	
	if(msg->get_payload() == "room") {
		Room room("my room", 2);
		std::cout << room.ID() << room.Title() << room.Capacity() << std::endl;
	}

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}
