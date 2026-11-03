# Code Coverage Report — Traffic Management System

## 1. Objective

Measure source-code execution during the automated tests and show how the
current implementation is validated against the Traffic Management System
SRS.

## 2. Tools and execution

- **Language:** C++
- **Environment:** macOS
- **Compiler:** Apple Clang 21.0.0, invoked as `g++`
- **Coverage tool:** LLVM gcov
- **Instrumentation:** `-fprofile-arcs -ftest-coverage -O0 -g`
- **Measurement date:** 3 November 2026

All five test programs were compiled with coverage instrumentation, executed,
and exited successfully. Coverage was measured separately for each test
program. The `main.cpp` percentages below therefore describe only the part of
that file reached by the corresponding test program; they are not an
aggregated project-wide percentage.

## 3. Coverage results

| Test suite | Test file coverage | `main.cpp` coverage |
|---|---:|---:|
| Queue Manager | 96.97% (96 of 99 lines) | 97.73% (43 of 44 lines) |
| Vehicle Manager | 97.89% (93 of 95 lines) | 98.04% (50 of 51 lines) |
| System Test Plan | 96.90% (407 of 420 lines) | 94.78% (236 of 249 lines) |
| Performance and Workload | 96.40% (107 of 111 lines) | 88.76% (221 of 249 lines) |
| Branch Coverage | 96.88% (62 of 64 lines) | 42.75% (109 of 255 lines) |

The branch-coverage test is intentionally focused on edge paths, so its
individual `main.cpp` percentage is not representative of the whole test
suite.

## 4. SRS traceability

| SRS requirement | Implementation and validation |
|---|---|
| FR1, SR1 — add vehicles and validate input | The interactive menu and manager APIs reject invalid lanes, empty or whitespace-only IDs and types, and duplicate IDs. Queue Manager tests include direct insertion attempts that bypass the menu. |
| FR2, FR5 — lane queues and rule-based movement | Independent FIFO queues are covered by Queue Manager, Vehicle Manager, and TC-02 tests. TC-04 and the vehicle tests verify movement is blocked on RED/YELLOW and allowed on GREEN. |
| FR3, FR4 — signal states and cycle | TC-03 checks the GREEN → YELLOW → RED cycle and lane order; TC-04 checks movement against the active signal. |
| FR6, NFR2 — display traffic state | TC-05 verifies lane names, signal states, queue contents, counts, and statistics are displayed. |
| FR7 — detect congestion | TC-06 and TC-07 verify queue lengths at, below, and above the threshold; more than five vehicles is reported as HIGH. |
| FR8 — traffic statistics | TC-08 checks added/processed totals, queue lengths, individual waiting times, and average waiting time. |
| NFR1 — performance | TC-09 and the performance suite exercise 5,000, 20,000, and 200,000 vehicle workloads without a specified response-time threshold. |
| NFR3, SR2 — queue integrity | Queue and system tests verify FIFO order, duplicate prevention, empty/invalid-operation safeguards, and consistency between processed and queued vehicles. |

## 5. Test suites and observed results

| Test suite | Coverage |
|---|---|
| `tests/test_queue_manager.cpp` | FIFO behavior, independent lane queues, invalid lanes, empty queues, duplicate IDs, and blank vehicle data |
| `tests/test_vehicle_manager.cpp` | Vehicle validation, signal-controlled movement, FIFO exits, and empty/invalid lanes |
| `tests/test_plan_cases.cpp` | End-to-end test cases TC-01 through TC-12 |
| `tests/test_performance.cpp` | 200,000-vehicle component workload and 20,000-vehicle simulation workload |
| `tests/test_branch_coverage.cpp` | Invalid lane queries, end-of-input handling, and whitespace trimming |

All five suites passed in the validation run. TC-01 through TC-12 passed, and
the performance suite processed the complete configured workloads.

## 6. Reproducing coverage

Compile each test program with the instrumentation flags used for the report,
run it, and pass the generated coverage data to `gcov`. For example:

```sh
g++ -std=c++17 -fprofile-arcs -ftest-coverage -O0 -g \
  tests/test_queue_manager.cpp -o test_queue_manager
./test_queue_manager
gcov --gcno=test_queue_manager-test_queue_manager.gcno \
  --gcda=test_queue_manager-test_queue_manager.gcda tests/test_queue_manager.cpp
```

Use the corresponding test source for the other suites. The System Test Plan
and Branch Coverage programs also compile successfully as C++14.

## 7. Report artifacts

The checked-in `.gcov` listings under `docs/` are retained from an earlier
coverage run and are historical snapshots. The percentages in this report
come from the 3 November 2026 run described above.

## 8. Conclusion

The implementation was validated against all functional, non-functional, and
security requirements stated in the SRS. The test suites passed, including
input validation at the queue API boundary, signal-cycle and congestion
scenarios, statistics checks, and representative high-volume workloads.
