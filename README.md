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

Execution time of each step is measured and reported to `stdout`.

## Getting Started

### Installation

Clone or download the project repository.

```bash
git clone https://github.com/cslabcbnu/SHINOBI.git
cd SHINOBI
make
```

## Example
```bash
./SHINOBI 8g

Benchmarking ready
execution time : 0h 0m 11s 718ms
Sequential summation done
execution time : 0h 0m 13s 963ms
Binary search done
execution time : 0h 6m 24s 468ms
All data shuffle done
execution time : 0h 2m 53s 756ms
Shuffle summation done
execution time : 0h 4m 7s 220ms
Heapify / Heappop done
execution time : 0h 34m 55s 238ms
```
![Image](https://github.com/user-attachments/assets/f579d1d1-26d9-4bd2-89d3-15735b0d7bfd)

*recorded by damo(https://github.com/damonitor/damo)

## Contact

If you have any questions, feel free to open an issue on this repository or contact hyeonsa@cbnu.ac.kr.

---
