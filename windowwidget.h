#ifndef WINDOWWIDGET_H
#define WINDOWWIDGET_H

#include <qwidget.h>
#include <qpalette.h>
#include <qpainter.h>
#include <qpen.h>
#include <qbrush.h>
#include <qcolor.h>
#include <qpushbutton.h>

class eatingsnake;

const int windowWidth = 900;
const int windowHeight = 600;

class WindowWidget :
	public QWidget
{
private:
	eatingsnake* p;
	QPushButton* deadButton;
public:
	WindowWidget(eatingsnake* parentClass, QWidget* parent = nullptr);
	~WindowWidget();
	void paintMap();
	void paintPause();
	void paintDead();
protected:
	virtual void paintEvent(QPaintEvent*);
};

#endif // !WINDOWWIDGET_H
