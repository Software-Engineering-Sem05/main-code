# Branch Coverage Report — Traffic Management System

Issue #31. This report measures **branch coverage** of `main.cpp`: whether every decision in the code (each `if`, loop condition, `switch` case, and each side of `&&` / `||`) has been taken both ways by the tests. It complements the line coverage in [CODE_COVERAGE_REPORT.md](CODE_COVERAGE_REPORT.md) (issue #30), which records which lines ran but not which way each decision went.

| | |
|---|---|
| Code measured | `main.cpp` on `main` at commit `5883fe5` |
| Compiler | g++ 13.3, `-std=c++14 -O0 -g --coverage` |
| Coverage tools | gcov 13.3 (data collection), gcovr 8.8 (merging results across test programs, branch and decision reports) |

## 1. Summary

Results for all test programs combined:

| Metric | Covered | Total | Coverage |
|---|---:|---:|---:|
| **Decision coverage** (source-level decision outcomes) | 62 | 66 | **93.9%** |
| **Branch coverage** (gcov branches, excluding exception paths) | 199 | 227 | **87.7%** |
| Line coverage | 224 | 230 | 97.4% |
| Function coverage | 23 | 23 | 100% |

- **Every decision a user or caller can actually reach is covered in both directions.** The 4 decision outcomes not taken are in two defensive checks that the program's own design prevents from triggering (section 5.2).
- The remaining untaken *branches* are those two checks plus compiler-generated clean-up branches for temporary strings, which are not decisions in our source code (section 5.3).
- While measuring, 5 real gaps were found (decisions that had only been taken one way). New tests were added for all of them in `tests/test_branch_coverage.cpp` (section 4).

## 2. Method

1. Each test program was compiled with coverage instrumentation:
   ```sh
   g++ -std=c++14 -O0 -g --coverage tests/test_queue_manager.cpp   -o build/test_queue_manager
   g++ -std=c++14 -O0 -g --coverage tests/test_vehicle_manager.cpp -o build/test_vehicle_manager
   g++ -std=c++14 -O0 -g --coverage tests/test_plan_cases.cpp      -o build/test_plan_cases
   g++ -std=c++14 -O0 -g --coverage tests/test_performance.cpp     -o build/test_performance
   g++ -std=c++14 -O0 -g --coverage tests/test_branch_coverage.cpp -o build/test_branch_coverage
   g++ -std=c++14 -O0 -g --coverage main.cpp                       -o build/tms
   ```
2. Every program was run once. All test programs passed. The simulation itself (`build/tms`) was run with a short scripted session (add a vehicle, view status, advance, exit) so that `main()` is also measured.
3. gcovr merged the coverage data from all six runs and reported branch and decision coverage for `main.cpp`:
   ```sh
   gcovr -r . --object-directory build build --filter "main\.cpp" \
         --exclude-throw-branches --exclude-unreachable-branches \
         --decisions --txt-metric branch --txt-summary
   ```
4. The same command with `--html-details docs/branch-coverage/index.html` produced the line-by-line report in [`docs/branch-coverage/`](branch-coverage/).

**Why exception branches are excluded:** in C++, gcov also counts hidden branches that the compiler adds for exceptions, such as a `std::string` failing to allocate memory. No normal test can trigger them, and they are not decisions in our code. Without the exclusion gcov counts 345 branches (207 taken, 60%), but most of the difference is these exception paths. Excluding them is standard practice and is what the flags `--exclude-throw-branches --exclude-unreachable-branches` do.

## 3. Results per test program

Each row shows the coverage of `main.cpp` reached by that program on its own. The unit-test programs only use some classes, so their totals count only the code compiled into them. The **combined** row is the project-wide result.

| Test program | Decisions | Branches | Lines |
|---|---:|---:|---:|
| `test_queue_manager` | 7 / 8 (87%) | 24 / 26 (92%) | 29 / 30 (96%) |
| `test_vehicle_manager` | 13 / 14 (92%) | 38 / 50 (76%) | 42 / 43 (97%) |
| `test_plan_cases` (TC-01 to TC-12) | 53 / 64 (82%) | 176 / 219 (80%) | 206 / 219 (94%) |
| `test_performance` | 45 / 64 (70%) | 156 / 219 (71%) | 192 / 219 (87%) |
| `test_branch_coverage` (new) | 25 / 66 (37%) | 82 / 225 (36%) | 88 / 224 (39%) |
| `tms` (scripted run of the program) | 36 / 64 (56%) | 134 / 221 (60%) | 174 / 224 (77%) |
| **Combined, before new tests** | 58 / 66 (87.9%) | 194 / 227 (85.5%) | 220 / 230 (95.7%) |
| **Combined, final** | **62 / 66 (93.9%)** | **199 / 227 (87.7%)** | **224 / 230 (97.4%)** |

## 4. Gaps found and closed

