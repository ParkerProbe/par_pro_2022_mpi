// Copyright 2022 Kolesnikov Denis
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>


#include "./q_batcher_sort.h"
using std::vector;


TEST(Q_BATCHER_SORT, empty) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<int> a = {5};
  vector<int> sort = SeqQuickSort(&a[0], 0, a.size());
  if (rank == 0) {
      ASSERT_EQ(vector<int>(), sort);
  }
}



// TEST(Q_BATCHER_SORT, sort_small_arr) {
//   int rank;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   vector<int> arr = {5, 1, 3};
//   vector<int> sorted_arr = {1, 3, 5};
//   vector<int> sort = PrlQuickSort(arr.data(), arr.size());
//   if (rank == 0) {
//       vector<int> seq_sort = SeqQuickSort(arr.data(), 0, arr.size()-1);
//       ASSERT_EQ(seq_sort, sort);
//       ASSERT_EQ(seq_sort, sorted_arr);
//   }
// }


// TEST(Q_BATCHER_SORT, sort_big_arr_1) {
//   int rank;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   vector<int> arr = { 15, 70, 17, 68, 78, 63, 18, 21, 92, 2};
//   vector<int> sorted_arr = { 2, 15, 17, 18, 21, 63, 68, 70, 78, 92};
//   vector<int> sort = PrlQuickSort(arr, arr.size());
//   if (rank == 0) {
//       vector<int> seq_sort = SeqQuickSort(arr, 0, arr.size()-1);
//       ASSERT_EQ(seq_sort, sort);
//       ASSERT_EQ(seq_sort, sorted_arr);
//   }
// }



// TEST(Q_BATCHER_SORT, sort_big_arr_2) {
//   int rank;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//   vector<int> arr = {
//       76, 74, 64, 40, 18, 3, 86, 7, 36, 83, 87,
//       11, 92, 83, 64, 41, 68, 14, 44, 40, 69, 34,
//       36, 44, 88, 54, 99, 73, 94, 24, 32, 70, 97,
//       96, 9, 15, 98, 94, 21, 34, 77, 8, 45, 68, 90,
//       8, 9};
//   vector<int> sorted_arr = {
//       3, 7, 8, 8, 9, 9, 11, 14, 15, 18, 21, 24, 32, 34,
//       34, 36, 36, 40, 40, 41, 44, 44, 45, 54, 64, 64, 68,
//       68, 69, 70, 73, 74, 76, 77, 83, 83, 86, 87, 88, 90,
//       92, 94, 94, 96, 97, 98, 99};

//   vector<int> sort = PrlQuickSort(arr, arr.size());
//   if (rank == 0) {
//       vector<int> seq_sort = SeqQuickSort(arr, 0, arr.size()-1);
//       ASSERT_EQ(seq_sort, sort);
//       ASSERT_EQ(seq_sort, sorted_arr);
//   }
// }


// TEST(Q_BATCHER_SORT, sort_big_arr_3) {
//   int rank;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//   vector<int> arr = {
//       19, 46, 77, 38, 65, 76, 42, 77, 6, 72, 32, 36, 93, 84, 66,
//       50, 62, 57, 17, 22, 62, 79, 43, 60, 17, 10, 23, 37, 87, 30,
//       47, 5, 75, 23, 42, 40, 99, 83, 17, 5, 55, 48, 40, 48, 32, 6,
//       97, 93, 63, 14, 14, 24, 92, 57, 84, 9, 66, 7, 45, 52, 36, 92,
//       56, 11, 14, 98, 50, 12, 81, 66, 17, 35, 13, 56, 83, 45, 62,
//       79, 38, 25, 92, 52, 48, 84, 8, 32, 93, 73, 38, 38, 24, 74,
//       29, 80, 84, 43, 77, 33, 55, 58, 98, 71, 92, 11, 27, 75, 55,
//       88, 53, 92, 12};
//   vector<int> sorted_arr = {
//       5, 5, 6, 6, 7, 8, 9, 10, 11, 11, 12, 12, 13, 14, 14, 14,
//       17, 17, 17, 17, 19, 22, 23, 23, 24, 24, 25, 27, 29, 30, 32,
//       32, 32, 33, 35, 36, 36, 37, 38, 38, 38, 38, 40, 40, 42, 42, 43,
//       43, 45, 45, 46, 47, 48, 48, 48, 50, 50, 52, 52, 53, 55, 55, 55,
//       56, 56, 57, 57, 58, 60, 62, 62, 62, 63, 65, 66, 66, 66, 71, 72,
//       73, 74, 75, 75, 76, 77, 77, 77, 79, 79, 80, 81, 83, 83, 84, 84,
//       84, 84, 87, 88, 92, 92, 92, 92, 92, 93, 93, 93, 97, 98, 98, 99};

//   vector<int> sort = PrlQuickSort(arr, arr.size());
//   if (rank == 0) {
//       vector<int> seq_sort = SeqQuickSort(arr, 0, arr.size()-1);
//       ASSERT_EQ(seq_sort, sort);
//       ASSERT_EQ(seq_sort, sorted_arr);
//   }
// }

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
