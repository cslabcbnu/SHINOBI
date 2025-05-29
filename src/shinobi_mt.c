#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>


long long int idx;


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
		"Usage: %s -m <size>[g|m|k] -t <num> [-s0 <num>] [-s1 <num>] [-s2 <num>] [-s3 <num>] [-s4 <num>]\n"
		"  -m <size>	 memory size that you need (e.g. 8g, 512m, 1024k)\n"
		"  -t <num>	 numbers of threads that you need (e.g. 2, 4, 8)\n"
		"  -s0 <num>	 sequence 0 (Sequential Sum Calculation) repeat times (default: 1)\n"
		"  -s1 <num>	 sequence 1 (Binary Search Sum Calculation) repeat times (default: 1)\n"
		"  -s2 <num>	 sequence 2 (Sattolo Algorithm Shuffle ~ Tracked Summation) repeat times (default: 1)\n"
		"  -s3 <num>	 sequence 3 (Sattolo Algorithm Shuffle ~ Heap Sort) repeat times (default: 1)\n"
		"  -s4 <num>	 sequence 4 (Sattolo Algorithm Shuffle ~ Quick Sort) repeat times (default: 1)\n",
		prog
	);
}


void print_execution_time(struct timespec start, struct timespec end, int thread_no) {
	double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	int hours = (int)(elapsed_time / 3600);
	int min = ((int)(elapsed_time) % 3600) / 60;
	int sec = (int)(elapsed_time) % 60;
	int msec = (int)((elapsed_time - (int)elapsed_time) * 1000);
	
	if (thread_no + 1) fprintf(stdout, "[THREAD %d][RESULT] Execution Time : %dh %dm %ds %dms\n", thread_no, hours, min, sec, msec);
	else fprintf(stdout, "[RESULT] Execution Time : %dh %dm %ds %dms\n", hours, min, sec, msec);
}


