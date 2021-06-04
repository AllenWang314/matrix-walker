#include <iostream>
#include "matrix_walker.h"

int matrix_walker::size() {return 0;}
void matrix_walker::teleport(int i, int j) {}
void matrix_walker::move_left() {}
void matrix_walker::move_right() {}
void matrix_walker::move_up() {}
void matrix_walker::move_down() {}
void matrix_walker::set(int i, int j, int value) {}
void matrix_walker::set_default() {}

int next_power_of_2(unsigned int n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n; 
}

naive_matrix_walker:: naive_matrix_walker(int n) {
    this->n = n;
    this->arr = (int*) malloc(n * n * sizeof(int));
}

int naive_matrix_walker::size() {
    return this->n;
}

int naive_matrix_walker::translate(int i, int j) {
    return i * this->n + j;
}

void naive_matrix_walker::teleport(int i, int j) {
    this->i = i;
    this->j = j;
    this->value = *(this->arr + this->translate(this->i, this->j));
}

void naive_matrix_walker::move_left() {
    if(this->j == 0)
        return;
    this->j = this->j - 1;
    this->value = *(this->arr + this->translate(this->i, this->j));
}
void naive_matrix_walker::move_right() {
    if(this->j == this->n-1)
        return;
    this->j = this->j + 1;
    this->value = *(this->arr + this->translate(this->i, this->j));
}
void naive_matrix_walker::move_up() {
    if(this->i == 0)
        return;
    this->i = this->i - 1;
    this->value = *(this->arr + this->translate(this->i, this->j));
}
void naive_matrix_walker::move_down() {
    if(this->i == this->n - 1)
        return;
    this->i = this->i + 1;
    this->value = *(this->arr + this->translate(this->i, this->j));
}

int naive_matrix_walker::get() {
    return this->value;
}

void naive_matrix_walker::set(int i, int j, int value) {
    *(this->arr + this->translate(i, j)) = value;
}

void naive_matrix_walker::set_default() {
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->n; j++) {
            this->set(i, j, i * this->n + j);
        }
    }
}


z_matrix_walker:: z_matrix_walker(int n) {
    this->n = n;
    this->n_pw2 = next_power_of_2(n);
    this->arr = (int *)malloc(this->n_pw2 * this->n_pw2 * sizeof(int));
    this->i = 0;
    this->j = 0;
    this->z_value = 0;
    this->value = 0;
    this->even_bits = 0;
    long long size = this->n_pw2 * this->n_pw2;
    for (long long i = 1; i < size; i <<= 2)
    {
        this->even_bits |= i;
    }
    this->odd_bits = 0;
    for (long long i = 2; i < size; i <<= 2) {
        this->odd_bits |= i;
    }
}

int z_matrix_walker::size() {
    return this->n;
}

uint64_t interleave_uint32_with_zeros(uint32_t input)  {
    uint64_t word = input;
    word = (word ^ (word << 16)) & 0x0000ffff0000ffff;
    word = (word ^ (word << 8 )) & 0x00ff00ff00ff00ff;
    word = (word ^ (word << 4 )) & 0x0f0f0f0f0f0f0f0f;
    word = (word ^ (word << 2 )) & 0x3333333333333333;
    word = (word ^ (word << 1 )) & 0x5555555555555555;
    return word;
}

int interleave(int x, int y) {
    return interleave_uint32_with_zeros(x) 
  | (interleave_uint32_with_zeros(y) << 1);
}

int z_matrix_walker::translate(int i, int j) {
    return interleave(j, i);
}

void z_matrix_walker::teleport(int i, int j) {
    this->i = i;
    this->j = j;
    this->z_value = this->translate(this->i, this->j);
    this->value = *(this->arr + this->z_value);
}

void z_matrix_walker::move_left() {
    if(this->j == 0)
        return;
    this->j = this->j - 1;
    this->z_value = (((this->z_value & this->even_bits) - 1) & this->even_bits) | (this->z_value & this->odd_bits);
    this->value = *(this->arr + this->z_value);
}
void z_matrix_walker::move_right() {
    if(this->j == this->n - 1)
        return;
    this->j = this->j + 1;
    this->z_value = (((this->z_value | this->odd_bits) + 1) & this->even_bits) | (this->z_value & this->odd_bits);
    this->value = *(this->arr + this->z_value);
}
void z_matrix_walker::move_up() {
    if(this->i == 0)
        return;
    this->i = this->i - 1;
    this->z_value = (((this->z_value & this->odd_bits) - 1) & this->odd_bits) | (this->z_value & this->even_bits);
    this->value = *(this->arr + this->z_value);
}
void z_matrix_walker::move_down() {
    if(this->i == this->n - 1)
        return;
    this->i = this->i + 1;
    this->z_value = (((this->z_value | this->even_bits) + 1) & this->odd_bits) | (this->z_value & this->even_bits);
    this->value = *(this->arr + this->z_value);
}

int z_matrix_walker::get() {
    return this->value;
}

void z_matrix_walker::set(int i, int j, int value) {
    *(this->arr + this->translate(i, j)) = value;
}

void z_matrix_walker::set_default() {
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->n; j++) {
            this->set(i, j, i * this->n + j);
        }
    }
}


hilbert_matrix_walker:: hilbert_matrix_walker(int n) {
    this->n = n;
    this->n_pw2 = next_power_of_2(n);
    this->arr = (int *)malloc(this->n_pw2 * this->n_pw2 * sizeof(int));
    this->i = 0;
    this->j = 0;
    this->h_value = 0;
    this->value = 0;
}

int hilbert_matrix_walker::size() {
    return this->n;
}

int hilbert_matrix_walker::translate(int i, int j) {
    int ret = 0;
    for (int mask = this->n_pw2 / 2; mask >= 1; mask /= 2){
        int quadrant = (((i & mask) == mask) << 1) + ((j & mask) == mask);
        i &= mask - 1;
        j &= mask - 1;
        if (quadrant == 0){
            int tmp = i;
            i = j;
            j = tmp;
        }
        else if (quadrant == 1){
            ret += 1;
        }
        else if (quadrant == 2){
            int tmp = i;
            i = mask - 1 - j;
            j = mask - 1 - tmp;
            ret += 3;
        }
        else {
            ret += 2;
        }
        if (mask > 1){
            ret <<= 2;
        }
    }
    return ret;
}

void hilbert_matrix_walker::move(int i, int j) {
    if(i < 0 || i >= this->n || j < 0 || j >= this->n){
        return;
    }
    this->i = i;
    this->j = j;
    this->h_value = this->translate(this->i, this->j);
    this->value = *(this->arr + this->h_value);
}

void hilbert_matrix_walker::teleport(int i, int j) {
    this->move(i,j);
}

void hilbert_matrix_walker::move_left() {
    this->move(this->i, this->j - 1);
}
void hilbert_matrix_walker::move_right() {
    this->move(this->i, this->j + 1);
}
void hilbert_matrix_walker::move_up() {
    this->move(this->i - 1, this->j);
}
void hilbert_matrix_walker::move_down() {
    this->move(this->i + 1, this->j);
}

int hilbert_matrix_walker::get() {
    return this->value;
}

void hilbert_matrix_walker::set(int i, int j, int value) {
    *(this->arr + this->translate(i, j)) = value;
}

void hilbert_matrix_walker::set_default() {
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->n; j++) {
            this->set(i, j, i * this->n + j);
        }
    }
}

void hilbert_matrix_walker::print() {
    for (int i = 0; i < (this->n_pw2 * this->n_pw2); i++){
        std::cout << this->arr[i] << " ";
    }
    std::cout << std::endl;
}