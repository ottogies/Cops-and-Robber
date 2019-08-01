class Room {
	unsigned int room_id;
	std::string title;
	int capacity;
public:
	Room(std::string, int);
	unsigned int ID();
	std::string Title();
	int Capacity();
};
