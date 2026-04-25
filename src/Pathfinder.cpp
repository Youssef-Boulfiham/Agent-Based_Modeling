#include "Pathfinder.h"
#include <algorithm>
#include <cmath>

Pathfinder::Pathfinder(std::shared_ptr<GridWorld> world)
    : world_(world) {
}

std::vector<Position> Pathfinder::findPath(Position start, Position goal, const std::vector<Domain>& allowedDomains) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_map<int, Node> openMap;
    std::unordered_map<int, Node> closedMap;

    auto posToKey = [](Position p) { return p.x * 10000 + p.y; };

    openSet.emplace(start, 0, heuristic(start, goal));
    openMap.insert({posToKey(start), Node(start, 0, heuristic(start, goal))});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        closedMap.insert({posToKey(current.pos), current});

        if (current.pos == goal) {
            // Reconstruct path
            std::vector<Position> path;
            Position pos = goal;
            while (pos.x != -1) {
                path.push_back(pos);
                auto it = closedMap.find(posToKey(pos));
                if (it != closedMap.end()) {
                    pos = it->second.parent;
                } else {
                    break;
                }
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Neighbors
        std::vector<Position> neighbors = {
            {current.pos.x + 1, current.pos.y},
            {current.pos.x - 1, current.pos.y},
            {current.pos.x, current.pos.y + 1},
            {current.pos.x, current.pos.y - 1}
        };

        for (auto& neighbor : neighbors) {
            if (!world_->isAccessible(neighbor.x, neighbor.y, allowedDomains)) {
                continue;
            }

            int tentativeGCost = current.gCost + 1;
            int key = posToKey(neighbor);

            if (closedMap.count(key)) continue;

            auto it = openMap.find(key);
            if (it == openMap.end() || tentativeGCost < it->second.gCost) {
                Node neighborNode(neighbor, tentativeGCost, heuristic(neighbor, goal), current.pos);
                openSet.push(neighborNode);
                openMap.insert({key, neighborNode});
            }
        }
    }

    return {}; // No path found
}

int Pathfinder::heuristic(Position a, Position b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Manhattan distance
}