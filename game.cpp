#include "game.hpp"
#include "map.hpp"

std::list <Game> games;

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
std::multimap <Role, User> Game::Players(){ return players; }
void Game::accept(User user, Role role){
	players.insert( std::pair<Role, User>(role, user) );
}
int Game::size(){
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
		else if(str == "rob")
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
		return "rob";
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
	std::multimap <Role, User> players = (*game).Players();
	std::multimap <Role, User>::iterator player;
	for(player = players.begin(); player != players.end(); player++)
		if(player->first == role)
			count++;
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
	std::cout << "player size: " << (*game).size() << std::endl;
	return 1;
}

int allSelected(unsigned int game_id){
	std::list <Game>::iterator it;
	for(it = games.begin(); it != games.end(); it++)
		if((*it).ID() == game_id)
			break;
	std::cout << "player size: " << (*it).size() << std::endl;
	Game game = getGame(game_id);
	std::cout << "Cop_num: " << game.Cop_num() << " Rob_num: " << game.Rob_num() << " player size: " << game.size() << std::endl;
	if(game.Cop_num() + game.Rob_num() == game.size())
		return 1;
	else
		return 0;
}
