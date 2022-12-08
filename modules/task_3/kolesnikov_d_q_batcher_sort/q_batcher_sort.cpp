// Copyright 2022 Kolesnikov Denis

#include "../../../modules/task_3/kolesnikov_d_q_batcher_sort/q_batcher_sort.h"


vector<int> GenRndArr(int size) {
  std::random_device dev;
  std::mt19937_64 gen(dev());
  vector<int> result;
  std::uniform_int_distribution<int> dist(0, 1000);
  for (int i = 0; i < size; i++) {
    result.push_back(dist(gen));
  }
  return result;
}

void BatcherMerge(vector<int>* arr, int n, int lo, int r) {
    int m = r * 2;
    if (m < n) {
      BatcherMerge(arr, n, lo, m);
      BatcherMerge(arr, n, lo + r, m);

      for (int i = lo + r; i + r < lo + n; i += m) {
        if ((*arr)[i] > (*arr)[i + r])
            std::swap((*arr)[i], (*arr)[i + r]);
      }
    } else {
      if ((*arr)[lo] > (*arr)[lo + r]) {
          std::swap((*arr)[lo], (*arr)[lo+r]);
      }
    }
}
vector<int> Merge(vector<vector<int>> v) {
  vector<int> res(v.size()*v[0].size());
  for (int i = 0; i < v.size(); i++) {
    std::move(v[i].begin(), v[i].end(), res.begin() + i*v.size());
  }
  BatcherMerge(&res, res.size());
  return res;
}

void SeqQuickSort(vector<int>* data, int l, int r) {
  if (l < r) {
    double pivot = (*data)[l];
    int p = l;
    for (int i=l+1; i < r; i++)
      if ((*data)[i] < pivot) {
        p++;
        swap((*data)[i], (*data)[p]);
      }
    swap((*data)[l], (*data)[p]);
    SeqQuickSort(data, l, p);
    SeqQuickSort(data, p+1, r);
  }
}



vector<int> PrlQuickSort(vector<int> data, int size) {
  if (size == 0) {
    return vector<int>();
  }
  int number_of_process;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD, &number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Status status;

  int chunk_size = size / number_of_process;
  vector<int> chunk(chunk_size);
  vector<int> result;
  chunk_size = (size >= chunk_size*(rank + 1))
    ? chunk_size
    : (size - chunk_size*rank);

  MPI_Scatter(data.data(), chunk_size, MPI_INT, chunk.data(),
        chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  SeqQuickSort(&chunk, 0, chunk_size);

  if (rank != 0) {
    MPI_Send(chunk.data(), chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else {
    vector<vector<int>> all;
    all.push_back(chunk);
    for (int i = 1; i < number_of_process; i++) {
      MPI_Recv(chunk.data(), chunk_size, MPI_INT,
          i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
      all.push_back(chunk);
    }
    vector<int> result = Merge(all);
  }
  return result;
}
