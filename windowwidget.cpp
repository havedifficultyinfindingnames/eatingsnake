#include "windowwidget.h"
#include "eatingsnake.h"
#include "map.h"

WindowWidget::WindowWidget(eatingsnake* parentClass, QWidget* parent)
	: QWidget(parent)
	, p(parentClass)
	, deadButton(new QPushButton("retry", this))
{
	deadButton->hide();
	deadButton->setGeometry(350, 400, 200, 50);
	deadButton->setFont(QFont("Bradley Hand ITC", 24, 60));

	setFocus();
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
	setPalette(pal);
	setMinimumSize(windowWidth, windowHeight);
	setMaximumSize(windowWidth, windowHeight);
	setStyleSheet("border:0.5px solid black");
	update();
	connect(deadButton, &QPushButton::clicked, p, &eatingsnake::gameInit);
}
WindowWidget::~WindowWidget()
{
	if (deadButton) delete deadButton;
}
void WindowWidget::paintMap()
{
	QPainter painter(this);
	painter.setPen(QPen(QColor(100, 100, 100), 0.5)); // the number stands for line width
	double&& drawWidth = (double)windowWidth / p->map->width;
	double&& drawHeight = (double)windowHeight / p->map->height;
	// paint the borders of the blocks
	for (int i = 1; i < p->map->height; ++i)
		painter.drawLine(0, i * drawHeight, 900, i * drawHeight);
	for (int i = 1; i < p->map->width; ++i)
		painter.drawLine(i * drawWidth, 0, i * drawWidth, 600);
	// paint the blocks of the map
	for (int i = 0; i < p->map->width; ++i)
		for (int j = 0; j < p->map->height; ++j)
		{
			QRectF drawArea(drawWidth * i, drawHeight * j, drawWidth, drawHeight);
			switch (p->map->data[i][j])
			{
			case element::blank:
				break;
			case element::wall:
				painter.setBrush(QBrush(QColor(160, 160, 160)));
				painter.drawRect(drawArea);
				break;
			case element::snakehead:
				painter.setBrush(QBrush(Qt::red));
				painter.drawRect(drawArea);
				break;
			case element::snakebody:
				painter.setBrush(QBrush(QColor(255, 255, 50)));
				painter.drawRect(drawArea);
				break;
			case element::food:
				painter.setBrush(QBrush(QColor(0, 230, 255)));
				painter.drawEllipse(drawArea);
				break;
			}
		}
}
void WindowWidget::paintPause()
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QColor(245, 255, 255)));
	painter.drawRect(0, 0, 900, 600);
	painter.setPen(Qt::black);
	painter.setFont(QFont("Bradley Hand ITC", 72, 75, 0));
	painter.drawText(325, 295, "Paused");
}
void WindowWidget::paintDead()
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setFont(QFont("Bradley Hand ITC", 72, 75, 0));
	painter.drawText(365, 260, "dead");
	painter.drawText(310, 340, p->scorelabel());
	deadButton->show();
	deadButton->setAutoFillBackground(true);
	QPalette pa = deadButton->palette();
	pa.setColor(QPalette::Window, QColor(255, 240, 220));
	deadButton->setPalette(pa);
}

void WindowWidget::paintEvent(QPaintEvent*)
{
	switch (p->gameState)
	{
	case 2: // paused
		paintPause();
		break;
	case 3: // dead
		paintMap();
		paintDead();
		break;
	default: // normal time
		deadButton->hide();
		setFocus();
		paintMap();
		break;
	}
}
