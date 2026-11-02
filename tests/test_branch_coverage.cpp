// Extra tests for branches that the other suites did not take (issue #31).
// See docs/BRANCH_COVERAGE_REPORT.md for how these gaps were found.
//
// Build and run from the repo root:
//   g++ -std=c++14 -Wall -Wextra tests/test_branch_coverage.cpp -o test_branch_coverage.exe
//   .\test_branch_coverage.exe        (Windows)
//   ./test_branch_coverage.exe        (Linux / macOS)

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Pull in the classes from main.cpp without its main() function.
#define TMS_UNIT_TEST
#include "../main.cpp"

namespace {

int failures = 0;

void check(bool condition, const std::string& description) {
    std::cout << (condition ? "  PASS  " : "  FAIL  ") << description << '\n';

    if (!condition) {
        ++failures;
    }
}

bool contains(const std::string& text, const std::string& part) {
    return text.find(part) != std::string::npos;
}

// Runs the simulation with exactly this input (no automatic "4" at the end),
// so the input can run out part-way through a command.
std::string runUntilInputEnds(const std::string& script) {
    std::istringstream input(script);
    std::ostringstream output;

    std::streambuf* const originalIn = std::cin.rdbuf(input.rdbuf());
    std::streambuf* const originalOut = std::cout.rdbuf(output.rdbuf());

    {
        TrafficSimulation simulation;
        simulation.run();
    }

    std::cin.rdbuf(originalIn);
    std::cout.rdbuf(originalOut);
    return output.str();
}

// QueueManager::peekFront and QueueManager::snapshot with an invalid lane.
void testInvalidLaneQueries() {
    std::cout << "Queue Manager queries on an invalid lane\n";
    QueueManager queues(4);
    queues.enqueue(0, Vehicle{"V1", "car", 0});

    Vehicle out{"unchanged", "unchanged", 99};
    check(!queues.peekFront(4, out), "peekFront on lane 4 is refused");
    check(out.id == "unchanged", "refused peekFront leaves the output untouched");
    check(queues.snapshot(4).empty(), "snapshot of lane 4 is empty");
    check(queues.snapshot(0).size() == 1, "valid lane snapshot is unaffected");
}

// Input ends (end-of-file / Ctrl+Z / Ctrl+D) at each prompt.
void testInputEnds() {
    std::cout << "Input ends at each prompt\n";

    const std::string atMenu = runUntilInputEnds("");
    check(contains(atMenu, "Simulation ended."),
          "input ending at the menu exits cleanly");

    const std::string atLane = runUntilInputEnds("1\n");
    check(contains(atLane, "Simulation ended.") && !contains(atLane, "added to the"),
          "input ending at the lane prompt adds nothing and exits");

    const std::string atId = runUntilInputEnds("1\n2\n");
    check(contains(atId, "Simulation ended.") && !contains(atId, "added to the"),
          "input ending at the vehicle ID prompt adds nothing and exits");

    const std::string atType = runUntilInputEnds("1\n2\nKA01\n");
    check(contains(atType, "Simulation ended.") && !contains(atType, "added to the"),
          "input ending at the vehicle type prompt adds nothing and exits");

    const std::string afterValid = runUntilInputEnds("1\n2\nKA01\ncar\n");
    check(contains(afterValid, "Vehicle KA01 added to the South lane queue.") &&
              contains(afterValid, "Simulation ended."),
          "a completed add followed by end of input exits cleanly");
}

// trimmed() with surrounding whitespace of every kind it removes.
void testTrimmed() {
    std::cout << "trimmed() helper\n";
    check(trimmed("  KA01  ") == "KA01", "spaces around text are removed");
    check(trimmed("\tKA01\r\n") == "KA01", "tabs and line endings are removed");
    check(trimmed("K A") == "K A", "spaces inside text are kept");
    check(trimmed(" \t\r\n").empty(), "whitespace-only text becomes empty");
    check(trimmed("").empty(), "empty text stays empty");
}

} // namespace

int main() {
    testInvalidLaneQueries();
    testInputEnds();
    testTrimmed();

    std::cout << '\n'
              << (failures == 0 ? "All tests passed.\n" : "Some tests failed.\n");
    return failures == 0 ? 0 : 1;
}
