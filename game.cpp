#include "game.hpp"
#include "map.hpp"
#include "room.hpp"

std::list <Game> games;

//Player::Player(User user_, Role role_){
//	user = user_;
//	role = role_;
//}
unsigned int Player::ID(){ return player_id; }
User Player::Usr() { return user; }
websocketpp::connection_hdl Player::Hdl() const {
	return user.Hdl();
}
Role Player::Rol(){ return role; }
vertex_id Player::Pos(){ return pos; }
State Player::Stat(){ return state; }
int Player::Turn(){ return turn; }
void Player::setPos(unsigned int pos_){
	pos = pos_;	
}
int Player::operator++(int){
	int t = turn;
	turn++;
	return t;
}
void Player::arrested(){
	if(role == Rob)
		state = Arrested;
}

Game::Game(unsigned int room_id_, int cop_num_, int rob_num_, int width, int height){
	game_id = static_cast <unsigned int>(randomNum(1, 9999));
	room_id = room_id_;
	cop_num = cop_num_;
	rob_num = rob_num_;
	map = makeMap(width, height);
	map_size = width * height;
	turn = 1;
	limit = (width + height) * 2;
}
unsigned int Game::ID(){ return game_id; }
unsigned int Game::Room_id(){ return room_id; }
int Game::Cop_num(){ return cop_num; }
int Game::Rob_num(){ return rob_num; }
Vertex* Game::Map(){ return map; }
int Game::Map_size(){ return map_size; }
//std::multimap <Role, User> Game::Users(){ return users; }
std::vector <Player> Game::Players(){ return players; }
int Game::Turn(){ return turn; }
int Game::Limit(){ return limit; }
void Game::accept(User user, Role role){
	//users.insert( std::pair<Role, User>(role, user) );
	Player player(user, role);
	players.push_back(player);
}
int Game::size(){
	//return users.size();
	return players.size();
}
void Game::initPos(){
	for(int i=0; i<players.size(); i++)
		players[i].setPos(0);
	int index = 0;
	while(index != players.size()){
		unsigned int pos = static_cast<unsigned int>(randomNum(1, map_size));
		if(map[pos-1].ID() == ERASED)
			continue;
		int i;
		for(i=0; i<index; i++){
			if(players[i].Pos() == pos)
				break;
		}
		if(i == index){
			players[index].setPos(pos);
			index++;
		}
	}
	for(int i=0; i<players.size(); i++)
		std::cout << "player[" << i << "] pos: " << players[i].Pos() << std::endl; 
}
void Game::setPos(int i, unsigned int pos){
	players[i].setPos(pos);	
}
void Game::incTurn(int i){
	players[i]++;
}
int Game::operator++(int){
	int t = turn;
	turn++;
	return t;
}
void Game::arrest(int i){
	players[i].arrested();
}

unsigned int startGame(unsigned int room_id, int cop_num, int rob_num, int width, int height) {
	Game game(room_id, cop_num, rob_num, width, height);
	games.push_back(game);
	return game.ID();
}

Game getGame(unsigned int game_id) {
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++){
		if((*game).ID() == game_id){
			return *game;
		}
	}
}

Role stor(std::string str){
	try{
		if(str == "cop")
			return Cop;
		else if(str == "robber")
			return Rob;
		else
			throw "Wrong string format";
	}catch(const char* err){
		std::cout << err << std::endl;
	}
}

std::string rtos(Role role){
	if(role == Cop)
		return "cop";
	else if(role == Rob)
		return "robber";
}

int selectRole(unsigned int game_id, User user, Role role){
	int count = 0;
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++)
		if((*game).ID() == game_id)
			break;
	if(game == games.end())		// no game number
		return 1;
	unsigned int room_id = (*game).Room_id();
	Room room = getRoom(room_id);
	if(room.Index(user) == -1)		// user not in the room
		return 1;
	std::cout << "selected Role: " << role << " " << rtos(role) << std::endl;
	//std::multimap <Role, User> users = (*game).Users();
	//std::multimap <Role, User>::iterator it;
