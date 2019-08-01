#define ERASED 0

#include <set>

typedef unsigned int vertex_id;

class Edge {
	vertex_id str;
	vertex_id dst;
public:
	Edge(int, int);
	vertex_id Str();
	vertex_id Dst();
}; 

class Vertex {
	vertex_id id;
	int x;
	int y;
	std::vector <Edge> edges;
public:
	Vertex();
	Vertex(int, int, int);
	vertex_id ID();
	int X();
	int Y();
	Edge getEdge(int);
	EdgeSize();
	void erase();
	void push(Edge);
	void exclude(vertex_id);
	void showEdges();
};

Vertex* makeMap(int *);
//Vertex* makeMap();
int randomNum(int, int);
void createVertex(int, int, Vertex*);
void excludeVertex(int, Vertex*, std::set <vertex_id> &);
void createEdge(int, int, Vertex*);
void excludeEdge(std::set <vertex_id> &, int, Vertex*);
