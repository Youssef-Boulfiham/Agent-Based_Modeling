#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include "GridWorld.h"
#include "Domain.h"

struct Node {
    Position pos;
    int gCost, hCost, fCost;
    Position parent;

    Node(Position p, int g, int h, Position par = {-1, -1})
        : pos(p), gCost(g), hCost(h), fCost(g + h), parent(par) {}

    bool operator>(const Node& other) const { return fCost > other.fCost; }
};

class Pathfinder {
public:
    Pathfinder(std::shared_ptr<GridWorld> world);
    ~Pathfinder() = default;

    std::vector<Position> findPath(Position start, Position goal, const std::vector<Domain>& allowedDomains);
    int heuristic(Position a, Position b) const;

private:
    std::shared_ptr<GridWorld> world_;
};