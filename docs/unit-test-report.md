# Unit Test Report
## Traffic Management System Simulation

**Report date:** 2 November 2026  
**Project:** Traffic Management System Simulation  
**Test environment:** Windows Command Prompt, GNU g++ with C++17  
**Compiler flags:** `-std=c++17 -Wall -Wextra -pedantic`

---

## 1. Objective

This report records the execution results of the automated test programs available in the project's `tests/` directory. The tests cover queue management, vehicle management, performance workloads, and the test-plan scenarios TC-01 to TC-12.

## 2. Test files and execution commands

Each test program was compiled and executed from the project root directory.

| Test file | Compilation command | Execution command |
|---|---|---|
| `tests/test_queue_manager.cpp` | `g++ -std=c++17 -Wall -Wextra -pedantic tests/test_queue_manager.cpp -o test_queue_manager.exe` | `test_queue_manager.exe` |
| `tests/test_vehicle_manager.cpp` | `g++ -std=c++17 -Wall -Wextra -pedantic tests/test_vehicle_manager.cpp -o test_vehicle_manager.exe` | `test_vehicle_manager.exe` |
| `tests/test_performance.cpp` | `g++ -std=c++17 -Wall -Wextra -pedantic tests/test_performance.cpp -o test_performance.exe` | `test_performance.exe` |
| `tests/test_plan_cases.cpp` | `g++ -std=c++17 -Wall -Wextra -pedantic tests/test_plan_cases.cpp -o test_plan_cases.exe` | `test_plan_cases.exe` |

All four compilation commands completed without reported compiler errors. All four test programs executed and reported successful results.

## 3. Test execution summary

| Test program | Coverage / purpose | Observed result |
|---|---|---|
| `test_queue_manager.cpp` | FIFO order, queue length, lane independence, empty-queue safeguards, invalid lane IDs, duplicate vehicle prevention | **PASS** — all reported checks passed |
| `test_vehicle_manager.cpp` | Vehicle entry validation, duplicate rejection, signal-based exit rules, FIFO exit order, empty/invalid lane handling | **PASS** — all reported checks passed |
| `test_performance.cpp` | Component stress test and full simulation workload | **PASS** — all reported checks passed |
| `test_plan_cases.cpp` | Test-plan scenarios TC-01 through TC-12 | **PASS** — 12 of 12 test cases passed |

No test failures were reported in the captured command output.

## 4. Detailed results

### 4.1 Queue Manager Tests

**Test file:** `tests/test_queue_manager.cpp`

The test program checked:

- FIFO ordering and queue length after enqueue and dequeue operations.
- Independence of queues belonging to different lanes.
- Protection against dequeue and peek operations on empty queues.
- Rejection of invalid lane identifiers.
- Prevention of duplicate vehicle IDs, including IDs that had already left a queue.
- Rejection of empty vehicle IDs.

**Result:** PASS. The program ended with `All tests passed.`

### 4.2 Vehicle Manager Tests

**Test file:** `tests/test_vehicle_manager.cpp`

The test program checked:

- Adding valid vehicles and storing vehicle information.
- Rejection of empty or whitespace-only IDs and vehicle types.
- Rejection of invalid lanes and duplicate vehicle IDs.
- Blocking vehicle movement on RED and YELLOW signals.
- Allowing vehicle movement on GREEN signals.
- FIFO vehicle exit order.
- Handling of empty and invalid lanes.

**Result:** PASS. The program ended with `All tests passed.`

### 4.3 Performance Tests

**Test file:** `tests/test_performance.cpp`

Two workloads were executed:

| Workload | Observed result |
|---|---|
| Component stress test with 200,000 vehicles | Completed in **784.331 ms** |
| Full simulation workload with 20,000 vehicles | 26,736 ticks completed in **297.615 ms** |

The checks verified successful vehicle addition and processing, FIFO order, no duplicate departures or lost vehicles, signal restrictions, normal program termination, and final queue state.

**Result:** PASS. The program ended with `All tests passed.`

*Note: Execution times are observations from this test run and may vary with hardware and system load.*

### 4.4 Test Plan Execution

**Test file:** `tests/test_plan_cases.cpp`

The program executed test cases TC-01 to TC-12:

| Test case | Scenario | Result |
|---|---|---|
| TC-01 | Add a vehicle with valid input | PASS |
| TC-02 | Per-lane FIFO order | PASS |
| TC-03 | Signal cycle RED / YELLOW / GREEN | PASS |
| TC-04 | Vehicle waits on RED/YELLOW and leaves on GREEN | PASS |
| TC-05 | Traffic status display checklist | PASS |
| TC-06 | Lane above congestion threshold | PASS |
| TC-07 | Lane below congestion threshold | PASS |
| TC-08 | Traffic statistics | PASS |
| TC-09 | Representative workload | PASS |
| TC-10 | Empty-queue operation and integrity | PASS |
| TC-11 | Invalid input rejection | PASS |
| TC-12 | Mixed valid and invalid operations | PASS |

The representative workload in TC-09 processed 5,000 vehicles across 6,736 ticks in **74.163 ms**.

**Result:** 12 of 12 test cases passed.

## 5. Overall conclusion

All four test programs compiled and executed successfully in the recorded test run. The queue manager, vehicle manager, performance, and test-plan checks reported no failures.

The results provide evidence that the tested queue operations, vehicle validation and movement rules, traffic signal restrictions, simulation statistics, and representative workloads behaved as expected during this run.

---
**End of report**
