#include "game.hpp"
#include "map.hpp"
#include "room.hpp"

std::list <Game> games;

//Player::Player(User user_, Role role_){
//	user = user_;
//	role = role_;
//}
User Player::User() { return user; }
Role Player::Rol(){ return role; }

Game::Game(unsigned int room_id_, int cop_num_, int rob_num_, int width, int height){
	game_id = static_cast <unsigned int>(randomNum(1, 9999));
	room_id = room_id_;
	cop_num = cop_num_;
	rob_num = rob_num_;
	map = makeMap(width, height);
	map_size = width * height;
}
unsigned int Game::ID(){ return game_id; }
unsigned int Game::Room_id(){ return room_id; }
int Game::Cop_num(){ return cop_num; }
int Game::Rob_num(){ return rob_num; }
Vertex* Game::Map(){ return map; }
int Game::Map_size(){ return map_size; }
//std::multimap <Role, User> Game::Users(){ return users; }
std::vector <Player> Game::Players(){ return players; }
void Game::accept(User user, Role role){
	//users.insert( std::pair<Role, User>(role, user) );
	Player player(user, role);
	players.push_back(player);
}
int Game::size(){
	//return users.size();
	return players.size();
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
		if(players[i].User() == user)
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
