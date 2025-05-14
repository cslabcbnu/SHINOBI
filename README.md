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

### Usage
```bash
Usage: ./SHINOBI_BASE -m <size>[g|m|k] [-s0 <num>] [-s1 <num>] [-s2 <num>] [-s3 <num>]
  -m <size>      memory size that you need (e.g. 8g, 512m, 1024k)
  -s0 <num>      sequence 0 (Sequential Sum Calculation) repeat times (default: 1)
  -s1 <num>      sequence 1 (Binary Search Sum Calculation) repeat times (default: 1)
  -s2 <num>      sequence 2 (Sattolo Algorithm Shuffle ~ Heappify) repeat times (default: 1)
```

## Example
```bash
./SHINOBI_BASE -m 1g -s0 3 -s2 3 -s1 3

Benchmarking ready
execution time : 0h 0m 1s 522ms
Sequential Sum Calculation repeat 2 times remaining
execution time : 0h 0m 0s 609ms
Sequential Sum Calculation repeat 1 times remaining
execution time : 0h 0m 0s 469ms
Sequential Sum Calculation done
execution time : 0h 0m 0s 679ms
Binary Search Sum Calculation repeat 2 times remaining
execution time : 0h 0m 14s 907ms
Binary Search Sum Calculation repeat 1 times remaining
execution time : 0h 0m 14s 579ms
Binary Search Sum Calculation done
execution time : 0h 0m 14s 778ms
Sattolo Algorithm Shuffle repeat 2 times remaining
execution time : 0h 0m 17s 893ms
Tracked Shuffle Sum Calculation repeat 2 times remaining
execution time : 0h 0m 25s 199ms
Heapify repeat 2 times remaining
execution time : 0h 0m 2s 440ms
Sattolo Algorithm Shuffle repeat 1 times remaining
execution time : 0h 0m 17s 75ms
Tracked Shuffle Sum Calculation repeat 1 times remaining
execution time : 0h 0m 25s 359ms
Heapify repeat 1 times remaining
execution time : 0h 0m 2s 417ms
Sattolo Algorithm Shuffle done
execution time : 0h 0m 17s 85ms
Tracked Shuffle Sum Calculation done
execution time : 0h 0m 25s 945ms
Heapify done
execution time : 0h 0m 2s 412ms
Heappop done
execution time : 0h 1m 15s 684ms
```
![Image](https://github.com/user-attachments/assets/ce9f82dd-f0e3-44b9-b5de-5fceb2ddee52)

*recorded by damo(https://github.com/damonitor/damo)

## Contact

If you have any questions, feel free to open an issue on this repository or contact hyeonsa@cbnu.ac.kr.

---