void swap(long long int* a, long long int* b) {
	long long int temp = *a;
	*a = *b;
	*b = temp;
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


void heapify(long long int* arr, long long int idx, long long int i) {
	long long int max = i;
	while (1) {
		long long int left = 2 * max + 1;
		long long int right = 2 * max + 2;
		long long int next = max;

		if (left < idx && arr[left] > arr[next]) next = left;
		if (right < idx && arr[right] > arr[next]) next = right;
		if (next == max) break;
		

		swap(&arr[max], &arr[next]);
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

		swap(&arr[i], &arr[max]);
		i = max;
	}

	return pop;
}


long long int partition(long long int* arr, long long int low, long long int high) {
	long long int pivot = arr[high];
	long long int i = low - 1;
	for (long long int j = low; j < high; j++) {
		if (arr[j] <= pivot) {
			i += 1;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return i + 1;
}


void quick_sort(long long int* arr, long long int low, long long int high) {
	if (low < high) {
		long long int p = partition(arr, low, high);
		quick_sort(arr, low, p -1);
		quick_sort(arr, p + 1, high);
	}
}


void seq_sum(long long int* bench, int thread_no) {
	long long int ans = idx * (idx - 1) / 2;
	long long int target = 0;

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (long long int i = 0; i < idx; i++) {
		target += bench[i];
	}
	if (target != ans) {
		fprintf(stderr, "[THREAD %d][ERROR]  Sequential Sum Calculation failed., ans = %lld target = %lld\n\n", thread_no, ans, target);
		exit(-1); 
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
}


void bin_sum(long long int* bench, int thread_no) {
	long long int ans = idx * (idx - 1) / 2;
	long long int target = 0;
	
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (long long int i = 0; i < idx; i++) {
		target += binary_search(bench, 0, idx - 1, i);
	}
	if (target != ans) {
		fprintf(stderr, "[THREAD %d][ERROR]  Binary Search Sum Calculation failed., ans = %lld target = %lld\n\n", thread_no, ans, target);
		exit(-1); 
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
}


void shuffle(long long int* bench, int thread_no) {
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (long long int i = 0; i < idx; i++) {
		bench[i] = i;
	}
	for (long long int i = idx - 1; i > 0; i--) {
		long long int j = rand() % i;
		swap(&bench[i], &bench[j]);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
	fprintf(stdout, "[THREAD %d][ALERT]  Sattolo Algorithm Shuffle done\n\n", thread_no);
}


void shuffle_sum(long long int* bench, int thread_no) {
	shuffle(bench, thread_no);
	long long int ans = idx * (idx - 1) / 2;
	long long int target = 0;
	long long int i = 0;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		target += bench[i];
		i = bench[i];
	} while (i != 0);
	if (target != ans) {
		fprintf(stderr, "[THREAD %d][ERROR]  Tracked Shuffle Sum Calculation failed., ans = %lld target = %lld\n\n", thread_no, ans, target);
		exit(-1); 
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
}


void heap(long long int* bench, int thread_no) {
	shuffle(bench, thread_no);
	long long int hsize = idx;
	
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (long long int i = hsize / 2 - 1; i >= 0; i--) heapify(bench, idx, i);
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
	fprintf(stdout, "[THREAD %d][ALERT]  Heapify done\n", thread_no);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for (long long int i = idx - 1; i >= 0; i--) {
		if (i !=  heappop(bench, &hsize)) {
			fprintf(stderr, "[THREAD %d][ERROR]  Heapify / Heappop failed., target = %lld\n\n", thread_no, i);
			exit(-1); 
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
	fprintf(stdout, "[THREAD %d][ALERT]  Heappop done\n\n", thread_no);
}


void quick(long long int* bench, int thread_no) {
	shuffle(bench, thread_no);
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	quick_sort(bench, 0, idx - 1);
	for (long long int i = 0; i < idx; i++) {
		if (bench[i] != i) {
			fprintf(stderr, "[THREAD %d][ERROR]  Quick Sort failed, bench[%lld] == %lld\n\n", thread_no, i, bench[i]);
			exit(-1);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
}


struct thread_args {
	int thread_no;
	char s[5];
	long long int* part;
};


void* thread_func(void* arg) {
	struct thread_args* args = (struct thread_args*)arg;
	int thread_no = args->thread_no;
	char seq[5];
	for (int i = 0; i < 5; i++) {
		seq[i] = args->s[i];
	}
	
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	long long int* bench = args->part;
	if (!bench) {
		fprintf(stderr, "[THREAD %d][ERROR] Memory allocation failed\n", thread_no);
		pthread_exit(NULL);
	}

	for (long long int i = 0; i < idx; i++) {
		bench[i] = i;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, thread_no);
	
	fprintf(stdout, "[THREAD %d][ALERT]  Benchmarking ready\n\n", thread_no);

	// 1. Sequential Summation
	while (seq[0] > 0) {
		seq_sum(bench, thread_no);
		if (seq[0]-- > 1) fprintf(stdout, "[THREAD %d][ALERT]  Sequential Sum Calculation repeat %d times remaining\n\n", thread_no, seq[0]);
		else fprintf(stdout, "[THREAD %d][ALERT]  Sequential Sum Calculation done\n\n", thread_no);
	}

	// 2. Binary Search Summation
	while (seq[1] > 0) {
		bin_sum(bench, thread_no);
		if (seq[1]-- > 1) fprintf(stdout, "[THREAD %d][ALERT]  Binary Search Sum Calculation repeat %d times remaining\n\n", thread_no, seq[1]);
		else fprintf(stdout, "[THREAD %d][ALERT]  Binary Search Sum Calculation done\n\n", thread_no);
	}

	// 4. Shuffle Summation
	while (seq[2] > 0) {
		shuffle_sum(bench, thread_no);
		if (seq[2]-- > 1) fprintf(stdout, "[THREAD %d][ALERT]  Tracked Shuffle Sum Calculation repeat %d times remaining\n\n", thread_no, seq[2]);
		else fprintf(stdout, "[THREAD %d][ALERT]  Tracked Shuffle Sum Calculation done\n\n", thread_no);
	}
	
	// 5. Heapify / Heappop
	while (seq[3] > 0) {
		heap(bench, thread_no);
		if (seq[3]-- > 1) fprintf(stdout, "[THREAD %d][ALERT]  Heapify / Heappop repeat %d times remaining\n\n", thread_no, seq[3]);
		else fprintf(stdout, "[THREAD %d][ALERT]  Heapify / Heappop done\n\n", thread_no);
	}


	// 6. Quick Sort
	while (seq[4] > 0) {
		quick(bench, thread_no);
		if (seq[4]-- > 1) fprintf(stdout, "[THREAD %d][ALERT]  Quick Sort repeat %d times remaining\n\n", thread_no, seq[4]);	
		else fprintf(stdout, "[THREAD %d][ALERT]  Quick Sort done\n\n", thread_no);	
	}


	fprintf(stdout, "[THREAD %d][ALERT]  SHINOBI successfully completed.\n\n", thread_no);

	pthread_exit(NULL);

}


int main(int argc, char *argv[]) {
	long long int m_size = -1;
	char t = -1;
	char s[5] = {1, 1, 1, 1, 1};

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-m") == 0 && i+1 < argc) {
			if (parse_size(argv[i+1], &m_size) != 0) {
				fprintf(stderr, "[ERROR]  Invalid -m argument.\n");
				print_usage(argv[0]);
				return -1;
			}
			i++;
		} else if (strncmp(argv[i], "-s", 2) == 0 && strlen(argv[i]) == 3 && argv[i][2] >= '0' && argv[i][2] <= '4' && i+1 < argc) {
			int idx = argv[i][2] - '0';
			char sval = atoi(argv[i+1]);
			if (sval < 0) {
				fprintf(stderr, "[ERROR]  -s%d must be >= 0.\n", idx);
				print_usage(argv[0]);
				return -1;
			}
			s[idx] = sval;
			i++;
		} else if (strcmp(argv[i], "-t") == 0 && i+1 < argc) {
			t = atoi(argv[i+1]);
			if (t < 1) {
				fprintf(stderr, "[ERROR]  -t must be >= 1.\n");
				print_usage(argv[0]);
				return -1;
			}
			i++;		
		} else {
			fprintf(stderr, "[ERROR]  Unknown or incomplete option: %s\n", argv[i]);
			print_usage(argv[0]);
			return -1;
		}
	}

	if (m_size == -1) {
		fprintf(stderr, "[ERROR]  -m option is required.\n");
		print_usage(argv[0]);
		return -1;
	}

	if (t == -1) {
		fprintf(stderr, "[ERROR]  -t option is required.\n");
		print_usage(argv[0]);
		return -1;
	}

	srand(time(NULL));
	idx = m_size / 8 / t;
	pthread_t threads[t];
	struct thread_args args[t];

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	long long int* bench = (long long int*)malloc(idx * t * sizeof(long long int));

	// 0. Thraed preparation
	for (int i = 0; i < t; i++) {
		args[i].thread_no = i;
		for (int j = 0; j < 5; j++) {
			args[i].s[j] = s[j];
		}
		args[i].part = bench + i * idx;
		if (pthread_create(&threads[i], NULL, thread_func, &args[i])) {
			fprintf(stderr, "[Thread %d][ERROR] pthread_create failed\n", i);
			return -1;
		}
	}

	for (int i = 0; i < t; i++) {
		pthread_join(threads[i], NULL);
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_execution_time(start, end, -1);

	free(bench);
	fprintf(stdout, "[ALERT]  SHINOBI successfully completed.\n");

	return 0;
}

