#include "MainWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QTimer>
#include <unordered_map>
#include <cmath>

static QColor domainColor(Domain domain) {
    switch (domain) {
        case Domain::Home: return QColor(172, 130, 90);
        case Domain::School: return QColor(180, 180, 255);
        case Domain::Friends: return QColor(255, 215, 180);
        case Domain::Leisure: return QColor(45, 160, 80);
        case Domain::Path: return QColor(120, 120, 120);
        case Domain::Public: return QColor(245, 245, 245);
        default: return QColor(200, 200, 200);
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), world_(std::make_shared<GridWorld>(20, 20)), simulation_(std::make_shared<Simulation>(world_)), updateTimer_(new QTimer(this)), view_(new QGraphicsView(this)), scene_(new QGraphicsScene(this)), infoLabel_(new QLabel(this)), logText_(new QTextEdit(this)) {
    setupUI();
    setupMenus();

    std::unordered_map<Domain, double> preferences1 = {
        {Domain::Home, 0.4}, {Domain::School, 0.1}, {Domain::Friends, 0.1}, {Domain::Leisure, 0.2}, {Domain::Public, 0.2}
    };
    std::unordered_map<Domain, double> preferences2 = {
        {Domain::Home, 0.2}, {Domain::School, 0.1}, {Domain::Friends, 0.4}, {Domain::Leisure, 0.2}, {Domain::Public, 0.1}
    };

    world_->addAgent(std::make_shared<Agent>(1, 15, Position(7, 4), Domain::Home, preferences1, true, 0.5));
    world_->addAgent(std::make_shared<Agent>(2, 17, Position(8, 9), Domain::Path, preferences2, true, 0.7));
    world_->addAgent(std::make_shared<Agent>(3, 14, Position(6, 14), Domain::Leisure, preferences1, false, 0.8));

    view_->setScene(scene_);
    view_->setRenderHint(QPainter::Antialiasing);
    drawGrid();
    updateStatusPanel();

    connect(updateTimer_, &QTimer::timeout, this, &MainWindow::updateDisplay);
    updateTimer_->start(100);
}

MainWindow::~MainWindow() {
    simulation_->stop();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->addWidget(view_);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* startButton = new QPushButton("Start");
    QPushButton* stopButton = new QPushButton("Stop");
    QPushButton* stepButton = new QPushButton("Step");

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::stepSimulation);

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(stepButton);
    layout->addLayout(buttonLayout);

    infoLabel_->setMinimumHeight(50);
    infoLabel_->setWordWrap(true);
    layout->addWidget(infoLabel_);

    logText_->setReadOnly(true);
    logText_->setMinimumHeight(120);
    layout->addWidget(logText_);
}

void MainWindow::setupMenus() {
    setWindowTitle("Agent-Based Model - 2.5D Simulation");
}

void MainWindow::updateStatusPanel() {
    int agents = static_cast<int>(world_->getAgents().size());
    int friendships = 0;
    for (const auto& agent : world_->getAgents()) {
        friendships += static_cast<int>(agent->getFriends().size());
    }
    friendships /= 2;

    infoLabel_->setText(QString("Agents: %1   Friendships: %2   Time: %3:00   Day: %4")
                        .arg(agents).arg(friendships).arg(simulation_->getHour()).arg(simulation_->getDayOfWeek()));
}

void MainWindow::updateDisplay() {
    simulation_->step();
    drawGrid();
    updateStatusPanel();

    logText_->clear();
    for (const auto& agent : world_->getAgents()) {
        logText_->append(QString("Agent %1 | Age %2 | Activity %3 | Goal (%4,%5) | Friends %6")
                         .arg(agent->getId())
                         .arg(agent->getAge())
                         .arg(QString::fromStdString(domainToString(agent->getCurrentActivity())))
                         .arg(agent->getGoal().x)
                         .arg(agent->getGoal().y)
                         .arg(static_cast<int>(agent->getFriends().size())));
    }
}

void MainWindow::drawGrid() {
    scene_->clear();
    int width = world_->getWidth();
    int height = world_->getHeight();
    int tileWidth = 30;
    int tileHeight = 15;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int isoX = (x - y) * tileWidth / 2;
            int isoY = (x + y) * tileHeight / 2;

            QPolygonF polygon;
            polygon << QPointF(isoX, isoY - tileHeight / 2)
                    << QPointF(isoX + tileWidth / 2, isoY)
                    << QPointF(isoX, isoY + tileHeight / 2)
                    << QPointF(isoX - tileWidth / 2, isoY);

            QColor color = domainColor(world_->getDomain(x, y));
            if (world_->getCell(x, y) == CellType::Obstacle) {
                color = QColor(80, 80, 80);
            }

            bool boundary = false;
            if (world_->getDomain(x, y) == Domain::Home) {
                static const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                for (auto& d : dirs) {
                    int nx = x + d[0];
                    int ny = y + d[1];
                    if (!world_->isValidPosition(nx, ny) || world_->getDomain(nx, ny) != Domain::Home) {
                        boundary = true;
                        break;
                    }
                }
            }

            QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);
            item->setBrush(color);
            item->setPen(QPen(boundary ? QColor(0, 90, 230) : Qt::black, boundary ? 2 : 1));
            scene_->addItem(item);
        }
    }

    for (const auto& agent : world_->getAgents()) {
        int x = agent->getPosition().x;
        int y = agent->getPosition().y;
        int isoX = (x - y) * tileWidth / 2;
        int isoY = (x + y) * tileHeight / 2;

        QPolygonF polygon;
        polygon << QPointF(isoX, isoY - tileHeight / 2)
                << QPointF(isoX + tileWidth / 2, isoY)
                << QPointF(isoX, isoY + tileHeight / 2)
                << QPointF(isoX - tileWidth / 2, isoY);

        QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);
        item->setBrush(QColor(220, 50, 50, 200));
        item->setPen(QPen(Qt::black, 1));
        scene_->addItem(item);
    }

    scene_->setSceneRect(-width * tileWidth / 2 - tileWidth,
                         -tileHeight,
                         width * tileWidth + tileWidth * 2,
                         height * tileHeight + tileHeight * 2);
    view_->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::startSimulation() {
    simulation_->start();
}

void MainWindow::stopSimulation() {
    simulation_->stop();
}

void MainWindow::stepSimulation() {
    simulation_->step();
    updateDisplay();
}