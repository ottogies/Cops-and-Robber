#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define delim ','

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

void on_open(server*, websocketpp::connection_hdl);
void on_close(server*, websocketpp::connection_hdl);
void on_message(server*, websocketpp::connection_hdl, message_ptr);