//	for(it = users.begin(); it != users.end(); it++){
//		if(it->first == role)
//			count++;
//		if(it->second == user)
//			return 1;
//	}
	std::vector <Player> players = (*game).Players();
	for(int i=0; i<players.size(); i++){
		if(players[i].Rol() == role)
			count++;
		if(players[i].Usr() == user)
			return 1;
	}
	if(role == Cop){
		if(count < (*game).Cop_num()){
			(*game).accept(user, role);
			std::cout << "count: " << count << " Cop_num: " << (*game).Cop_num() << std::endl;
			return 0;
		}
	}else {
		if(count < (*game).Rob_num()){
			(*game).accept(user, role);
			std::cout << "count: " << count << " Rob_num: " << (*game).Rob_num() << std::endl;
			return 0;
		}
	}
	std::cout << "user size: " << (*game).size() << std::endl;
	return 1;
}

int allSelected(unsigned int game_id){
	std::list <Game>::iterator it;
	for(it = games.begin(); it != games.end(); it++)
		if((*it).ID() == game_id)
			break;
	std::cout << "user size: " << (*it).size() << std::endl;
	Game game = getGame(game_id);
	std::cout << "Cop_num: " << game.Cop_num() << " Rob_num: " << game.Rob_num() << " user size: " << game.size() << std::endl;
	if(game.Cop_num() + game.Rob_num() == game.size())
		return 1;
	else
		return 0;
}

int createPlayers(unsigned int game_id){
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++)
		if((*game).ID() == game_id)
			break;
	if(game == games.end())		// no game number
		return 1;
	(*game).initPos();
	return 0;
}

unsigned int checkCondition(unsigned int game_id, unsigned int player_id, User user, unsigned int pos){
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++)
		if((*game).ID() == game_id)
			break;
	if(game == games.end())		// no game number
		return 1;
	std::vector <Player> players = (*game).Players();
	int i;
	for(i=0; i<players.size(); i++)
		if(players[i].ID() == player_id)
			break;
	if(i == players.size())		// no player number
		return 1;
	if(!(players[i].Usr() == user))		// not owner
		return 1;
	if(pos <= 0 || pos > (*game).Map_size())	// invalid vertex_id
		return 1;
	if(players[i].Stat() == Arrested)	// robber arrested
		return 1;
	return 0;
}

int movePlayer(unsigned int game_id, unsigned int player_id, vertex_id* cur_pos, unsigned int to){
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++)
		if((*game).ID() == game_id)
			break;
	std::vector <Player> players = (*game).Players();
	int i, j;
	for(i=0; i<players.size(); i++)
		if(players[i].ID() == player_id)
			break;
	*cur_pos = players[i].Pos();
	int from = static_cast<int>(*cur_pos);
	Vertex* map = (*game).Map();
	for(j=0; j<map[from-1].EdgeSize(); j++){
		if(map[from-1].getEdge(j).Dst() == to){
			(*game).setPos(i, to);
			(*game).incTurn(i);
			(*game)++;
			return 0;
		}
	}
	if(j == map[from-1].EdgeSize())	// not valid (no edge from 'from' to 'to')
		return 0;
}

int arrested(unsigned int game_id, std::vector <Player>* robbers){
	std::cout << "arrest" << std::endl;
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++)
		if((*game).ID() == game_id)
			break;
	std::vector <Player> players = (*game).Players();
	std::cout << "players.size: " << players.size() << std::endl;
	for(int i=0; i<players.size(); i++){
		std::cout << "players[" << i << "].pos = " << players[i].Pos() << std::endl;
		if(players[i].Rol() == Cop)
			continue;
		for(int j=0; j<players.size(); j++){
			if(players[j].Rol() == Rob)
				continue;
			std::cout << "players[" << j << "].pos = " << players[j].Pos() << std::endl;
			if(players[i].Pos() == players[j].Pos()){
				std::cout << "same pos" << std::endl;
				(*game).arrest(i);
				(*robbers).push_back(players[i]);
				std::cout << (*robbers).size() << std::endl;
				return 1;
			}
		}
	}
	return 0;
}

int gameWon(unsigned int game_id, Role* winner){
	std::list <Game>::iterator game;
	for(game = games.begin(); game != games.end(); game++)
		if((*game).ID() == game_id)
			break;
	std::vector <Player> players = (*game).Players();
	int i;
	for(i=0; i<players.size(); i++){
		if(players[i].Rol() == Rob && players[i].Stat() == Free){
			break;
			//return 0;
		}
	}
	if(i == players.size()){
		*winner = Cop;
		return 1;
	}
	for(int j=0; j<players.size(); j++){
		if(players[j].Stat() == Free){
			if(players[j].Turn() > (*game).Limit()){
				*winner = Rob;
				return 1;
			}else {
				return 0;
			}
		}
	}
}
