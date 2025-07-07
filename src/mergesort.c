#include "mergesort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, int left, int mid, int right) {
  int left_size = mid - left;
  int right_size = right - mid;

  int *left_arr = (int *)malloc(left_size * sizeof(int));
  int *right_arr = (int *)malloc(right_size * sizeof(int));

  if (!left_arr || !right_arr) {
    fprintf(stderr, "Memory allocation failed in merge\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < left_size; i++) {
    left_arr[i] = arr[left + i];
  }
  for (int j = 0; j < right_size; j++) {
    right_arr[j] = arr[mid + j];
  }

  int i = 0, j = 0, k = left;
  while (i < left_size && j < right_size) {
    arr[k++] = (left_arr[i] <= right_arr[j]) ? left_arr[i++] : right_arr[j++];
  }

  while (i < left_size) arr[k++] = left_arr[i++];
  while (j < right_size) arr[k++] = right_arr[j++];

  free(left_arr);
  free(right_arr);
}


void merge_sort_sequential(int *arr, int left, int right) {
  if (right - left <= 1)
    return;
  int mid = (left + right) / 2;
  merge_sort_sequential(arr, left, mid);
  merge_sort_sequential(arr, mid, right);
  merge(arr, left, mid, right);
}

void merge_sort_parallel_internal(int *arr, int left, int right) {
  if (right - left <= 1)
    return;

  int mid = (left + right) / 2;

  // Use parallelism only for larger subarrays
  if (right - left > 1000) {
    #pragma omp task
    merge_sort_parallel_internal(arr, left, mid);
    #pragma omp task
    merge_sort_parallel_internal(arr, mid, right);
    #pragma omp taskwait
  } else {
    merge_sort_sequential(arr, left, mid);
    merge_sort_sequential(arr, mid, right);
  }

  merge(arr, left, mid, right);
}


void merge_sort_parallel(int *arr, int left, int right) {

// NOTE: This part need not be modified, the parallel and single
// constructs creates the threads.
#pragma omp parallel
#pragma omp single
  {
    // the main sorting logic will be in this function.
    merge_sort_parallel_internal(arr, left, right);
  }
}
