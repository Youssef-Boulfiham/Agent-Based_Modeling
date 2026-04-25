add_executable(tests test_agent.cpp test_pathfinder.cpp)
target_link_libraries(tests Catch2::Catch2)
include(Catch)
catch_discover_tests(tests)