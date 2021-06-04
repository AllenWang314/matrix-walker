from math import log2, ceil
from itertools import chain
from cache import Cache
import random

class MatrixWalker:
    def _next_pow_2(self, n):
        return int(2**(ceil(log2(n))))

    def _matrix_to_array(self, mat):
        arr = [-1] * (self.n_pw2**2)
        for i in range(len(mat)):
            for j in range(len(mat)):
                arr[self.translate(i,j)] = mat[i][j]
        return arr
    
    def __init__(self, mat, cache_width, cache_height):
        self.mat = mat
        self.size = len(mat)
        self.n_pw2 = self._next_pow_2(len(mat))
        self.arr = self._matrix_to_array(mat)
        self.i = random.randrange(len(mat))
        self.j = random.randrange(len(mat))
        self.loc = 0 # location in flattened array
        self.val = self.arr[self.translate(self.i, self.j)]
        self.cache = Cache(cache_width, cache_height)

    @classmethod
    # alternative constructor, analogous to NumPy
    def zeros(cls, size):
        mat = [[0]*size] * size
        return cls(mat)
            
    def _move(self, i, j):
        if i< 0 or i>=self.size or j<0 or j>=self.size:
            raise IndexError('attempted to move out of bounds')
        self.i = i
        self.j = j
        self.loc = self.translate(i, j)
        self.val = self.arr[self.loc]
        self.cache.access(self.get_cache_index(i, j))

    def random_teleport(self):
        self._move(random.randrange(len(mat)), random.randrange(len(mat)))

    def teleport(self, i, j):
        self._move(i,j)

    def left(self):
        self._move(self.i, self.j - 1)
    
    def right(self):
        self._move(self.i, self.j + 1)

    def up(self):
        self._move(self.i - 1, self.j)
    
    def down(self):
        self._move(self.i + 1, self.j)

    def get(self):
        return self.val

    def __setitem__(self, index, val):
        i = index[0]
        j = index[1]
        self.arr[self.translate(i, j)] = val

    def get_cache_index(self, i, j):
        return self.translate(i, j) // self.cache.width

class ZWalker(MatrixWalker):
    def translate(self, i, j):
        bin_i = bin(i+self.n_pw2)[3:] # ensure correct length of bin repr
        bin_j = bin(j+self.n_pw2)[3:]
        interleaved = ''.join(chain(*zip(bin_i, bin_j)))
        return int(interleaved, 2)

class HilbertWalker(MatrixWalker):
    def translate(self, i, j):
        # recurse into quadrants (done iteratively here)
        base_case = [0, 1, 3, 2]
        ret = 0
        for pow in range(int(log2(self.n_pw2))-1, -1, -1):
            mask = 2**pow
            quadrant = ((i & mask == mask) << 1) + (j & mask == mask)
            ret += base_case[quadrant]
            i &= mask - 1
            j &= mask - 1
            if quadrant == 0: # Flip next layer depending on quadrant
                i, j = j, i
            elif quadrant == 2:
                i, j = mask - 1 - j, mask - 1 - i

            if pow > 0: # Make room for next recursive layer
                ret <<= 2
        return ret

class NaiveWalker(MatrixWalker):
    def translate(self, i, j):
        return self.size * i + j


if __name__=="__main__":
    small = [[0, 1, 2, 3], [4, 5, 6, 7], [8, 9, 10, 11], [12, 13, 14, 15]]
    # mw = NaiveWalker(small, 8, 128)
    hw = HilbertWalker(small, 8, 128)
    # zw = ZWalker(small, 8, 128)
    # print("Arrays of size 4")
    # print(f"Naive:\n{mw.arr}\nHilbert:\n{hw.arr}\nZ:\n{zw.arr}")
    print("#####")
    print(hw.translate(0,1))
    # print(hw.arr)