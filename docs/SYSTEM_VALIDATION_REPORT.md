# System Validation Test Report

## 1. Objective

Validate the Traffic Management System's queue and vehicle behavior, end-to-end
console simulation, and representative high-volume workloads.

## 2. Execution Details

- **Execution date:** 2026-11-02
- **Environment:** macOS
- **Compiler:** Apple Clang 21.0.0, invoked as `g++`
- **Result:** 4 of 4 test suites passed; 0 failures

The test programs were compiled from the repository root and their exit codes
and output were checked. Test executables and logs were written under `/tmp`;
no generated binaries or logs were added to the repository.

## 3. Test Results

| Test suite | Scope | Result |
|---|---|---:|
| [Queue Manager](../tests/test_queue_manager.cpp) | FIFO order, independent lane queues, empty/invalid queue handling, and duplicate ID prevention | PASS (28 checks) |
| [Vehicle Manager](../tests/test_vehicle_manager.cpp) | Vehicle validation and entry, FIFO exit, signal restrictions, and empty/invalid lane handling | PASS (28 checks) |
| [System Test Plan](../tests/test_plan_cases.cpp) | End-to-end console scenarios TC-01 through TC-12, including signals, congestion, statistics, invalid input, and mixed operations | PASS (12/12 cases) |
| [Performance and Workload](../tests/test_performance.cpp) | 200,000-vehicle component stress test and 20,000-vehicle end-to-end simulation | PASS (11 checks) |

### Workload observations

- The component stress test added and processed all 200,000 vehicles across
  four lanes. It verified FIFO order, uniqueness, empty queues at completion,
  and that red signals block vehicle movement.
- The end-to-end workload accepted and processed all 20,000 vehicles, completed
  26,736 simulation ticks, and ended with every lane empty. Signal cycling and
  normal application exit were also verified.
- On this run, the measured workloads completed in approximately 51.6 ms and
  42.4 ms respectively. These timings are informational only; the requirements
  define correctness and completion, not a response-time threshold.

## 4. Build and Run Commands

The following commands were used. The output binaries were placed in `/tmp`.

```sh
g++ -std=c++17 -Wall -Wextra -pedantic \
  tests/test_queue_manager.cpp -o /tmp/tms_test_queue_manager
/tmp/tms_test_queue_manager

g++ -std=c++17 -Wall -Wextra -pedantic \
  tests/test_vehicle_manager.cpp -o /tmp/tms_test_vehicle_manager
/tmp/tms_test_vehicle_manager

g++ -std=c++14 -O2 -Wall -Wextra \
  tests/test_plan_cases.cpp -o /tmp/tms_test_plan_cases
/tmp/tms_test_plan_cases

g++ -std=c++17 -O2 -Wall -Wextra -pedantic \
  tests/test_performance.cpp -o /tmp/tms_test_performance
/tmp/tms_test_performance
```

## 5. Conclusion

The current checkout passed all executed queue, vehicle, system, and workload
validation suites. This report records functional and workload validation; it
does not claim additional platform coverage or replace the separate
[code coverage report](./CODE_COVERAGE_REPORT.md).
