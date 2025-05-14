#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


int parse_size(const char* input, long long int* value_out) {
	int len = strlen(input);
	if (len < 2) return -1;

	char unit = tolower(input[len-1]);
	char numpart[32];

	if (len-1 >= sizeof(numpart)) return -1;

	strncpy(numpart, input, len-1);
	numpart[len-1] = '\0';

	for (int i = 0; i < len-1; i++) {
		if (!isdigit(numpart[i])) return -1;
	}

	long long value = strtoll(numpart, NULL, 10);
	if (value <= 0) return -1;

	switch(unit) {
		case 'g': 
			value *= 1024 * 1024 * 1024;
			break;
		case 'm': 
			value *= 1024 * 1024;
			break;
		case 'k':
			value *= 1024;
			break;
		default:
			return -1;
	}
	*value_out = value;

	return 0;
}


void print_usage(const char* prog) {
	fprintf(stderr,
		"Usage: %s -m <size>[g|m|k] [-s0 <num>] [-s1 <num>] [-s2 <num>] [-s3 <num>]\n"
		"  -m <size>	 memory size that you need (e.g. 8g, 512m, 1024k)\n"
		"  -s0 <num>	 sequence 0 (Sequential Sum Calculation) repeat times (default: 1)\n"
		"  -s1 <num>	 sequence 1 (Binary Search Sum Calculation) repeat times (default: 1)\n"
		"  -s2 <num>	 sequence 2 (Sattolo Algorithm Shuffle ~ Heappify) repeat times (default: 1)\n",
		prog
	);
}


void print_execution_time(clock_t start, clock_t end) {
	double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
	int hours = (int)(elapsed_time / 3600);
	int min = ((int)(elapsed_time) % 3600) / 60;
	int sec = (int)(elapsed_time) % 60;
	int msec = (int)(end - start) % 1000;

	fprintf(stdout, "execution time : %dh %dm %ds %dms\n", hours, min, sec, msec);
}


