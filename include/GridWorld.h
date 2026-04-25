#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "Agent.h"
#include "Domain.h"

enum class CellType {
    Empty,
    Obstacle,
    Agent
};

class GridWorld {
public:
    GridWorld(int width, int height);
    ~GridWorld() = default;

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    CellType getCell(int x, int y) const;
    Domain getDomain(int x, int y) const;
    void setCell(int x, int y, CellType type);
    void setDomain(int x, int y, Domain domain);
    bool isValidPosition(int x, int y) const;
    bool isObstacle(int x, int y) const;
    bool isAccessible(int x, int y, const std::vector<Domain>& allowedDomains) const;
    void addAgent(std::shared_ptr<Agent> agent);
    const std::vector<std::shared_ptr<Agent>>& getAgents() const { return agents_; }
    void update();
    Position randomPositionForDomain(Domain domain) const;
    std::vector<Position> getMeetingPoints(Domain domain, int count) const;

private:
    int width_, height_;
    std::vector<std::vector<CellType>> grid_;
    std::vector<std::vector<Domain>> domainGrid_;
    std::vector<std::shared_ptr<Agent>> agents_;
    std::unordered_map<int, std::vector<std::vector<bool>>> domainMasks_;

    void initializeDomainLayers();
    int domainMaskKey(const std::vector<Domain>& domains) const;
    std::vector<std::vector<bool>> getMaskForDomains(const std::vector<Domain>& domains);
};