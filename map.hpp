#define ERASED 0

typedef unsigned int vertex_id;

class Edge {
	vertex_id str;
	vertex_id dst;
public:
	Edge(int, int);
	Str();
	Dst();
}; 

class Vertex {
	vertex_id id;
	int x;
	int y;
	std::vector <Edge> edges;
public:
	Vertex();
	Vertex(int, int, int);
	ID();
	X();
	Y();
	Edge getEdge(int);
	EdgeSize();
	erase();
	push(Edge);
	exclude(vertex_id);
	showEdges();
};

Vertex* makeMap(int *);
//Vertex* makeMap();
int randomNum(int, int);
void createVertex(int, int, Vertex*);
void excludeVertex(int, Vertex*, std::set <vertex_id> &);
void createEdge(int, int, Vertex*);
void excludeEdge(std::set <vertex_id> &, int, Vertex*);
