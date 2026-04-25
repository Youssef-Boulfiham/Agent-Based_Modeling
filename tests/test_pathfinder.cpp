#include <catch2/catch_test_macros.hpp>
#include "GridWorld.h"
#include "Pathfinder.h"

TEST_CASE("Pathfinder finds path in empty grid", "[pathfinder]") {
    auto world = std::make_shared<GridWorld>(10, 10);
    Pathfinder pathfinder(world);

    Position start(0, 0);
    Position goal(9, 9);
    auto path = pathfinder.findPath(start, goal);

    REQUIRE(!path.empty());
    REQUIRE(path.front() == start);
    REQUIRE(path.back() == goal);
}

TEST_CASE("Pathfinder avoids obstacles", "[pathfinder]") {
    auto world = std::make_shared<GridWorld>(5, 5);
    world->setCell(2, 1, CellType::Obstacle);
    world->setCell(2, 2, CellType::Obstacle);
    world->setCell(2, 3, CellType::Obstacle);
    Pathfinder pathfinder(world);

    Position start(0, 0);
    Position goal(4, 4);
    auto path = pathfinder.findPath(start, goal);

    REQUIRE(!path.empty());
    for (auto& pos : path) {
        REQUIRE(world->getCell(pos.x, pos.y) != CellType::Obstacle);
    }
}