#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "map.hpp"

#define ERASED 0

Vertex::Vertex(){}
Vertex::Vertex(int id_, int x_, int y_){
	id = static_cast<vertex_id>(id_);
	x = x_;
	y = y_;
}
Vertex::ID(){ return id; }
Vertex::X(){ return x; }
Vertex::Y(){ return y; }
Vertex::erase(){
	id = ERASED;
}
Vertex::push(Edge edge){
	edges.push_back(edge);
}

Edge::Edge(int str_, int dst_){
	str = static_cast<vertex_id>(str_);
	dst = static_cast<vertex_id>(dst_);
}

void makeMap() {
	int width, height, num;
	width = randomNum(4, 7);
	height = randomNum(3, 6);
	num = width * height;
	Vertex* vertices = new Vertex[num];
	std::vector <int> unfilled;
	
	createVertex(num, width, vertices);
	excludeVertex(num, vertices, unfilled);
	createEdge(num, width, vertices);

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

void excludeVertex(int num, Vertex* vertices, std::vector <int> & unfilled){
	int i, ran;
	for(i=0; i<num/10; i++){
		ran = randomNum(1, num);
		vertices[ran-1].erase();
		unfilled.push_back(ran);
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

