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

void Compare(vector<int>* a, int i, int j) {
  if ((*a)[i] > (*a)[j]) {
    swap((*a)[i], (*a)[j]);
  }
}


void BatcherMerge(vector<int>* arr, int n, int l, int r) {
  int m = r * 2;
  if (m < n) {
    BatcherMerge(arr, n, l, m);
    BatcherMerge(arr, n, l + r, m);
    for (int i = l + r; i + r < l + n; i += m) {
      Compare(arr, i, i+r);
    }
  } else {
      Compare(arr, l, l+r);
  }
}


vector<int> Merge(vector<vector<int>> vectors) {
  while (vectors.size() != 1) {
    for (int i = 0; i < vectors.size(); i++) {
      auto temp = vectors[i];
      temp.insert(temp.end(),
          vectors[i + 1].begin(), vectors[i + 1].end());
      BatcherMerge(&temp, temp.size());
      vectors[i] = temp;
      vectors.erase(vectors.begin() + i);
    }
  }
  return vectors[0];
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


vector<int> PrlQuickSort(vector<int> arr, int size) {
    int procNum, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeOfBuff = size / procNum;
    vector<int> localVector, result;

    localVector.resize(sizeOfBuff);

    MPI_Scatter(arr.data(), sizeOfBuff, MPI_INT, localVector.data(),
        sizeOfBuff, MPI_INT, 0, MPI_COMM_WORLD);
    SeqQuickSort(&localVector, 0, localVector.size());

    if (rank != 0) {
        MPI_Send(localVector.data(), sizeOfBuff, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        vector<vector<int>> vectorOfVal;
        vectorOfVal.push_back(localVector);

        for (int i = 1; i < procNum; ++i) {
            MPI_Recv(localVector.data(), sizeOfBuff,
                MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            vectorOfVal.push_back(localVector);
        }

        result = Merge(vectorOfVal);
    }

    return result;
}
