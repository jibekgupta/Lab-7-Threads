#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20 // Define MAX as 20 as specified in the task

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// Thread argument structure to pass thread-specific information
typedef struct {
  int thread_id;
  int start_row;
  int end_row;
} thread_args;

int i;
int j;
void fillMatrix(int matrix[MAX][MAX]) {
    for(i = 0; i<MAX; i++) {
        for(j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

int i;
int j;
void printMatrix(int matrix[MAX][MAX]) {
    for(i = 0; i<MAX; i++) {
        for(j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
  thread_args* t_args = (thread_args*)args;

  // Each thread handles a subset of rows
  int i;
  int j;
  for (i = t_args->start_row; i < t_args->end_row; i++) {
    for (j = 0; j <MAX; j++) {
      matSumResult[i][j] = matA[i][j] + matB[i][j];
    }
  }

  free(args); // Free dynamically allocated memory
  return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
  thread_args* t_args = (thread_args*)args;

  // Each thread handles a sbust of rows
  int i;
  int j;
  for (i = t_args->start_row; i < t_args->end_row; i++) {
    for (j = 0; j < MAX; j++) {
      matDiffResult[i][j] = matA[i][j] - matB[i][j];
    }
  }
  free(args);
  return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
  thread_args* t_args = (thread_args*)args;

  // Each thread handles a sbust of rows
  int i;
  int j;
  int k;
  for (i = t_args->start_row; i < t_args->end_row; i++) {
    for (j = 0; j < MAX; j++) {
      matProductResult[i][j] = 0;
      for (k = 0; k < MAX; k++) {
        matProductResult[i][j] += matA[i][k] * matB[k][j];
      }
    }
  }
  free(args);
  return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 1. Fill the matrices (matA and matB) with random values.
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t threads[10];

    // 4. Create a thread for each cell of each matrix operation.
    int rows_per_thread = MAX / 10;

    int i;
    for (i = 0; i < 10; i++) {
      thread_args* args = malloc(sizeof(thread_args));
      args->thread_id = i;
      args->start_row = i * rows_per_thread;
      args->end_row = (i==9) ? MAX : (i+1) * rows_per_thread;

      pthread_create(&threads[i], NULL, computeSum, (void*)args);
    }
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.

    // wait for sum threads to complete
    
    for (i = 0; i< 10; i++) {
      pthread_join(threads[i],NULL);
    }

    // Create threads for matrix difference
    
    for (i = 0; i < 10; i++) {
      thread_args* args = malloc(sizeof(thread_args));
      args->thread_id = i;
      args->start_row = i * rows_per_thread;
      args->end_row = (i == 9) ? MAX : (i+1) * rows_per_thread;

      pthread_create(&threads[i], NULL, computeDiff, (void*)args);
    }

    // Wait for difference threads to complete
    
    for (i = 0; i<10; i++) {
      pthread_join(threads[i], NULL);
    }

    // Create threads for matix product
    
    for (i = 0; i < 10; i++) {
      thread_args* args = malloc(sizeof(thread_args));
      args->thread_id = i;
      args->start_row = i * rows_per_thread;
      args->end_row = (i == 9) ? MAX : (i+1) * rows_per_thread;

      pthread_create(&threads[i], NULL, computeProduct, (void*)args);
    }
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    
    // 5. Wait for all threads to finish.
    
    for (i = 0; i < 10; i++) {
      pthread_join(threads[i], NULL);
    }
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}