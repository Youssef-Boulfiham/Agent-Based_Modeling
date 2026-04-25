#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include "Domain.h"

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

class Agent {
public:
    Agent(int id, int age, Position start, Domain homeDomain, const std::unordered_map<Domain, double>& preferences,
          bool usesSubstances, double resistance);
    ~Agent() = default;

    int getId() const { return id_; }
    int getAge() const { return age_; }
    Position getPosition() const { return position_; }
    Position getGoal() const { return goal_; }
    Domain getHomeDomain() const { return homeDomain_; }
    Domain getCurrentActivity() const { return currentActivity_; }
    const std::vector<Position>& getPath() const { return path_; }
    const std::vector<std::string>& getUsedMaterials() const { return usedMaterials_; }
    const std::unordered_set<int>& getFriends() const { return friends_; }
    bool usesSubstances() const { return usesSubstances_; }
    double getResistance() const { return resistance_; }

    void setPath(const std::vector<Position>& path) { path_ = path; }
    void moveTo(Position pos) { position_ = pos; }
    void setGoal(Position goal) { goal_ = goal; }
    void setCurrentActivity(Domain activity) { currentActivity_ = activity; }
    bool hasReachedGoal() const { return position_ == goal_; }
    bool isFriendWith(int otherId) const { return friends_.count(otherId) > 0; }
    void addFriend(int otherId) { friends_.insert(otherId); }
    void logMaterialUse(const std::string& material);

    Domain chooseActivity(int hour, int dayOfWeek) const;
    bool shouldUseSubstance() const;

private:
    int id_;
    int age_;
    Position position_;
    Position goal_;
    Domain homeDomain_;
    Domain currentActivity_;
    std::vector<Position> path_;
    std::unordered_map<Domain, double> preferences_;
    std::unordered_set<int> friends_;
    std::vector<std::string> usedMaterials_;
    bool usesSubstances_;
    double resistance_;
};