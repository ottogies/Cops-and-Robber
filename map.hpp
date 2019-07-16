typedef unsigned int vertex_id;

class Edge {
	vertex_id str;
	vertex_id dst;
public:
	Edge(int, int);
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
	erase();
	push(Edge);
};

void makeMap();
int randomNum(int, int);
void createVertex(int, int, Vertex*);
void excludeVertex(int, Vertex*, std::vector <int> &);
void createEdge(int, int, Vertex*);
