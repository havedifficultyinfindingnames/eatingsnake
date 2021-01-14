#ifndef MAP_H
#define MAP_H

enum class element { blank, wall, snakehead, snakebody, food };
class eatingsnake;

class Map
{
private:
	eatingsnake* p;
public:
	element** data;
	int width, height;
public:
	Map(eatingsnake* parent, bool default = 1, int w = 24, int h = 16);
	~Map();
	static int newMap();
	static bool openMap(Map*&, const char*);
};

#endif // !MAP_H
