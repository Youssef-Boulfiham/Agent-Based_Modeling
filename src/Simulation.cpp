#include "Simulation.h"
#include <chrono>
#include <thread>
#include <algorithm>

Simulation::Simulation(std::shared_ptr<GridWorld> world)
    : world_(world), pathfinder_(std::make_shared<Pathfinder>(world)), running_(false), speed_(100), currentHour_(8), currentDayOfWeek_(1) {
}

Simulation::~Simulation() {
    stop();
}

void Simulation::start() {
    if (running_) return;
    running_ = true;
    simulationThread_ = std::thread(&Simulation::simulationLoop, this);
}

void Simulation::stop() {
    running_ = false;
    if (simulationThread_.joinable()) {
        simulationThread_.join();
    }
}

void Simulation::step() {
    updateAgents();
    evaluateFriendships();
    simulateSubstanceUse();
    world_->update();

    currentHour_ = (currentHour_ + 1) % 24;
    if (currentHour_ == 0) {
        currentDayOfWeek_ = currentDayOfWeek_ % 7 + 1;
    }
}

void Simulation::simulationLoop() {
    while (running_) {
        std::lock_guard<std::mutex> lock(mutex_);
        step();
        std::this_thread::sleep_for(std::chrono::milliseconds(speed_));
    }
}

Position Simulation::chooseGoalForAgent(std::shared_ptr<Agent> agent) {
    Domain activity = agent->getCurrentActivity();
    std::vector<Domain> allowed = {Domain::Path, activity, Domain::Public};
    if (activity == Domain::Home) {
        return world_->randomPositionForDomain(Domain::Home);
    }
    Position goal = world_->randomPositionForDomain(activity);
    return goal;
}

void Simulation::updateAgents() {
    for (auto& agent : world_->getAgents()) {
        if (agent->hasReachedGoal() || agent->getPath().empty()) {
            Domain nextActivity = agent->chooseActivity(currentHour_, currentDayOfWeek_);
            agent->setCurrentActivity(nextActivity);
            Position goal = chooseGoalForAgent(agent);
            agent->setGoal(goal);
            auto allowed = std::vector<Domain>{Domain::Path, nextActivity, Domain::Public};
            auto path = pathfinder_->findPath(agent->getPosition(), goal, allowed);
            if (!path.empty()) {
                agent->setPath(path);
            }
        }
    }
}

void Simulation::assignMeetingPoints() {
    // Meeting points are currently implicit in agent co-location within public and friends domains.
}

void Simulation::evaluateFriendships() {
    const auto& agents = world_->getAgents();
    for (size_t i = 0; i < agents.size(); ++i) {
        for (size_t j = i + 1; j < agents.size(); ++j) {
            auto& a = agents[i];
            auto& b = agents[j];
            if (a->getCurrentActivity() == Domain::Home || b->getCurrentActivity() == Domain::Home) {
                continue;
            }
            if (a->getPosition() == b->getPosition() && !a->isFriendWith(b->getId())) {
                a->addFriend(b->getId());
                b->addFriend(a->getId());
            }
        }
    }
}

void Simulation::simulateSubstanceUse() {
    for (auto& agent : world_->getAgents()) {
        if ((agent->getCurrentActivity() == Domain::Leisure || agent->getCurrentActivity() == Domain::Friends) && agent->shouldUseSubstance()) {
            agent->logMaterialUse("substance");
        }
    }
}
