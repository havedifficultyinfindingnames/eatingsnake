#pragma once
#include <cstdlib>
#include <ctime>
#include <qobject.h>
#include <qtimer.h>
#include "map.h"

class eatingsnake;
class Buffer;

struct Node
{
	eatingsnake* p;
	int x, y;
	element type;
	Node* next;
	Node(eatingsnake* parent, int theX, int theY, element theType = element::blank, Node* theNext = nullptr);
	~Node();
	static void destructor(Node* n); // main part of the destructor function
};

class Snake : public QObject
{
	Q_OBJECT // to replace signal, put together the logic and display
			 // to replace qtimer, use system time
public:
	enum class direction :char { up, left, down, right };
	Buffer* tempdir;
private:
	eatingsnake* p;
	int speed;
	int score;
	QTimer* snakeTimer;
	Node* head;
	direction dir;
public:
	Snake(eatingsnake* parent);
	Snake(eatingsnake* parent, int x, int y);
	~Snake();
	void start_pause(int gameState);
	void changeDirection();
	void move();
private:
	void triggered(const int x, const int y);
	void grow(const int x, const int y); // grow at the head
	void cut(); // cut at the tail
signals:
	void eat(int x, int y);
	void updateScore(Snake*, int score);
};

class Buffer
{
private:
	Snake::direction* const data;
	Snake::direction* itr;
	const int maxlength;
	int len;
	bool toclear;
public:
	Buffer(int size);
	~Buffer();
	void push(Snake::direction d);
	bool get(Snake::direction& d);
	void clear();
};

class Food : public QObject
{
	Q_OBJECT
private:
	eatingsnake* p;
	Node* latest;
public:
	Food(eatingsnake* parent);
	~Food();
	void create(element type = element::food);
	bool eliminate(int x, int y);
};
