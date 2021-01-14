#include "map.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;


Map::Map(eatingsnake* parent, bool default, int w, int h)
	: p(parent)
	, data(new element * [w])
	, width(w)
	, height(h)
{
	if (default) // with walls around
	{
		data[0] = new element[height];
		for (int i = 1; i < width - 1; ++i)
		{
			data[i] = new element[height]{ element::wall, element::blank };
			data[i][height - 1] = element::wall;
		}
		data[width - 1] = new element[height];
		for (int i = 0; i < height; ++i)
		{
			data[0][i] = element::wall;
			data[width - 1][i] = element::wall;
		}
	}
	else
	{
		for (int i = 0; i < width; ++i)
			data[i] = new element[height];
	}
}
Map::~Map()
{
	for (int i = 0; i < width; ++i)
		delete[] data[i];
	delete[] data;
}

int Map::newMap()
{
	ifstream fi;
	string s;
	int i = 0;
	do
	{
		fi.close();
		++i;
		s = ".\\map\\map" + to_string(i) + ".txt";
		fi.open(s, ios_base::in);
	} while (fi);
	fi.close();
	ofstream of(s, ios_base::out);
	of << "start and finish by doubling @ symbol and a return\n# stands for wall and 0 stands for blank space\nthe map has to be a rectangle\nall elements in ENGLISH!\n@@\n########################\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n#0000000000000000000000#\n########################\n@@\n";
	return i;
}
bool Map::openMap(Map * &map, const char* filename)
{
	string temp(filename);
	ifstream fi(temp, ios_base::in);
	char c;
	do
	{
		if (fi.eof()) return 0;
		while (!fi.eof() && (c = fi.get()) != '@') continue;
	} while (c = fi.get() != '@');
	fi.get();
	getline(fi, temp);
	int&& width = temp.length();
	vector<string*> Vector(width, nullptr);
	for (int i = 0; i < width; ++i)
	{
		Vector[i] = new string(1, temp[i]);
	}
	while (!fi.eof())
	{
		getline(fi, temp);
		if (temp == "@@")
		{
			eatingsnake* copy = map->p;
			int&& height = Vector[0]->length();
			delete map;
			map = new Map(copy, 0, width, height);
			for (int i = 0; i < width; ++i)
			{
				int j = 0;
				for (char c : *(Vector[i]))
				{
					switch (c)
					{
					case '#':
						map->data[i][j] = element::wall;
						break;
					default:
						map->data[i][j] = element::blank;
						break;
					}
					++j;
				}
				delete Vector[i];
			}
			return 1;
		}
		for (int i = 0; i < width && i < temp.length(); ++i)
			Vector[i]->append(1, temp[i]);
	}
	for (int i = 0; i < width; ++i)
		delete Vector[i];
	return 0;
}