int binary_search(long long int* arr, long long int left, long long int right, long long int target) {
	while (left <= right) {
		long long int mid = left + (right - left) / 2;
		if (arr[mid] == target) {
			return mid;
		}
		else if (arr[mid] < target) {
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}

	return -1;
}


void shuffle(long long int* arr, long long int idx) {
	for (long long int i = 0; i < idx; i++) {
		arr[i] = i;
	}
	for (long long int i = idx - 1; i > 0; i--) {
		long long int j = rand() % i;
		long long int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}


void heapify(long long int* arr, long long int idx, long long int i) {
	long long int max = i;
	while (1) {
		long long int left = 2 * max + 1;
		long long int right = 2 * max + 2;
		long long int next = max;

		if (left < idx && arr[left] > arr[next]) next = left;
		if (right < idx && arr[right] > arr[next]) next = right;
		if (next == max) break;
		
		long long int temp = arr[max];
		arr[max] = arr[next];
		arr[next] = temp;

		max = next;
	}
}


long long int heappop(long long int* arr, long long int* idx) {
	long long int pop = arr[0];
	arr[0] = arr[*idx - 1];
	(*idx) -= 1;

	long long int i = 0;
	while (1) {
		long long int left = 2 * i + 1;
		long long int right = 2 * i + 2;
		long long int max = i;

		if (left < *idx && arr[left] > arr[max]) max = left;
		if (right < *idx && arr[right] > arr[max]) max = right;
		if (max == i) break;

		long long int temp = arr[i];
		arr[i] = arr[max];
		arr[max] = temp;
		i = max;
	}

	return pop;
}


int main(int argc, char *argv[]) {
	long long m_size = -1;
	int s[4] = {1, 1, 1, 1};

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-m") == 0 && i+1 < argc) {
			if (parse_size(argv[i+1], &m_size) != 0) {
				fprintf(stderr, "Error: Invalid -m argument.\n");
				print_usage(argv[0]);
				return -1;
			}
			i++;
		} else if (strncmp(argv[i], "-s", 2) == 0 && strlen(argv[i]) == 3 && argv[i][2] >= '0' && argv[i][2] <= '3' && i+1 < argc) {
			int idx = argv[i][2] - '0';
			int sval = atoi(argv[i+1]);
			if (sval < 1) {
				fprintf(stderr, "Error: -s%d must be >= 1.\n", idx);
				print_usage(argv[0]);
				return -1;
			}
			s[idx] = sval;
			i++;
		} else {
			fprintf(stderr, "Unknown or incomplete option: %s\n", argv[i]);
			print_usage(argv[0]);
			return -1;
		}
	}

	if (m_size == -1) {
		fprintf(stderr, "Error: -m option is required.\n");
		print_usage(argv[0]);
		return -1;
	}

	srand(time(NULL));
	clock_t start, end;
	long long int idx = m_size / 8;
	long long int ans = idx * (idx - 1) / 2;
	long long int target = 0;

	// 0. Benchmark preparation
	start = clock();

	long long int* bench = (long long int*)malloc(idx * sizeof(long long int));
	if (!bench) {
		fprintf(stderr, "Error: Memmory allocation failed\n");
		return -1;
	}
	for (long long int i = 0; i < idx; i++) {
		bench[i] = i;
	}
	end = clock();
	fprintf(stdout, "Benchmarking ready\n");
	print_execution_time(start, end);

	// 1. Sequential Summation
	while (s[0] > 1) {
		start = clock();
		for (long long int i = 0; i < idx; i++) {
			target += bench[i];
		}
		if (target != ans) {
			fprintf(stderr, "Error: Sequential Sum Calculation failed., ans = %lld target = %lld\n", ans, target);
			return -1; 
		}
		end = clock();
		fprintf(stdout, "Sequential Sum Calculation repeat %d times remaining\n", --s[0]);
		print_execution_time(start, end);
		target = 0;
	}
	start = clock();
	for (long long int i = 0; i < idx; i++) {
		target += bench[i];
	}
	if (target != ans) {
		fprintf(stderr, "Error: Sequential Sum Calculation failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Sequential Sum Calculation done\n");
	print_execution_time(start, end);
	target = 0;

	// 2. Binary Search Summation
	while (s[1] > 1) {
		start = clock();
		for (long long int i = 0; i < idx; i++) {
			target += binary_search(bench, 0, idx - 1, i);
		}
		if (target != ans) {
			fprintf(stderr, "Error: Binary Search Sum Calculation failed., ans = %lld target = %lld\n", ans, target);
			return -1; 
		}
		end = clock();
		fprintf(stdout, "Binary Search Sum Calculation repeat %d times remaining\n", --s[1]);
		print_execution_time(start, end);
		target = 0;
	}
	start = clock();
	for (long long int i = 0; i < idx; i++) {
		target += binary_search(bench, 0, idx - 1, i);
	}
	if (target != ans) {
		fprintf(stderr, "Error: Binary Search Sum Calculation failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Binary Search Sum Calculation done\n");
	print_execution_time(start, end);


	while (s[2] > 1) {
		start = clock();
		shuffle(bench, idx);
		end = clock();
		fprintf(stdout, "Sattolo Algorithm Shuffle repeat %d times remaining\n", --s[2]);
		print_execution_time(start, end);

		start = clock();
		long long int i = 0;
		target = 0;
		do {
			target += bench[i];
			i = bench[i];
		} while (i != 0);
		if (target != ans) {
			fprintf(stderr, "Error: Tracked Shuffle Sum Calculation failed., ans = %lld target = %lld\n", ans, target);
			return -1; 
		}
		end = clock();
		fprintf(stdout, "Tracked Shuffle Sum Calculation repeat %d times remaining\n", s[2]);
		print_execution_time(start, end);

		start = clock();
		long long int hsize = idx;
		target = 0;
		for (long long int i = hsize / 2 - 1; i >= 0; i--) heapify(bench, idx, i);
		end = clock();
		fprintf(stdout, "Heapify repeat %d times remaining\n", s[2]);
		print_execution_time(start, end);
	}
	// 3. Data Shuffle (Sattolo Algorithm)
	start = clock();
	shuffle(bench, idx);
	end = clock();
	fprintf(stdout, "Sattolo Algorithm Shuffle done\n");
	print_execution_time(start, end);

	// 4. Shuffle Summation
	start = clock();
	long long int i = 0;
	target = 0;
	do {
		target += bench[i];
		i = bench[i];
	} while (i != 0);
	if (target != ans) {
		fprintf(stderr, "Error: Tracked Shuffle Sum Calculation failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Tracked Shuffle Sum Calculation done\n");
	print_execution_time(start, end);

	// 5. Heapify
	start = clock();
	long long int hsize = idx;
	target = 0;
	for (long long int i = hsize / 2 - 1; i >= 0; i--) heapify(bench, idx, i);
	end = clock();
	fprintf(stdout, "Heapify done\n");
	print_execution_time(start, end);

	// 6. Heappop All
	start = clock();
	for (long long int i = 0; i < idx; i++) target += heappop(bench, &hsize);
	if (target != ans) {
		fprintf(stderr, "Error: Heapify / Heappop failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Heappop done\n");
	print_execution_time(start, end);


	free(bench);
	return 0;
}

