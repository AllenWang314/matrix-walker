#include "tensor_walker.h"
#include "stdio.h"


int ans(int i, int j, int k, int n) {
    return i*n*n + j*n + k;
}

int verify(int i, int j, int k, int n, int res) {
    if (res != ans(i, j, k, n)) {
        fprintf(stderr, "\nFailed test at index (%d, %d, %d); got %d instead of %d\n", 
                                                i, j, k, res, ans(i, j, k, n));
        return false;
    }
    return true;
}

template <typename T> 
int test_move(int n) {
    fprintf(stderr, "\nTesting move with n=%d...", n);
    T *mat = new T(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                mat->set(i, j, k, ans(i, j, k, n));
            }
        }
    }
    
    for (int i = 1; i < n; i++) {
        mat->move_out();
        if (!verify(i, 0, 0, n, mat->get())) return -1;
    }
    mat->move_down();
    if (!verify(n-1, 1, 0, n, mat->get())) return -1;
    mat->move_right();
    if (!verify(n-1, 1, 1, n, mat->get())) return -1;
    mat->move_in();
    if (!verify(n-2, 1, 1, n, mat->get())) return -1;
    mat->move_left();
    if (!verify(n-2, 1, 0, n, mat->get())) return -1;
    mat->move_up();
    if (!verify(n-2, 0, 0, n, mat->get())) return -1;
    for (int k = 1; k < n; k++) {
        mat->move_right();
        if (!verify(n-2, 0, k, n, mat->get())) return -1;
    }
    fprintf(stderr, "\nsuccess");
    return 0;
}

template <typename T>
int test_teleport(int n) {
    fprintf(stderr, "\nTesting teleport with n=%d...", n);
    T *mat = new T(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                mat->set(i, j, k, ans(i, j, k, n));
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                mat->teleport(i, j, k);
                if (!verify(i, j, k, n, mat->get())) return -1;
            }
        }
    }
    fprintf(stderr, "\nsuccess");
    return 0;
}

int main(){
    fprintf(stderr, "\n\nNaive Implementation");
    test_move<naive_tensor_walker>(4);
    test_move<naive_tensor_walker>(200);
    test_teleport<naive_tensor_walker>(4);
    test_teleport<naive_tensor_walker>(200);

    fprintf(stderr, "\n\nZ Implementation");
    test_move<z_tensor_walker>(4);
    test_move<z_tensor_walker>(200);
    test_teleport<z_tensor_walker>(4);
    test_teleport<z_tensor_walker>(200);

    fprintf(stderr, "\n\nHilbert Implementation");
    test_move<hilbert_tensor_walker>(4);
    test_move<hilbert_tensor_walker>(200);
    test_teleport<hilbert_tensor_walker>(4);
    test_teleport<hilbert_tensor_walker>(200);
    return 0;
}