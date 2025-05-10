#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


long long int convert_bytes(char* program, char unit, long long int value) {
	switch (unit) {
		case 'k':
			return value * 1024;
		case 'm':
			return value * 1024 * 1024;
		case 'g':
			return value * 1024 * 1024 * 1024;
		default:
			fprintf(stderr, "Error: Unknown unit inputted. (Supported units: g, m, k)\n");
			fprintf(stderr, "Usage: %s <size>[g|m|k]\n", program);
			return -1;
	}


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
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <size>[g|m|k]\n", argv[0]);
		return -1;
	}

	char* input = argv[1];
	int len = strlen(input);

	if (len < 2) {
		fprintf(stderr, "Error: Invalid input format. Please provide a number followed by a unit.\n");
		fprintf(stderr, "Usage: %s <size>[g|m|k]\n", argv[0]);
		return -1;
	}

	char unit = tolower(input[len - 1]);

	input[len - 1] = '\0';

	for (int i = 0; i < len - 1; i++) {
		if (!isdigit(input[i])) {
			fprintf(stderr, "Error: Invalid numeric part '%s'.\n", input);
			fprintf(stderr, "Usage: %s <size>[g|m|k]\n", argv[0]);
			return -1;
		}
	}

	char* endptr;
	long long int value = strtoll(input, &endptr, 10);

	if (*endptr != '\0') {
		 fprintf(stderr, "Error: Failed to convert numeric part.\n");
		 return -1;
	}

	long long int result = 0;

	result = convert_bytes(argv[0], unit, value);
	if (result == -1) {
		return -1;
	}

	srand(time(NULL));
	clock_t start, end;
	long long int idx = result / 8;
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
	start = clock();
	for (long long int i = 0; i < idx; i++) {
		target += bench[i];
	}
	if (target != ans) {
		fprintf(stderr, "Error: Sequential summation failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Sequential summation done\n");
	print_execution_time(start, end);

	// 2. Binary Search Summation
	start = clock();
	target = 0;
	for (long long int i = 0; i < idx; i++) {
		target += binary_search(bench, 0, idx - 1, i);
	}
	if (target != ans) {
		fprintf(stderr, "Error: Binary search failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Binary search done\n");
	print_execution_time(start, end);

	// 3. Data Shuffle (Sattolo Algorithm)
	start = clock();
	shuffle(bench, idx);
	end = clock();
	fprintf(stdout, "All data shuffle done\n");
	print_execution_time(start, end);

	// 4. Shuffke Summation
	start = clock();
	long long int i = 0;
	target = 0;
	do {
		target += bench[i];
		i = bench[i];
	} while (i != 0);
	if (target != ans) {
		fprintf(stderr, "Error: Shuffle summation failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Shuffle summation done\n");
	print_execution_time(start, end);

	// 5. Heapify & Heappop All
	start = clock();
	long long int hsize = idx;
	target = 0;
	for (long long int i = hsize / 2 - 1; i >= 0; i--) heapify(bench, idx, i);
	for (long long int i = 0; i < idx; i++) target += heappop(bench, &hsize);
	if (target != ans) {
		fprintf(stderr, "Error: Heapify / Heappop failed., ans = %lld target = %lld\n", ans, target);
		return -1; 
	}
	end = clock();
	fprintf(stdout, "Heapify / Heappop done\n");
	print_execution_time(start, end);


	free(bench);
	return 0;
}

