#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>
#include "map.hpp"

//#define ERASED 0

Edge::Edge(int str_, int dst_){
	str = static_cast<vertex_id>(str_);
	dst = static_cast<vertex_id>(dst_);
}
vertex_id Edge::Str(){ return str; }
vertex_id Edge::Dst(){ return dst; }

Vertex::Vertex(){}
Vertex::Vertex(int id_, int x_, int y_){
	id = static_cast<vertex_id>(id_);
	x = x_;
	y = y_;
}
vertex_id Vertex::ID(){ return id; }
int Vertex::X(){ return x; }
int Vertex::Y(){ return y; }
Edge Vertex::getEdge(int i){
	return edges[i];
}
int Vertex::EdgeSize(){
	return edges.size();
}
void Vertex::erase(){
	id = ERASED;
}
void Vertex::push(Edge edge){
	edges.push_back(edge);
}
void Vertex::exclude(vertex_id erased){
	std::vector <Edge>::iterator it = edges.begin();
	while(it != edges.end()){
		if((*it).Str() == erased || (*it).Dst() == erased)
			edges.erase(it);
		else 
			it++;
	}
}
void Vertex::showEdges(){
	std::cout << "Vertex " << id << " has\t";
	for(int i=0; i<edges.size(); i++)
		std::cout << edges[i].Str() << "->" << edges[i].Dst() << "\t";
	std::cout << std::endl;
}

Vertex* makeMap(int *vertex_size) {
//Vertex* makeMap() {
	int width, height, num;
	width = randomNum(4, 7);
	height = randomNum(3, 6);
	num = width * height;
	Vertex* vertices = new Vertex[num];
	std::set <vertex_id> unfilled;
	
	createVertex(num, width, vertices);
	excludeVertex(num, vertices, unfilled);
	createEdge(num, width, vertices);
	excludeEdge(unfilled, num, vertices);
	
	for(int i=0; i<num; i++)
		vertices[i].showEdges();
	std::set <vertex_id> ::iterator it;
	for(it=unfilled.begin(); it!=unfilled.end(); it++)
		std::cout << *it << " ";
		
	*vertex_size = num;
	return vertices;	
}

int randomNum(int min, int max){		/*min~max 사이의 랜덤값 반환*/ 
	static bool first_call = true;
	if(first_call){						/*srand() 한번만 실행해주어야 한다*/ 
		srand(static_cast<unsigned int>(time(NULL)));
		first_call = false;
	}
	return rand() % (max-min+1) + min;
}

void createVertex(int num, int width, Vertex* vertices){
	int id, quo, rem, x, y;
	for(id = 1; id <=num; id++){
		quo = id / width;
		rem = id % width;
		if(rem == 0){
			x = width * 100;
			y = quo * 100;
		}else {
			x = rem * 100;
			y = (quo+1) * 100;
		}
		Vertex ver(id, x, y);
		vertices[id-1] = ver;
	}
}

void excludeVertex(int num, Vertex* vertices, std::set <vertex_id> & unfilled){
	int i, ran;
	for(i=0; i<num/10; i++){
		ran = randomNum(1, num);
		vertices[ran-1].erase();
		unfilled.insert(static_cast<vertex_id>(ran));
	}
} 

void createEdge(int num, int width, Vertex* vertices){
	int id;
	for(id=1; id<=num; id++){		//가로 edge 
		if(id % width == 0)
			continue;
		Edge edge(id, id+1);
		vertices[id-1].push(edge);
		Edge inverseEdge(id+1, id);
		vertices[id].push(inverseEdge);
	}
	for(id=1; id<=num-width; id++){			//세로 edge			
		Edge edge(id, id+width);
		vertices[id-1].push(edge);
		Edge inverseEdge(id+width, id);
		vertices[id+width-1].push(inverseEdge);
	}
}

void excludeEdge(std::set <vertex_id> & unfilled, int num, Vertex* vertices){
	std::set <vertex_id>::iterator it;
	for(it = unfilled.begin(); it != unfilled.end(); it++)
		for(int i=0; i<num; i++)
			vertices[i].exclude(*it);
}
