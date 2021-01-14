#pragma once

#include <QtWidgets/QMainWindow>
#include <qboxlayout.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qfont.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qfiledialog.h>
#include "snakeandfood.h"
#include "windowwidget.h"
#include "map.h"
//#include "ui_eatingsnake_try3.h"

class eatingsnake : public QMainWindow
{
	Q_OBJECT

private:
	//Ui::eatingsnake_try3Class ui;
	// menu bar
	QMenu* _Game, * _Map, * _Help;
	QAction* New_game, * Open_game, * Save_game, * Save_as_game;
	QAction* Open_map, * New_map;
	QAction* HelpContent;
	// status bar
	QStatusBar* statusBar;
	QLabel* timeLabel, * keyLabel; // timelabel for showing time, key label for showing which key has been pressed
	QTimer* currentTimer; // for refreshing timelabel
	// main interface
	QWidget* mainWidget;
	QHBoxLayout* mainLayout;
	QVBoxLayout* rightLayout;
	WindowWidget* w;
	QLabel* titleLabel; // showing the title
	QLabel* scoreLabel;
	QSpacerItem* rightSpacer;
	QLabel* welcomeLabel;

public:
	// game data
	int gameState; // 0 for initial state; 1 for running; 2 for pause; 3 for dead
	Map* map;
	Snake* snake1;
	Food* food;
	QTimer* screenTimer;

public:
	eatingsnake(QWidget* parent = nullptr);
	~eatingsnake();
	void gameInit();
	void updateScoreLabel(Snake* snake, int score);
	const QString scorelabel() const;

protected:
	virtual void keyPressEvent(QKeyEvent*);
};
