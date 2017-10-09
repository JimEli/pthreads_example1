/*
Instructions for implementing pthreads on MS Visual Studio 2017:

Download: http://cs.du.edu/~mitchell/pthreads_compiled.zip
add: pthread.h, sched.h & semaphore.h to "C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt"
add: pthreadVC2.lib to "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\ucrt\x86"
add: pthreadVC2.dll to "C:\Windows"
add: #define _TIMESPEC_DEFINED // Precludes redefining timespec struct located in time.h
add: pthreadVC2.lib to the Project Properties > Configuration Properties > Linker > Input > Additional Dependencies
*/
#define _TIMESPEC_DEFINED // Precludes redefining timespec struct located in time.h

// pthread matrix multiplication.
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SIZE 3   // Size by SIZE matrices

int numThreads;  // number of threads
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

// Initialize matrix with incemental values.
void initMatrix(int m[SIZE][SIZE], const int zero) {
	int i, j, val = 0;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			m[i][j] = (zero) ? 0 : val++;
}

// Pretty print matrix.
void printMatrix(const int m[SIZE][SIZE]) {
	int i, j;

	for (i = 0; i < SIZE; i++) {
		printf("\n\t| ");
		for (j = 0; j < SIZE; j++)
			printf("%2d ", m[i][j]);
		printf("|");
	}
}

// Thread function: takes "slice" as its argument.
void* multiply(void *slice) {
	int s = (int)slice;                     // Retrive the slice info.
	int from = (s * SIZE) / numThreads;     // Note: 'slicing' works fine, even if SIZE not divisible by num_thrd.
	int to = ((s + 1) * SIZE) / numThreads;
	int i, j, k;

	printf("computing slice %d (from row %d to %d)\n", s, from, to - 1);
	for (i = from; i < to; i++) {
		for (j = 0; j < SIZE; j++) { //C[i][j] = 0;
			for (k = 0; k < SIZE; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
	printf("finished slice %d\n", s);
	return 0;
}

int main(int argc, char* argv[]) {
	pthread_t* thread;  // pointer to a group of threads
	int i;

	if (argc != 2) {
		printf("Usage: %s number_of_threads\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	numThreads = atoi(argv[1]);
	//numThreads = 3;
	initMatrix(A, false);
	initMatrix(B, false);
	initMatrix(C, true);
	thread = (pthread_t*)malloc(numThreads * sizeof(pthread_t));

	// This for loop not entered if thread number is specified as 1.
	for (i = 1; i < numThreads; i++) {
		// Creates each thread working on its own slice of i
		if (pthread_create(&thread[i], NULL, multiply, (void*)i) != 0) {
			perror("Can't create thread");
			free(thread);
			exit(-1);
		}
	}

	// Main thread works on slice 0, so everybody is busy.
	// Main thread does everything if thread number is specified as 1.
	multiply(0);

	// Main thread waits for other thread(s) to complete.
	for (i = 1; i < numThreads; i++)
		pthread_join(thread[i], NULL);

	printf("\n\n");
	printMatrix(A);
	printf("\n\n\t * \n");
	printMatrix(B);
	printf("\n\n\t = \n");
	printMatrix(C);
	printf("\n\n");

	free(thread);
	return 0;
}

