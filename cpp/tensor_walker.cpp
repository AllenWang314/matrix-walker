#include <iostream>
#include "tensor_walker.h"

int tensor_walker::size() {return 0;}
void tensor_walker::teleport(int i, int j, int k) {}
void tensor_walker::move_left() {}
void tensor_walker::move_right() {}
void tensor_walker::move_up() {}
void tensor_walker::move_down() {}
void tensor_walker::move_in() {}
void tensor_walker::move_out() {}
void tensor_walker::set(int i, int j, int k, int value){}
void tensor_walker::set_default() {}

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

naive_tensor_walker:: naive_tensor_walker(int n) {
    this->n = n;
    this->arr = (int*) malloc(n*n*n * sizeof(int));
    this->i = 0;
    this->j = 0;
    this->k = 0;
    this->value = 0;
}

int naive_tensor_walker::size() {
    return this->n;
}

int naive_tensor_walker::translate(int i, int j, int k) {
    return i * this->n * this->n + j * this-> n + k;
}

void naive_tensor_walker::teleport(int i, int j, int k) {
    this->i = i;
    this->j = j;
    this->k = k;
    this->value = *(this->arr + this->translate(this->i, this->j, this->k));
}

void naive_tensor_walker::move_left() {
    if(this->k == 0)
        return;
    this->k = this->k - 1;
    this->value = *(this->arr + this->translate(this->i, this->j, this->k));
}
void naive_tensor_walker::move_right() {
    if(this->k == this->n-1)
        return;
    this->k = this->k + 1;
    this->value = *(this->arr + this->translate(this->i, this->j, this->k));
}
void naive_tensor_walker::move_up() {
    if(this->j == 0)
        return;
    this->j = this->j - 1;
    this->value = *(this->arr + this->translate(this->i, this->j, this->k));
}
void naive_tensor_walker::move_down() {
    if(this->j == this->n - 1)
        return;
    this->j = this->j + 1;
    this->value = *(this->arr + this->translate(this->i, this->j, this->k));
}
void naive_tensor_walker::move_in(){
    if(this->i == 0)
        return;
    this->i = this->i - 1;
    this-> value = *(this->arr + this->translate(this->i, this->j, this->k));
}
void naive_tensor_walker::move_out(){
    if(this->i == this->n-1)
        return;
    this->i = this->i + 1;
    this-> value = *(this->arr + this->translate(this->i, this->j, this->k));
}

int naive_tensor_walker::get() {
    return this->value;
}

void naive_tensor_walker::set(int i, int j, int k, int value) {
    *(this->arr + this->translate(i, j, k)) = value;
}

void naive_tensor_walker::set_default() {
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->n; j++) {
            for (int k = 0; k < this->n; k++) {
                this->set(i, j, k, i * this->n * this->n + j * this->n + k);
            }
        }
    }
}

void naive_tensor_walker::print() {
    int n = this->n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                fprintf(stderr, "%d ", *(this->arr + i*n*n + j*n + k));
            }
        }
    }
}

z_tensor_walker:: z_tensor_walker(int n) {
    this->n = n;
    this->n_pw2 = next_power_of_2(n);
    this->i = 0;
    this->j = 0;
    this->k = 0;
    this->i_bits = 0;
    this->j_bits = 0;
    this->k_bits = 0;
    this->arr = (int *)malloc(this->n_pw2 * this->n_pw2 * this->n_pw2 * sizeof(int));
    this->z_value = 0;
    this->value = 0;
    long long size = this->n_pw2 * this->n_pw2 * this->n_pw2;
    for (long long i = 1; i < size; i <<= 3) {
        this->i_bits |= (i << 2);
        this->j_bits |= (i << 1);
        this->k_bits |= i;
    }
}

int z_tensor_walker::size() {
    return this->n;
}

uint64_t interleave_uint16_with_zeros(uint16_t input)  {
    // abcd -> a00b00c00d
    uint64_t word = input;
    word = (word ^ (word << 16)) & 0x00000000ff0000ff;
    word = (word ^ (word << 8 )) & 0x000000f00f00f00f;
    word = (word ^ (word << 4 )) & 0x00000c30c30c30c3;
    word = (word ^ (word << 2 )) & 0x0000249249249249;
    return word;
}

int interleave(int x, int y, int z) {
    return interleave_uint16_with_zeros(z) 
  | (interleave_uint16_with_zeros(y) << 1)
  | (interleave_uint16_with_zeros(x) << 2);
}

int z_tensor_walker::translate(int i, int j, int k) {
    return interleave(i, j, k);
}

void z_tensor_walker::teleport(int i, int j, int k) {
    this->i = i;
    this->j = j;
    this->k = k;
    this->z_value = this->translate(this->i, this->j, this->k);
    this->value = *(this->arr + this->z_value);
}

