#include "GridWorld.h"
#include <algorithm>
#include <random>

GridWorld::GridWorld(int width, int height)
    : width_(width), height_(height), grid_(height, std::vector<CellType>(width, CellType::Empty)),
      domainGrid_(height, std::vector<Domain>(width, Domain::Unknown)) {
    initializeDomainLayers();
}

CellType GridWorld::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) return CellType::Obstacle;
    return grid_[y][x];
}

Domain GridWorld::getDomain(int x, int y) const {
    if (!isValidPosition(x, y)) return Domain::Unknown;
    return domainGrid_[y][x];
}

void GridWorld::setCell(int x, int y, CellType type) {
    if (isValidPosition(x, y)) {
        grid_[y][x] = type;
    }
}

void GridWorld::setDomain(int x, int y, Domain domain) {
    if (isValidPosition(x, y)) {
        domainGrid_[y][x] = domain;
    }
}

bool GridWorld::isValidPosition(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool GridWorld::isObstacle(int x, int y) const {
    return getCell(x, y) == CellType::Obstacle;
}

bool GridWorld::isAccessible(int x, int y, const std::vector<Domain>& allowedDomains) const {
    if (!isValidPosition(x, y) || isObstacle(x, y)) {
        return false;
    }
    if (allowedDomains.empty()) {
        return true;
    }
    Domain domain = getDomain(x, y);
    for (auto allowed : allowedDomains) {
        if (allowed == domain) {
            return true;
        }
    }
    return false;
}

void GridWorld::addAgent(std::shared_ptr<Agent> agent) {
    agents_.push_back(agent);
    setCell(agent->getPosition().x, agent->getPosition().y, CellType::Agent);
}

std::vector<std::vector<bool>> GridWorld::getMaskForDomains(const std::vector<Domain>& domains) {
    int key = domainMaskKey(domains);
    auto it = domainMasks_.find(key);
    if (it != domainMasks_.end()) {
        return it->second;
    }

    std::vector<std::vector<bool>> mask(height_, std::vector<bool>(width_, false));
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            mask[y][x] = isAccessible(x, y, domains);
        }
    }
    domainMasks_.emplace(key, mask);
    return mask;
}

int GridWorld::domainMaskKey(const std::vector<Domain>& domains) const {
    int key = 0;
    for (auto domain : domains) {
        key |= (1 << static_cast<int>(domain));
    }
    return key;
}

void GridWorld::initializeDomainLayers() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            domainGrid_[y][x] = Domain::Public;
            grid_[y][x] = CellType::Empty;
        }
    }

    int roomX1 = 4;
    int roomY1 = 2;
    int roomX2 = 11;
    int roomY2 = 7;
    for (int y = roomY1; y <= roomY2; ++y) {
        for (int x = roomX1; x <= roomX2; ++x) {
            domainGrid_[y][x] = Domain::Home;
        }
    }

    int pathStartX = 8;
    for (int y = roomY2 + 1; y <= roomY2 + 5; ++y) {
        domainGrid_[y][pathStartX] = Domain::Path;
    }
    for (int x = pathStartX - 1; x <= pathStartX + 1; ++x) {
        domainGrid_[roomY2 + 2][x] = Domain::Path;
    }

    int greenX1 = 4;
    int greenY1 = 12;
    int greenX2 = 12;
    int greenY2 = 18;
    for (int y = greenY1; y <= greenY2; ++y) {
        for (int x = greenX1; x <= greenX2; ++x) {
            domainGrid_[y][x] = Domain::Leisure;
        }
    }
}

Position GridWorld::randomPositionForDomain(Domain domain) const {
    std::vector<Position> candidates;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (getDomain(x, y) == domain && !isObstacle(x, y)) {
                candidates.emplace_back(x, y);
            }
        }
    }
    if (candidates.empty()) {
        return Position(0, 0);
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distribution(0, static_cast<int>(candidates.size() - 1));
    return candidates[distribution(rng)];
}

std::vector<Position> GridWorld::getMeetingPoints(Domain domain, int count) const {
    std::vector<Position> points;
    for (int x = 0; x < width_; ++x) {
        for (int y = 0; y < height_; ++y) {
            if (getDomain(x, y) == domain && !isObstacle(x, y)) {
                points.emplace_back(x, y);
            }
        }
    }
    if (points.size() <= static_cast<size_t>(count)) {
        return points;
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(points.begin(), points.end(), rng);
    points.resize(count);
    return points;
}

void GridWorld::update() {
    for (auto& agent : agents_) {
        setCell(agent->getPosition().x, agent->getPosition().y, CellType::Empty);
    }
    for (auto& agent : agents_) {
        if (!agent->getPath().empty()) {
            agent->moveTo(agent->getPath().front());
            agent->setPath(std::vector<Position>(agent->getPath().begin() + 1, agent->getPath().end()));
        }
        setCell(agent->getPosition().x, agent->getPosition().y, CellType::Agent);
    }
}
