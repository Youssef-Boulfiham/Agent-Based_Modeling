#include <catch2/catch_test_macros.hpp>
#include "Agent.h"

TEST_CASE("Agent creation and basic functionality", "[agent]") {
    Position start(0, 0);
    Position goal(5, 5);
    Agent agent(1, start, goal);

    REQUIRE(agent.getId() == 1);
    REQUIRE(agent.getPosition() == start);
    REQUIRE(agent.getGoal() == goal);
    REQUIRE(!agent.hasReachedGoal());

    agent.moveTo(goal);
    REQUIRE(agent.hasReachedGoal());
}