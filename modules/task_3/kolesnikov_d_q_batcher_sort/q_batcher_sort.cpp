// Copyright 2022 Kolesnikov Denis

#include "../../modules/task_3/kolesnikov_d_q_batcher_sort/q_batcher_sort.h"



void Compare(int* a, int i, int j) {
  if (a[i] > a[j]) {
    swap(a[i], a[j]);
  }
}

int* MergeArr(int* arr1, int* arr2) {
  int size1 = sizeof(arr1)/sizeof(int);
  int size2 = sizeof(arr2)/sizeof(int);
  int * result = new int[size1 + size2];
  std::copy(arr1, arr1 + size1, result);
  std::copy(arr2, arr2 + size2, result + size1);
  return result;
}

void BatcherMerge(int* a, int l, int n, int r) {
  int m = r * 2;
  if (m < n) {
    BatcherMerge(a, l, n, m);
    BatcherMerge(a, l + r, n, m);
    for (int i = l + r; i + r < l + n; i+=m) {
      Compare(a, i, i + r);
    }
  } else {
    Compare(a, l, l + r);
  }
}


void SeqQuickSort(int* data, int l, int r) {
  if (l < r) {
    double pivot = data[l];
    int p = l;
    for (int i=l+1; i < r; i++)
      if (data[i] < pivot) {
        p++;
        swap(data[i], data[p]);
      }
    swap(data[l], data[p]);
    SeqQuickSort(data, l, p);
    SeqQuickSort(data, p+1, r);
  }
}



void PrlQuickSort(int* data, int count) {
  if (count == 0) {
    return;
  }
  int number_of_process;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD, &number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  int chunk_size= (count % number_of_process == 0) ?
        (count /
        number_of_process) :
        (count /
        number_of_process - 1);
  int* chunk = new int[chunk_size];
  MPI_Scatter(data, chunk_size, MPI_INT, chunk,
        chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  delete [] data;
  data = nullptr;
  int own_chunk_size = (count >= chunk_size*(rank + 1))
      ? chunk_size
      : (count - chunk_size*rank);

  SeqQuickSort(chunk, 0, own_chunk_size);

  for (int step = 1; step < number_of_process; step = 2 * step) {
    if (rank % (2 * step) != 0) {
      MPI_Send(chunk, own_chunk_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      break;
    }

    if (rank + step < number_of_process) {
      int received_chunk_size = (count >= chunk_size * (rank + 2 * step))
          ? (chunk_size * step)
          : (count - chunk_size * (rank + step));
      int* chunk_received = new int[received_chunk_size];
      MPI_Recv(chunk_received, received_chunk_size,
          MPI_INT, rank + step, 0,
          MPI_COMM_WORLD, &status);
      data = MergeArr(chunk, chunk_received);
      BatcherMerge(data, 0, (sizeof(data)/sizeof(int)) - 1, 1);
      delete [] chunk;
      delete [] chunk_received;
      chunk = data;
      own_chunk_size = own_chunk_size + received_chunk_size;
    }
  }
  return;
}
