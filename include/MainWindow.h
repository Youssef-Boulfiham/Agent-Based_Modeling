#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QTextEdit>
#include <memory>
#include "GridWorld.h"
#include "Simulation.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateDisplay();
    void startSimulation();
    void stopSimulation();
    void stepSimulation();

private:
    void setupUI();
    void setupMenus();
    void drawGrid();
    void updateStatusPanel();

    std::shared_ptr<GridWorld> world_;
    std::shared_ptr<Simulation> simulation_;
    QTimer* updateTimer_;
    QGraphicsView* view_;
    QGraphicsScene* scene_;
    QLabel* infoLabel_;
    QTextEdit* logText_;
};