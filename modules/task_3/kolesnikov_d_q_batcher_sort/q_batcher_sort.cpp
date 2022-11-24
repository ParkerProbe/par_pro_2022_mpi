#include "../../../modules/task_3/kolesnikov_d_q_bather_sort/q_batcher_sort.h"






void Compare(vector<int>& a, int i, int j) {
  if (a[i] > a[j]) {
    swap(a[i], a[j]);
  }
}



void BatcherMerge(vector<int>& a, int l, int n, int r) {
  int m = r * 2;
  if (m < n) {
    BatcherMerge(a, l, n, m);  // even subsequence
    BatcherMerge(a, l + r, n, m); // odd subsequence
    for (int i = l + r; i + r < l + n; i+=m) {
      Compare(i, i + r);
    }
  }
  else {
    Compare(a, l, l + r);
  }
}


void SeqQuickSort(vector<int>& data, int l, int r) {
  if (l < r) {
    double pivot=data[l];
    int p = l;
    for (int i=l+1; i < r; i++)
      if (data[i] < pivot) {
        p++;
        swap(data[i],data[p]);
      }
    swap(data[l],data[p]);
    SeqQuickSort(data,l,p);
    SeqQuickSort(data,p+1,r);
  }
}



void PrlQuickSort(vector<int> data, int count) {

  if (count == 0) {
    return;
  }
  
  int number_of_process;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD, &number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  chunk_size= (count % number_of_process == 0) ?
        (count /
        number_of_process) :
        (count /
        (number_of_process - 1);

  vector<int> chunk(chunk_size); 
  MPI_Scatter(data, chunk_size, MPI_INT, chunk,
        chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  data.clear();
  // Not last chank
  own_chunk_size = (count >= chunk_size*(rank + 1))
      ? chunk_size
      : (count - chunk_size*rank);

  SeqQuickSort(chunk, 0, own_chunk_size);

  for(int step = 1; step < number_of_process; step = 2 * step) {
    if (rank % (2 * step) != 0) {
      MPI_Send(chunk, own_chunk_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      break;
    }

    if (rank + step < number_of_process) {
      int received_chunk_size = (count >= chunk_size * (rank + 2 * step))
          ? (chunk_size * step)
          : (count - chunk_size * (rank + step));
      vector<int> chank_received(received_chunk_size);
      MPI_Recv(chunk_received, received_chunk_size,
          MPI_INT, rank + step, 0,
          MPI_COMM_WORLD, &status);

        data.clear();
        move(a.begin(), a.end(), data.begin());
        move(b.begin(), b.end(), data.begin() + size_a);

        BatcherMerge(data, 0, data.size() - 1, 1);

      chunk = data;
      own_chunk_size = own_chunk_size + received_chunk_size;
    }
  }
  return data;
}
