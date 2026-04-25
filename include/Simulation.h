#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include "GridWorld.h"
#include "Pathfinder.h"

class Simulation {
public:
    Simulation(std::shared_ptr<GridWorld> world);
    ~Simulation();

    void start();
    void stop();
    void step();
    bool isRunning() const { return running_; }
    void setSpeed(int speed) { speed_ = speed; }
    int getHour() const { return currentHour_; }
    int getDayOfWeek() const { return currentDayOfWeek_; }

private:
    void simulationLoop();
    void updateAgents();
    void assignMeetingPoints();
    void evaluateFriendships();
    void simulateSubstanceUse();
    Position chooseGoalForAgent(std::shared_ptr<Agent> agent);

    std::shared_ptr<GridWorld> world_;
    std::shared_ptr<Pathfinder> pathfinder_;
    std::thread simulationThread_;
    std::mutex mutex_;
    std::atomic<bool> running_;
    int speed_; // milliseconds between steps
    int currentHour_;
    int currentDayOfWeek_;
};