The first combined measurement showed 5 decisions in reachable code that had been taken in only one direction. Tests were added for each in `tests/test_branch_coverage.cpp` (14 checks, all passing):

| Line | Decision | Direction not tested before | New test |
|---:|---|---|---|
| 70 | `QueueManager::peekFront`: `if (!isValidLane(laneId) \|\| ...)` | invalid lane | `peekFront` on lane 4 is refused and leaves the output untouched |
| 90 | `QueueManager::snapshot`: `if (!isValidLane(laneId))` | invalid lane | `snapshot` of lane 4 is empty |
| 266 | `readInteger`: `if (!std::getline(std::cin, input))` | input ends at a number prompt | input ending at the menu or lane prompt exits cleanly |
| 300 | `addVehicle`: `if (!std::getline(std::cin, id))` | input ends at the vehicle ID prompt | nothing is added; program exits cleanly |
| 320 | `addVehicle`: `if (!std::getline(std::cin, type))` | input ends at the vehicle type prompt | nothing is added; program exits cleanly |

"Input ends" means end-of-file on standard input, for example Ctrl+Z on Windows, Ctrl+D on Linux, or a script that stops early. The program handled all of these correctly; they simply had not been tested. The new file also tests the `trimmed()` helper added for defect D-01 in the [test report](../TEST_REPORT.md).

## 5. Decision coverage in detail

### 5.1 Covered decisions

Every decision below was taken **both ways** (or, for `switch` cases, entered) during testing.

| Line | Function | Decision |
|---:|---|---|
| 44 | `QueueManager::enqueue` | invalid lane, empty ID or already-used ID → reject |
| 58 | `QueueManager::dequeue` | invalid lane or empty queue → refuse |
| 70 | `QueueManager::peekFront` | invalid lane or empty queue → refuse |
| 90 | `QueueManager::snapshot` | invalid lane → empty result |
| 114 | `trimmed` | text is only whitespace |
| 153 | `VehicleManager::addVehicle` | blank vehicle ID or type → reject |
| 169 | `VehicleManager::moveVehicle` | signal is not GREEN → blocked |
| 173 | `VehicleManager::moveVehicle` | lane queue empty → nothing to move |
| 201 | `TrafficSimulation::run` | menu loop continues / ends |
| 206–218 | `TrafficSimulation::run` | menu options 1, 2, 3, 4 and invalid option (all 5 cases) |
| 229–233 | `signalStateName` | RED, YELLOW, GREEN (all 3 cases) |
| 266 | `readInteger` | input ended |
| 274 | `readInteger` | input is a whole number |
| 285 | `TrafficSimulation::addVehicle` | lane list loop |
| 291 | `TrafficSimulation::addVehicle` | lane number out of range |
| 300 | `TrafficSimulation::addVehicle` | input ended at vehicle ID |
| 306 | `TrafficSimulation::addVehicle` | vehicle ID empty |
| 311 | `TrafficSimulation::addVehicle` | vehicle ID already used |
| 320 | `TrafficSimulation::addVehicle` | input ended at vehicle type |
| 326 | `TrafficSimulation::addVehicle` | vehicle type empty |
| 353 | `TrafficSimulation::showStatus` | lane loop |
| 367 | `TrafficSimulation::showStatus` | queue length ≤ 2 → LOW |
| 369 | `TrafficSimulation::showStatus` | queue length ≤ 5 → MEDIUM, else HIGH |
| 380 | `TrafficSimulation::showStatus` | queue empty |
| 386 | `TrafficSimulation::showStatus` | first vehicle in queue listing |
| 423 | `TrafficSimulation::advanceSimulation` | lane is YELLOW → switch to next lane |
| 452 | `TrafficSimulation::advanceSimulation` | GREEN lane has no vehicles |
| 476 | `TrafficSimulation::advanceSimulation` | GREEN time used up → YELLOW |

### 5.2 Decision outcomes not covered (defensive code)

| Line | Decision | Outcome never taken | Why it cannot happen |
|---:|---|---|---|
| 334 | `TrafficSimulation::addVehicle`: `if (!vehicles_.addVehicle(...))` | Vehicle Manager rejects the vehicle | Before this call, the menu has already rejected every input the Vehicle Manager would refuse: invalid lane, empty or blank ID or type, and already-used ID. This is a second safety check (defence in depth, SADS §6 / SR2). The Vehicle Manager's own rejection logic is fully covered by `test_vehicle_manager`. |
| 448 | `TrafficSimulation::advanceSimulation`: `if (result == MoveResult::BlockedBySignal)` | Vehicle Manager blocks the move because the signal is not GREEN | `advanceSimulation` only asks to move a vehicle from the lane that is currently GREEN, and returns early while that lane is YELLOW (line 423). So the exit rule never blocks here. The exit rule itself (RED/YELLOW blocks, GREEN allows) is fully covered at line 169 by `test_vehicle_manager` and system test TC-04. |

