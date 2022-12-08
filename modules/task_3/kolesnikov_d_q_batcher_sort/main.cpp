// Copyright 2022 Kolesnikov Denis
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>


#include "./q_batcher_sort.h"
using std::vector;


TEST(Q_BATCHER_SORT, SORT_SEQ) {
  int size = 100;
  int rank;
  int num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  if (log2(num) == static_cast<int>(log2(num))) {
    if (rank == 0) {
        vector<int> arr = GenRndArr(100);
        SeqQuickSort(&arr, 0, arr.size());
        for (int i = 0; i < arr.size() - 1; i++) {
            EXPECT_TRUE(arr[i] <= arr[i + 1]);
        }
    }
  }
}

TEST(Q_BATCHER_SORT, BATCHER_MERGE_CORRECT) {
  int rank, num;
  int size = 100;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  if (log2(num) == static_cast<int>(log2(num))) {
    if (rank == 0) {
      vector<int> arr1 = GenRndArr(size);
      vector<int> arr2 = GenRndArr(size);
      SeqQuickSort(&arr1, 0, size);
      SeqQuickSort(&arr2, 0, size);
      vector<vector<int>> array;
      array.push_back(arr1);
      array.push_back(arr2);
      auto res = Merge(array);
      for (int i = 0; i < static_cast<int>(res.size()) - 1; i++) {
        EXPECT_TRUE(res[i] <= res[i + 1]);
      }
    }
  }
}

TEST(Q_BATCHER_SORT, SORT_SMALL) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  if (log2(num) == static_cast<int>(log2(num))) {
    vector<int> temp;
    if (rank == 0) {
      temp = GenRndArr(4);
    }
    temp = PrlQuickSort(temp, 4);
    if (rank == 0) {
      for (int i = 0; i < static_cast<int>(temp.size()) - 1; i++) {
          EXPECT_TRUE(temp[i] <= temp[i + 1]);
      }
    }
  }
}


TEST(Q_BATCHER_SORT, SORT_BIG) {
  int rank, num;
  int size = 10000;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  if (log2(num) == static_cast<int>(log2(num))) {
    vector<int> tmp;
    if (rank == 0) {
      tmp = GenRndArr(size);
    }
    tmp = PrlQuickSort(tmp, size);
    if (rank == 0) {
      for (int i = 0; i < static_cast<int>(tmp.size()) - 1; i++) {
          EXPECT_TRUE(tmp[i] <= tmp[i + 1]);
      }
    }
  }
}

TEST(Q_BATCHER_SORT, SORT_VERY_BIG) {
  int rank, num;
  int size = 128000;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  if (log2(num) == static_cast<int>(log2(num))) {
    vector<int> tmp;
    if (rank == 0) {
      tmp = GenRndArr(size);
    }
    tmp = PrlQuickSort(tmp, size);
    if (rank == 0) {
      for (int i = 0; i < static_cast<int>(tmp.size()) - 1; i++) {
          EXPECT_TRUE(tmp[i] <= tmp[i + 1]);
      }
    }
  }
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
