# Code Coverage Report – Traffic Management System

## 1. Objective
Measure how much of the Traffic Management System source code is executed during testing using GCC and gcov.

## 2. Tools Used
- Programming Language: C++
- Compiler: GCC 6.3.0
- Coverage Tool: gcov 6.3.0
- Environment: Windows PowerShell

## 3. Test Suites

Four test suites were executed:

1. Queue Manager Tests
   - FIFO order and queue length
   - Independent lane queues
   - Empty queue handling
   - Invalid lane IDs
   - Duplicate vehicle prevention

2. Vehicle Manager Tests
   - Vehicle entry validation
   - Duplicate ID prevention
   - Vehicle exit on GREEN signal
   - FIFO vehicle exit
   - Empty and invalid lane handling

3. System Test Plan
   - 12 test cases covering vehicle entry, FIFO, signal cycles, congestion, statistics, workload, and invalid inputs.
   - All 12 test cases passed.

4. Performance Tests
   - Stress test with 200,000 vehicles across four lanes.
   - Full simulation workload with 20,000 vehicles.
   - Checked FIFO order, duplicate prevention, vehicle processing, and signal behavior.
   - All performance checks passed.

## 4. Coverage Results

| Test Suite | Test File Coverage | main.cpp Coverage |
|---|---:|---:|
| Queue Manager | 98.89% | 96.88% |
| Vehicle Manager | 98.92% | 97.78% |
| System Test Plan | 98.43% | 94.09% |
| Performance Test | 98.00% | 87.68% |

Note: These are individual test-suite coverage results, not an aggregated project-wide coverage percentage.

## 5. Coverage Method

The test files were compiled with GCC coverage instrumentation:

- -fprofile-arcs
- -ftest-coverage
- -O0
- -g

The generated execution data was analyzed using gcov.

## 6. Example Commands

Compile:

g++ -std=c++17 -fprofile-arcs -ftest-coverage -O0 -g tests/test_queue_manager.cpp -o coverage/test_queue_manager.exe

Run:

.\coverage\test_queue_manager.exe

Generate report:

gcov test_queue_manager.gcno

The same process was followed for the other test suites. The System Test Plan was compiled using C++14.

## 7. Report Files

- queue_manager_main.cpp.gcov
- vehicle_manager_main.cpp.gcov
- system_test_plan_main.cpp.gcov
- performance_main.cpp.gcov
- queue_manager_tests.gcov
- vehicle_manager_tests.gcov
- system_test_plan_tests.gcov
- performance_tests.gcov

## 8. Conclusion

All four test suites executed successfully. Test-file coverage ranged from 98.00% to 98.92%.

The gcov reports provide evidence of source-code execution during testing and help identify code that may require additional test cases.
