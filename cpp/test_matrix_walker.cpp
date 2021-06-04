#include "matrix_walker.h"
#include "stdio.h"

int verify(int i, int j, int res, int expected) {
    if (res != expected) {
        fprintf(stderr, "Failed test at index (%d, %d) got %d instead of %d\n", i, j, res, expected);
        return -1;
    }
    return 0;
}

template <typename T>
int test_move(int n) {
    T *matrix = new T(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) {
            matrix->set(i, j, i*n+j);
        }
    }

    matrix->teleport(n-1, n-1);
    if (matrix->get() != n*n - 1) {
        fprintf(stderr, "Failed test_move: teleport\n");
        return -1;
    }

    matrix->move_up();
    if (matrix->get() != n*n - n - 1) {
        fprintf(stderr, "Failed move up\n");
        return -1;
    }

    matrix->move_left();
    if (matrix->get() !=  n*n - n - 2) {
        fprintf(stderr, "Failed move left\n");
        return -1;
    }

    matrix->move_down();
    if (matrix->get() !=  n*n - 2) {
        fprintf(stderr, "Failed move down\n");
        return -1;
    }

    matrix->move_right();
    if (matrix->get() !=  n*n - 1) {
        fprintf(stderr, "Failed move right\n");
        return -1;
    }

    fprintf(stderr, "Success move test\n");
    return 0;
}

template <typename T>
int test_teleport(int n) {
    T *matrix = new T(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) {
            matrix->set(i, j, i*n+j);
        }
    }

    for (int i = n-1; i >= 0 ; i--){
        for (int j = 0; j < n; j++) {
            matrix->teleport(i, j);
            if (verify(i, j, matrix->get(), i*n+j)) {
                return -1;
            }
        }
    }
    fprintf(stderr, "Success teleport test\n");
    return 0;
}

int main() {
    fprintf(stderr, "Test Naive Matrix Walker Implementation\n");
    test_teleport<naive_matrix_walker>(100);
    test_teleport<naive_matrix_walker>(500);
    test_teleport<naive_matrix_walker>(1500);
    test_move<naive_matrix_walker>(100);
    test_move<naive_matrix_walker>(500);
    test_move<naive_matrix_walker>(1500);

    fprintf(stderr, "Test Z-Order Matrix Walker Implementation\n");
    test_teleport<z_matrix_walker>(100);
    test_teleport<z_matrix_walker>(500);
    test_teleport<z_matrix_walker>(1500);
    test_move<z_matrix_walker>(100);
    test_move<z_matrix_walker>(500);
    test_move<z_matrix_walker>(1500);

    fprintf(stderr, "Test Hilbert-Order Matrix Walker Implementation\n");
    test_teleport<hilbert_matrix_walker>(4);
    test_teleport<hilbert_matrix_walker>(500);
    test_teleport<hilbert_matrix_walker>(1500);
    test_move<hilbert_matrix_walker>(4);
    test_move<hilbert_matrix_walker>(500);
    test_move<hilbert_matrix_walker>(1500);
    return 0;
}