void z_tensor_walker::move_left() {
    if(this->k == 0)
        return;
    this->k = this->k - 1;
    this->z_value = (((this->z_value & this->k_bits) - 1) & this->k_bits) | 
                    (this->z_value & (this->i_bits | this->j_bits));
    this->value = *(this->arr + this->z_value);
}
void z_tensor_walker::move_right() {
    if(this->k == this->n - 1)
        return;
    this->k = this->k + 1;
    this->z_value = (((this->z_value | this->i_bits | this->j_bits) + 1) & this->k_bits) | 
                    (this->z_value & (this->i_bits | this->j_bits));
    this->value = *(this->arr + this->z_value);
}
void z_tensor_walker::move_up() {
    if(this->j == 0)
        return;
    this->j = this->j - 1;
    this->z_value = (((this->z_value & this->j_bits) - 1) & this->j_bits) | 
                    (this->z_value & (this->k_bits | this->i_bits));
    this->value = *(this->arr + this->z_value);
}
void z_tensor_walker::move_down() {
    if(this->j == this->n - 1)
        return;
    this->j = this->j + 1;
    this->z_value = (((this->z_value | this->k_bits | this->i_bits) + 1) & this->j_bits) | 
                    (this->z_value & (this->k_bits | this->i_bits));
    this->value = *(this->arr + this->z_value);
}
void z_tensor_walker::move_in() {
    if(this->i == 0)
        return;
    this->i = this->i - 1;
    this->z_value = (((this->z_value & this->i_bits) - 1) & this->i_bits) | 
                    (this->z_value & (this->j_bits | this->k_bits));
    this->value = *(this->arr + this->z_value);
}
void z_tensor_walker::move_out() {
    if(this->i == this->n - 1)
        return;
    this->i = this->i + 1;
    this->z_value = (((this->z_value | this->j_bits | this->k_bits) + 1) & this->i_bits) | 
                    (this->z_value & (this->j_bits | this->k_bits));
    this->value = *(this->arr + this->z_value);
}

int z_tensor_walker::get() {
    return this->value;
}

void z_tensor_walker::set(int i, int j, int k, int value) {
    *(this->arr + this->translate(i, j, k)) = value;
}

void z_tensor_walker::set_default() {
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->n; j++) {
            for (int k = 0; k < this->n; k++) {
                this->set(i, j, k, i * this->n * this->n + j * this->n + k);
            }
        }
    }
}

void z_tensor_walker::print() {
    int n = this->n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                fprintf(stderr, "%d ", *(this->arr + i*n*n + j*n + k));
            }
        }
    }
}

hilbert_tensor_walker:: hilbert_tensor_walker(int n) {
    this->n = n;
    this->n_pw2 = next_power_of_2(n);
    this->arr = (int *)malloc(this->n_pw2 * this->n_pw2 * this->n_pw2 * sizeof(int));
    this->i = 0;
    this->j = 0;
    this->k = 0;
    this->h_value = 0;
    this->value = 0;
}

int hilbert_tensor_walker::size() {
    return this->n;
}

int hilbert_tensor_walker::translate(int i, int j, int k) {
    int ret = 0;
    for (int mask = this->n_pw2 / 2; mask >= 1; mask /= 2){
        int octant = (((i & mask) == mask) << 2) + 
                     (((j & mask) == mask) << 1) + 
                      ((k & mask) == mask);
        i &= mask - 1;
        j &= mask - 1;
        k &= mask - 1;
        switch (octant) {
            case 0: {
                int tmp = i;
                i = k;
                k = tmp;
                break;
            }
            case 1: {
                ret += 1;
                int tmp = i;
                i = j;
                j = tmp;
                break;
            }
            case 2: {
                ret += 3;
                int tmp = j;
                j = mask - 1 - k;
                k = mask - 1 - tmp;
                break;
            }
            case 3: {
                ret += 2;
                int tmp = i;
                i = j;
                j = tmp;
                break;

            }
            case 4: {
                ret += 7;
                int tmp = i;
                i = mask - 1 - k;
                k = mask - 1 - tmp;
                break;

            }
            case 5: {
                ret += 6;
                int tmp = i;
                i = mask - 1 - j;
                j = mask - 1 - tmp;
                break;

            }
            case 6: {
                ret += 4;
                int tmp = j;
                j = mask - 1 - k;
                k = mask - 1 - tmp;
                break;

            }
            case 7: {
                ret += 5;
                int tmp = i;
                i = mask - 1 - j;
                j = mask - 1 - tmp;
                break;

            }
        }
        if (mask > 1){
            ret <<= 3;
        }
    }
    return ret;
}

void hilbert_tensor_walker::move(int i, int j, int k) {
    if(i < 0 || i >= this->n || j < 0 || j >= this->n || k < 0 || k >= this->n){
        return;
    }
    this->i = i;
    this->j = j;
    this->k = k;
    this->h_value = this->translate(this->i, this->j, this->k);
    this->value = *(this->arr + this->h_value);
}

void hilbert_tensor_walker::teleport(int i, int j, int k) {
    this->move(i,j,k);
}

void hilbert_tensor_walker::move_left() {
    this->move(this->i, this->j, this->k - 1);
}
void hilbert_tensor_walker::move_right() {
    this->move(this->i, this->j, this->k + 1);
}
void hilbert_tensor_walker::move_up() {
    this->move(this->i, this->j - 1, this->k);
}
void hilbert_tensor_walker::move_down() {
    this->move(this->i, this->j + 1, this->k);
}
void hilbert_tensor_walker::move_in() {
    this->move(this->i - 1, this->j, this->k);
}
void hilbert_tensor_walker::move_out() {
    this->move(this->i + 1, this->j, this->k);
}

int hilbert_tensor_walker::get() {
    return this->value;
}

void hilbert_tensor_walker::set(int i, int j, int k, int value) {
    *(this->arr + this->translate(i, j, k)) = value;
}

void hilbert_tensor_walker::set_default() {
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->n; j++) {
            for (int k = 0; k < this->n; k++) {
                this->set(i, j, k, i * this->n * this->n + j * this->n + k);
            }
        }
    }
}

void hilbert_tensor_walker::print() {
    int n = this->n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                fprintf(stderr, "%d ", *(this->arr + i*n*n + j*n + k));
            }
        }
    }
}