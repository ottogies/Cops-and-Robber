#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define delim ','

//struct User {
//	websocketpp::connection_hdl hdl;
//	std::string user_name;
//	int id;
//};

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

void on_message(server*, websocketpp::connection_hdl, message_ptr);
