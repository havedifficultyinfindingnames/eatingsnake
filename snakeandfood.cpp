#include "snakeandfood.h"
#include "eatingsnake.h"

Node::Node(eatingsnake* parent, int theX, int theY, element theType, Node* theNext)
	: p(parent)
	, x(theX)
	, y(theY)
	, type(theType)
	, next(theNext)
{
	p->map->data[x][y] = type;
}
Node::~Node()
{
	p->map->data[x][y] = element::blank;
}
void Node::destructor(Node* n)
{
	if (n->next)
	{
		destructor(n->next);
		delete n;
	}
	else
		delete n;
}

Snake::Snake(eatingsnake* parent)
	: p(parent)
	, speed(7)
	, score(0)
	, snakeTimer(new QTimer(this))
	, dir(direction::right)
	, tempdir(new Buffer(16))
{
	srand((unsigned)time(0));
	int&& x = 0, && y = 0;
	bool flag = 0;
	do
	{
		flag = 0;
		x = rand() % (p->map->width);
		y = rand() % (p->map->height);
		for (int i = 0; i < 4; ++i)
		{
			if (p->map->data[x + i][y] == element::wall)
			{
				flag = 1;
				break;
			}
		}
	} while (flag);
	head = new Node(parent, x, y, element::snakebody);
	head = new Node(parent, x + 1, y, element::snakebody, head);
	head = new Node(parent, x + 2, y, element::snakehead, head);
	connect(snakeTimer, &QTimer::timeout, this, &Snake::move);
}
Snake::Snake(eatingsnake* parent, int x, int y)
	: p(parent)
	, speed(7)
	, score(0)
	, snakeTimer(new QTimer(this))
	, head(new Node(parent, x, y, element::snakebody))
	, dir(direction::right)
	, tempdir(new Buffer(16))
{
	head = new Node(parent, x + 1, y, element::snakebody, head);
	head = new Node(parent, x + 2, y, element::snakehead, head);
	connect(snakeTimer, &QTimer::timeout, this, &Snake::move);
}
Snake::~Snake()
{
	Node::destructor(head);
	delete snakeTimer;
}
void Snake::start_pause(int gameState)
{
	if (gameState == 1)
		snakeTimer->start(2e3 / speed);
	else
		snakeTimer->stop();
}
void Snake::move()
{
	int&& x = 0, && y = 0;
	changeDirection();
	tempdir->clear();
	switch (dir)
	{
	case Snake::direction::up: x = head->x + 0; y = head->y - 1; break;
	case Snake::direction::left: x = head->x - 1; y = head->y + 0; break;
	case Snake::direction::down: x = head->x + 0; y = head->y + 1; break;
	case Snake::direction::right: x = head->x + 1; y = head->y + 0; break;
	}
	triggered(x, y);
}
void Snake::changeDirection()
{
	direction d;
	if (!tempdir->get(d)) return;
	switch (d)
	{
	case direction::up:
		if (dir == direction::down) return changeDirection();
		dir = d;
		break;
	case direction::left:
		if (dir == direction::right) return changeDirection();
		dir = d;
		break;
	case direction::down:
		if (dir == direction::up) return changeDirection();
		dir = d;
		break;
	case direction::right:
		if (dir == direction::left) return changeDirection();
		dir = d;
		break;
	}
}
void Snake::triggered(const int x, const int y)
{
	if (p->map->data[x][y] == element::wall)
	{
		start_pause(p->gameState = 3); // dead
	}
	else if (p->map->data[x][y] == element::food)
	{
		emit eat(x, y);
		score += 100;
		emit updateScore(this, score);
		grow(x, y);
	}
	else
	{
		cut();
		if (p->map->data[x][y] == element::snakebody)
			start_pause(p->gameState = 3); // dead
		else
			grow(x, y);
	}
}
void Snake::grow(const int x, const int y)
{
	p->map->data[head->x][head->y] = element::snakebody;
	head = new Node(p, x, y, element::snakehead, head);
}
void Snake::cut()
{
	Node* itr1 = head, * itr2 = head->next;
	while (itr2->next)
	{
		itr1 = itr2;
		itr2 = itr2->next;
	}
	itr1->next = nullptr;
	delete itr2;
}

Buffer::Buffer(int n)
	: data(new Snake::direction[n])
	, itr(data)
	, maxlength(n)
	, len(0)
	, toclear(0)
{

}
Buffer::~Buffer()
{
	delete[] data;
}
void Buffer::push(Snake::direction d)
{
	if (toclear)
	{
		itr = data;
		len = 0;
		toclear = 0;
	}
	if (len == maxlength) return;
	else data[len++] = d;
}
bool Buffer::get(Snake::direction& d)
{
	if (itr < data + len) d = *itr++;
	else return 0;
	return 1;
}
void Buffer::clear()
{
	toclear = 1;
}

Food::Food(eatingsnake* parent)
	: p(parent)
	, latest(nullptr)
{
	srand((unsigned)time(0));
	create();
	connect(p->snake1, &Snake::eat, this, &Food::eliminate);
	connect(p->snake1, &Snake::eat, this, [=]() {create(); }); // the signal and the slot "create" can't be paired
}
Food::~Food()
{
	Node::destructor(latest);
}
void Food::create(element type)
{
	int&& x = 0, && y = 0;
	do
	{
		x = rand() % (p->map->width);
		y = rand() % (p->map->height);
	} while (p->map->data[x][y] != element::blank);
	latest = new Node(p, x, y, element::food, latest);
}
bool Food::eliminate(int x, int y)
{
	Node* itr = latest;
	if (itr->x == x && itr->y == y)
	{
		latest = latest->next;
		delete itr;
		return 1;
	}
	Node* itr2 = latest;
	while (itr = itr->next)
	{
		if (itr->x == x && itr->y == y)
		{
			itr2->next = itr->next;
			delete itr;
			return 1;
		}
		itr2 = itr;
	}
	return 0;
}
