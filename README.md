# SHINOBI (SHINOBI Is Not Only Benchmark Iterator)

## Introduction

SHINOBI (SHINOBI Is Not Only Benchmark Iterator) is a flexible benchmark tool designed to measure memory performance. A key feature of this tool is its ability to allow users to **specify a minimum memory capacity** that the program will use for the benchmark, enabling precise memory performance analysis across various environments.

## Features

*   **Specific Memory Capacity Benchmarking**: Accurately specify the memory capacity to be tested in kibibytes (KiB) or mebibytes (MiB), also gibibytes (GiB).
*   **Easy to Use**: Easily execute and configure via a command-line interface.

## How it Works

The SHINOBI benchmark is structured around the following steps. The time taken to execute each step is measured and printed to standard output (stdout).

0.  **Array Allocation and Initialization**: Dynamically allocate an array of the specified capacity. Each element in the array is initialized with its own index (e.g., `bench[i] = i`).
1.  **Sequential Sum Calculation**: Calculate the sum of all elements from index 0 up to the maximum index in the array.
2.  **Binary Search Sum Calculation**: Calculate the sum of the entire array using a binary search approach. 
3.  **Sattolo Algorithm Shuffle**: Shuffle all elements of the array using the Sattolo algorithm.
4.  **Tracked Shuffle Sum Calculation**: Calculate the sum of the entire array by tracking values after the shuffle.
5.  **Heap Sort Sum Calculation**: Heapify the entire array, then attempt to `heappop` every element to calculate the sum.
6.  **Quick Sort**: Execute quick sort, then check all elements' sorting state.

Execution time of each step is measured and reported to `stdout`.

## Getting Started

### Installation

Clone or download the project repository.

```bash
git clone https://github.com/cslabcbnu/SHINOBI.git
cd SHINOBI
make
```

### Usage
```bash
Usage: ./SHINOBI_BASE -m <size>[g|m|k] [-s0 <num>] [-s1 <num>] [-s2 <num>] [-s3 <num>] [-s4 <num>]
  -m <size>      memory size that you need (e.g. 8g, 512m, 1024k)
  -s0 <num>      sequence 0 (Sequential Sum Calculation) repeat times (default: 1)
  -s1 <num>      sequence 1 (Binary Search Sum Calculation) repeat times (default: 1)
  -s2 <num>      sequence 2 (Sattolo Algorithm Shuffle ~ Tracked Summation) repeat times (default: 1)
  -s3 <num>      sequence 3 (Sattolo Algorithm Shuffle ~ Heap Sort) repeat times (default: 1)
  -s4 <num>      sequence 4 (Sattolo Algorithm Shuffle ~ Quick Sort) repeat times (default: 1)
```

## Example
```bash
./SHINOBI_BASE -m 1g -s0 2 -s2 2 -s1 2 -s3 2

execution time : 0h 0m 1s 445ms
Benchmarking ready
execution time : 0h 0m 0s 701ms
Sequential Sum Calculation repeat 1 times remaining
execution time : 0h 0m 0s 93ms
Sequential Sum Calculation done
Press Ctrl+C to stop
execution time : 0h 0m 13s 440ms
Binary Search Sum Calculation repeat 1 times remaining
execution time : 0h 0m 13s 763ms
Binary Search Sum Calculation done
execution time : 0h 0m 20s 447ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 29s 549ms
Tracked Shuffle Sum Calculation repeat 1 times remaining
execution time : 0h 0m 19s 141ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 28s 904ms
Tracked Shuffle Sum Calculation done
execution time : 0h 0m 19s 807ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 2s 164ms
Heapify done
execution time : 0h 1m 18s 613ms
Heappop done
Heapify / Heappop repeat 1 times remaining
execution time : 0h 0m 19s 976ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 2s 249ms
Heapify done
execution time : 0h 1m 18s 759ms
Heappop done
Heapify / Heappop done
execution time : 0h 0m 19s 719ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 21s 197ms
Quick Sort repeat 1 times remaining
execution time : 0h 0m 19s 843ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 21s 259ms
Quick Sort done
```
![Image](https://github.com/user-attachments/assets/254bb105-ca4e-4943-b314-9d1418aae687)

*recorded by damo(https://github.com/damonitor/damo)

## Contact

If you have any questions, feel free to open an issue on this repository or contact hyeonsa@cbnu.ac.kr.

---
