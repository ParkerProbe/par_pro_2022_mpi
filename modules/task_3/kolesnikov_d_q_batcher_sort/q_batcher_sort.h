// Copyright 2022 Kolesnikov Denis
#ifndef MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT_Q_BATCHER_SORT_H_
#define MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT_Q_BATCHER_SORT_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <random>
#include <iostream>
#include <cstring>

using std::swap;
int GenRndNum();
int* MergeArr(int* a, int* b);
void SeqQuickSort(int* data, int l, int r);
void PrlQuickSort(int* data, int count);
void BatcherMerge(int* a, int l, int n, int r);
void Compare(int* a, int i, int j);
#endif  // MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT_Q_BATCHER_SORT_H_