Neither gap is a missing test. Triggering these outcomes would mean bypassing the checks that exist to prevent them. They are kept as a safety net in case the calling code changes later.

### 5.3 Branches not covered (compiler-generated)

The other untaken branches on lines 153, 157, 187 and 228 are not decisions in the source code:

- **Lines 153, 157, 187:** clean-up paths the compiler adds when building temporary `std::string` and `Vehicle` objects (in `trimmed(...)`, `Vehicle{...}` and the lane names `"North"`, `"South"`, ...). They only run if memory allocation fails part-way through.
- **Line 228:** the implicit "no case matched" exit from `switch (state)` in `signalStateName`. `SignalState` only has the values RED, YELLOW and GREEN, and all three cases are covered, so the fallback `return "UNKNOWN";` can never run.

## 6. Coverage by function

Branch coverage (exception paths excluded) per function in `main.cpp`:

| Function | Branches | Coverage | Notes |
|---|---:|---:|---|
| `QueueManager::QueueManager` | 1 / 1 | 100% | |
| `QueueManager::enqueue` | 8 / 8 | 100% | |
| `QueueManager::dequeue` | 6 / 6 | 100% | |
| `QueueManager::peekFront` | 6 / 6 | 100% | gap closed by new test |
| `QueueManager::getQueueLength` | 2 / 2 | 100% | |
| `QueueManager::snapshot` | 3 / 3 | 100% | gap closed by new test |
| `trimmed` | 3 / 3 | 100% | |
| `VehicleManager::addVehicle` | 14 / 23 | 60.9% | all decisions covered; rest are temporary-string clean-up (5.3) |
| `VehicleManager::moveVehicle` | 4 / 4 | 100% | |
| `TrafficSimulation::TrafficSimulation` | 5 / 15 | 33.3% | no decisions; lane-name string clean-up only (5.3) |
| `TrafficSimulation::run` | 9 / 9 | 100% | |
| `TrafficSimulation::signalStateName` | 3 / 4 | 75.0% | impossible `switch` fallback (5.3) |
| `TrafficSimulation::readInteger` | 17 / 17 | 100% | gap closed by new test |
| `TrafficSimulation::addVehicle` | 45 / 47 | 95.7% | defensive check on line 334 (5.2) |
| `TrafficSimulation::showStatus` | 40 / 40 | 100% | |
| `TrafficSimulation::advanceSimulation` | 31 / 37 | 83.8% | defensive check on line 448 (5.2) |
| `main` | 2 / 2 | 100% | |

## 7. Report files

| File | Contents |
|---|---|
| `docs/BRANCH_COVERAGE_REPORT.md` | This report |
| `docs/branch-coverage/index.html` | Line-by-line HTML report: open in a browser and click `main.cpp`. Each line shows its branches and decisions (✓ taken / ✗ not taken). |
| `docs/branch-coverage/index.functions.html` | Function list with call counts |
| `tests/test_branch_coverage.cpp` | New tests that close the 5 gaps in section 4 |

## 8. How to reproduce

From the repository root. gcovr is installed with `pip install gcovr`. On Windows, use the `.exe` names and `\` paths.

```sh
mkdir build
g++ -std=c++14 -O0 -g --coverage tests/test_queue_manager.cpp   -o build/test_queue_manager
g++ -std=c++14 -O0 -g --coverage tests/test_vehicle_manager.cpp -o build/test_vehicle_manager
g++ -std=c++14 -O0 -g --coverage tests/test_plan_cases.cpp      -o build/test_plan_cases
g++ -std=c++14 -O0 -g --coverage tests/test_performance.cpp     -o build/test_performance
g++ -std=c++14 -O0 -g --coverage tests/test_branch_coverage.cpp -o build/test_branch_coverage
g++ -std=c++14 -O0 -g --coverage main.cpp                       -o build/tms

cd build
./test_queue_manager && ./test_vehicle_manager && ./test_plan_cases && ./test_performance && ./test_branch_coverage
printf '1\n1\nKA01\ncar\n2\n3\n4\n' | ./tms
cd ..

gcovr -r . --object-directory build build --filter "main\.cpp" --exclude-throw-branches --exclude-unreachable-branches --decisions --txt-metric branch --txt-summary
```

Without gcovr, a single program's branch coverage can be seen with plain gcov: run the program, then `gcov -b -c <name>.gcno` in the build folder. Plain gcov cannot merge several programs, and it includes the exception branches described in section 2.

## 9. Conclusion

Across all test programs, `main.cpp` reaches **93.9% decision coverage** and **87.7% branch coverage**, with 100% of functions and 97.4% of lines executed. Every decision that normal use of the program can reach is tested in both directions. The only uncovered decision outcomes are two defensive checks that the design makes impossible to trigger, and the remaining uncovered branches are compiler-generated exception clean-up. Measuring branch coverage found five untested directions, and they are now covered by `tests/test_branch_coverage.cpp`.
