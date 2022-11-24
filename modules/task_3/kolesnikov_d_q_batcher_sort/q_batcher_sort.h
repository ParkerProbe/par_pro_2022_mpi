#ifndef MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT
#define MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT

#include <vector>
#include <mpi.h>
#include <random>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using std::vector;
using std::swap;



void SeqQuickSort(vector<int>& data, int l, int r);
void PrlQuickSort(vector<int>& data, int count);
void BatcherMerge(vector<int>& a, int l, int n, int r);
void Compare(vector<int>& a, int i, int j);
#endif // MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT