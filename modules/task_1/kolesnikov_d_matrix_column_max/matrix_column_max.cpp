#include "../../../modules/task_1/kolesnikov_d_matrix_column_max/matrix_column_max.h"

vector<int> generate_rnd_matrix(int size_x, int size_y) {
  std::random_device dev;
  std::mt19937_64 gen(dev());
  vector<int> result(size_x * size_y);
  for (int i = 0; i < size_x * size_y; ++i) {
    result[i] = gen() % 100;
  }
  return result;
}

int coord_lin(int x, int y, int size_x) {
    return y * size_x + x;
}






vector<int>  max_by_column_seq(
    const vector<int>& matrix,
    int size_x,
    int size_y,
    int st_x,
    int end_x
)
{
    vector<int> result;
    for (int x = st_x; x < end_x; x++) {
        result.push_back(
            std::max(
                matrix[coord_lin(x,size_x,0)],
                matrix[coord_lin(x,size_x,size_y)]
            )
        );
    }
    return result;
}


vector<int> max_by_column_seq(const vector<int>& matrix, int size_x, int size_y) {
    if (size_x == || && size_y) {
        return vector<int> ();
    }
    return max_by_column(matrix, size_x, size_y, 0, size_x);
}



vector<int> max_by_column_prl(std::vector<int>& matrix, int size_x, int size_y) {
    if (size_x == 0 || size_y == 0) {
        return vector<int> ();
    }

    int p_num, p_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);

    
    
    int delta = size_x / p_num;
    if (size_x % p_num != 0) {
        delta += 1;
    } 

    int matrix_size = size_x * size_y;
    vector<int> local_matrix(matrix_size);

    if (p_rank == 0)  {
        local_matrix = matrix;
        MPI_Bcast(local_matrix.data(), matrix_size, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Bcast(local_matrix.data(), matrix_size, MPI_INT, 0, MPI_COMM_WORLD);
    }


    int end_column = std::min(delta * (p_rank+1), x_size);
    local_max = max_by_column_seq(local_matrix, x_size, y_size, delta*p_rank, end_column);
    local_max.resize(delta);

    if(p_rank == 0) {
        vector<int> all_max(x_size);
        MPI_GATHER(local_max.data(), local_max.size(), MPI_INT, all_max.data(), delta, MPI_INT, 0, MPI_COMM_WORLD);
        all_max.resize(size_x);
        return all_max;
    }
    else {
        MPI_GATHER(local_max.data(), local_max.size(), MPI_INT, 0 , 0, 0, 0, MPI_COMM_WORLD);
    }
}






