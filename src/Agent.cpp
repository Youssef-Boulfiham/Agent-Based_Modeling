#include "Agent.h"
#include <algorithm>
#include <random>

Agent::Agent(int id, int age, Position start, Domain homeDomain, const std::unordered_map<Domain, double>& preferences,
             bool usesSubstances, double resistance)
    : id_(id), age_(age), position_(start), goal_(start), homeDomain_(homeDomain), currentActivity_(homeDomain),
      preferences_(preferences), usesSubstances_(usesSubstances), resistance_(resistance) {
    if (preferences_.empty()) {
        preferences_ = {{Domain::Home, 0.3}, {Domain::School, 0.2}, {Domain::Friends, 0.2}, {Domain::Leisure, 0.2}, {Domain::Public, 0.1}};
    }
}

void Agent::logMaterialUse(const std::string& material) {
    usedMaterials_.push_back(material);
}

Domain Agent::chooseActivity(int hour, int dayOfWeek) const {
    std::unordered_map<Domain, double> scores = preferences_;
    if (dayOfWeek >= 1 && dayOfWeek <= 5) {
        if (hour >= 7 && hour < 10) {
            scores[Domain::School] += 0.4;
            scores[Domain::Home] += 0.1;
        }
        if (hour >= 18 && hour < 22) {
            scores[Domain::Friends] += 0.2;
            scores[Domain::Leisure] += 0.2;
        }
    } else {
        if (hour >= 14 && hour < 22) {
            scores[Domain::Friends] += 0.3;
            scores[Domain::Leisure] += 0.3;
        }
    }

    double totalScore = 0.0;
    for (auto& entry : scores) {
        totalScore += std::max(0.0, entry.second);
    }
    if (totalScore <= 0.0) {
        return currentActivity_ == Domain::Home ? Domain::Home : Domain::Public;
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> distribution(0.0, totalScore);
    double threshold = distribution(rng);

    for (auto& entry : scores) {
        threshold -= std::max(0.0, entry.second);
        if (threshold <= 0.0) {
            return entry.first;
        }
    }

    return Domain::Home;
}

bool Agent::shouldUseSubstance() const {
    if (!usesSubstances_) {
        return false;
    }
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(rng) > resistance_;
}
