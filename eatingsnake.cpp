#include "eatingsnake.h"

eatingsnake::eatingsnake(QWidget* parent)
	: QMainWindow(parent)
	, _Game(new QMenu("Game", this))
	, _Map(new QMenu("Map", this))
	, _Help(new QMenu("Help", this))
	, New_game(new QAction("New Game", _Game))
	, Open_game(new QAction("Open Game Record", _Game))
	, Save_game(new QAction("Save Record", _Game))
	, Save_as_game(new QAction("Save as...", _Game))
	, Open_map(new QAction("Open Map", _Map))
	, New_map(new QAction("New Map", _Map))
	, HelpContent(new QAction("Help", _Help))
	, statusBar(new QStatusBar(this))
	, timeLabel(new QLabel(this))
	, keyLabel(new QLabel("Key pressed: ", this))
	, currentTimer(new QTimer(this))
	, mainWidget(new QWidget(this))
	, mainLayout(new QHBoxLayout)
	, rightLayout(new QVBoxLayout)
	, w(new WindowWidget(this, mainWidget)) //参数暂时是this 和 QWidget*
	, titleLabel(new QLabel("Eating\nSnake", mainWidget))
	, scoreLabel(new QLabel(mainWidget))
	, rightSpacer(new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding))
	, welcomeLabel(new QLabel("Welcome to the eating snake game!\nstart by press the direction key\nor WASD", this))
	, gameState(0)
	, map(new Map(this))
	, snake1(new Snake(this))
	, food(new Food(this))
	, screenTimer(new QTimer(this))
{
	//ui.setupUi(this);
	// window
	setWindowTitle("Eating Snake");
	setMinimumSize(1200, 680);
	setMaximumSize(1200, 680);

	// menu bar
	menuBar()->addMenu(_Game);
	menuBar()->addMenu(_Map);
	menuBar()->addMenu(_Help);
	_Game->addAction(New_game);
	_Game->addSeparator();
	_Game->addAction(Open_game);
	_Game->addAction(Save_game);
	_Game->addAction(Save_as_game);
	_Map->addAction(Open_map);
	_Map->addSeparator();
	_Map->addAction(New_map);
	_Help->addAction(HelpContent);

	connect(New_game, &QAction::triggered, this, &eatingsnake::gameInit);
	connect(Open_map, &QAction::triggered, this, [=]()
		{
			QString filename = QFileDialog::getOpenFileName(this, "Open map", ".\\map\\");
			if (filename.isEmpty())
			{
				QMessageBox::critical(this, "failed", "open map failed");
				return;
			}
			if (!map->openMap(map, filename.toLocal8Bit().constData())) QMessageBox::critical(this, "failed", "file doesn't contain a valid map");
			else
			{
				delete snake1;
				delete food;
				snake1 = new Snake(this);
				food = new Food(this);
				QMessageBox::information(this, "success", "Successed!");
			}
		});
	connect(New_map, &QAction::triggered, this, [=]()
		{
			int i = Map::newMap();
			QString s("map");
			s += QString::number(i);
			QMessageBox::information(this, "success", s);
		});
	connect(HelpContent, &QAction::triggered, this, [=]() { QMessageBox::information(this, "help", "enjoy yourself!"); });

	// status bar
	setStatusBar(statusBar);
	statusBar->addWidget(timeLabel);
	currentTimer->start(1000);
	connect(currentTimer, &QTimer::timeout, this, [&]() { timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss")); });
	statusBar->addPermanentWidget(keyLabel);

	// main interface
	titleLabel->setAlignment(Qt::AlignHCenter);
	titleLabel->setFixedSize(200, 200);
	titleLabel->setFont(QFont("Blackadder ITC", 40, 60, 0));
	welcomeLabel->setGeometry(220, 220, 500, 120);
	welcomeLabel->setAlignment(Qt::AlignHCenter);
	welcomeLabel->setFont(QFont("Comic Sans MS", 20, 30, 0));
	scoreLabel->setText("Score: 0");

	// layouts
	mainLayout->addWidget(w);
	mainLayout->addLayout(rightLayout);
	mainWidget->setLayout(mainLayout);
	mainWidget->setGeometry(0, 0, 1200, 660);
	rightLayout->addWidget(titleLabel);
	rightLayout->addWidget(scoreLabel);
	rightLayout->addSpacerItem(rightSpacer);

	// others
	screenTimer->start(20);
	connect(screenTimer, SIGNAL(timeout()), this, SLOT(update()));
	connect(snake1, &Snake::updateScore, this, &eatingsnake::updateScoreLabel);
}
eatingsnake::~eatingsnake()
{
	if (food) delete food;
	if (snake1) delete snake1;
	if (map) delete map;
}

void eatingsnake::gameInit()
{
	welcomeLabel->hide();
	if (snake1) delete snake1;
	if (food) delete food;
	snake1 = new Snake(this);
	food = new Food(this);
	snake1->start_pause(gameState = 1);
	scoreLabel->setText("Score: 0");
	connect(screenTimer, SIGNAL(timeout()), this, SLOT(update()));
	connect(snake1, &Snake::updateScore, this, &eatingsnake::updateScoreLabel);
}
void eatingsnake::updateScoreLabel(Snake* snake, int score)
{
	QString s("Score:");
	s += QString::number(score);
	scoreLabel->setText(s);
}
const QString eatingsnake::scorelabel() const
{
	return scoreLabel->text();
}

void eatingsnake::keyPressEvent(QKeyEvent* e)
{
	QString keyString("Key pressed: ");
	keyString += QKeySequence(e->key()).toString();
	keyLabel->setText(keyString);
	if (e->key() == Qt::Key_Escape)
		if (QMessageBox::question(this, "warning", "sure to leave?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
			close();
	switch (gameState)
	{
	case 0:
		switch (e->key())
		{
		case Qt::Key_W:
		case Qt::Key_Up:
			welcomeLabel->hide();
			snake1->start_pause(gameState = 1);
			snake1->tempdir->push(Snake::direction::up);
			break;
		case Qt::Key_S:
		case Qt::Key_Down:
			welcomeLabel->hide();
			snake1->start_pause(gameState = 1);
			snake1->tempdir->push(Snake::direction::down);
			break;
		case Qt::Key_D:
		case Qt::Key_Right:
			welcomeLabel->hide();
			snake1->start_pause(gameState = 1);
			break;
		}
		break;
	case 1:
		switch (e->key())
		{
		case Qt::Key_W:
		case Qt::Key_Up:
			snake1->tempdir->push(Snake::direction::up);
			break;
		case Qt::Key_A:
		case Qt::Key_Left:
			snake1->tempdir->push(Snake::direction::left);
			break;
		case Qt::Key_S:
		case Qt::Key_Down:
			snake1->tempdir->push(Snake::direction::down);
			break;
		case Qt::Key_D:
		case Qt::Key_Right:
			snake1->tempdir->push(Snake::direction::right);
			break;
		case Qt::Key_Space:
			snake1->start_pause(gameState = 2);
			break;
		}
		break;
	case 2:
		if (e->key() == Qt::Key_Space) snake1->start_pause(gameState = 1);
		break;
	}
}
