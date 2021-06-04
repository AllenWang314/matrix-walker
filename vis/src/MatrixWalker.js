
/**
 * 
 * @param {maximum random int exclusive} max 
 * @returns a random integer from 0 to max exclusive
 */
function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}

/**
 * 
 * @param {*} n 
 * @returns the next power of 2 greater than or equal to n
 */
function next_pow_2(n) {
    return Math.floor(2**(Math.ceil(Math.log2(n))))
}

/**
 * 
 * @param {*} input 
 * @returns interleaving the binary representation of input with 0
 */
function interleave_uint16_with_zeros(input) {
    var word = input
    word = (word ^ (word << 16)) & 0x0000ffff
    word = (word ^ (word << 8 )) & 0x00ff00ff
    word = (word ^ (word << 4 )) & 0x0f0f0f0f
    word = (word ^ (word << 2 )) & 0x33333333
    word = (word ^ (word << 1 )) & 0x55555555
    return word
}

/**
 * interleaves two uint16 numbers x and y together by 
 * using every other digit from their binary representations
 * @param {*} x 
 * @param {*} y 
 * @returns result of interleaving
 */
function interleave(x, y) {
    return interleave_uint16_with_zeros(x) 
  | (interleave_uint16_with_zeros(y) << 1);
}

/**
 * implmentation of abstract matrix walker class
 */
class MatrixWalker {

    /**
     * constructus Matrix Walker with initial pointer at 0,0
     * creates a new cache associated witht the Matrix Walker with width cache_width and height cache_height
     * @param {side length of the matrix} matrix_size 
     * @param {width of the cache} cache_width 
     * @param {height of the cache} cache_height 
     */
    constructor(matrix_size, cache_width, cache_height) {
        this.size = matrix_size
        this.n_pw2 = next_pow_2(matrix_size)
        this.i = 0
        this.j = 0
        this.loc = 0
        this.cache = new Cache(cache_width, cache_height)
        this.move(0,0)
    }

    // moves the pointer to the entry on the ith row and jth column
    move(i, j) {
        if (i < 0 || i >= this.size || j < 0 || j >= this.size){
            console.log("attempted to move out of bounds")
        } else {
            this.i = i
            this.j = j
            this.loc = this.translate(i, j)
            this.cache.access(this.get_cache_index(i, j)) // accesses the index of the cache block
        }
    }

    // randomly teleport to a square in the matrix
    random_teleport(){
        this.move(getRandomInt(this.size), getRandomInt(this.size))
    }

    // teleport to square in the ith row and jth column
    teleport(i, j){
        this.move(i,j)
    }

    // moves pointer to the left
    left() {
        this.move(this.i, this.j - 1)
    }

    // moves pointer to the right
    right() {
        this.move(this.i, this.j + 1)
    }

    // moves pointer up
    up() {
        this.move(this.i - 1, this.j)
    }

    // moves pointer down
    down() {
        this.move(this.i + 1, this.j)
    }

    // gets the index of the cache block for square in the ith row jth column
    get_cache_index(i, j) {
        return Math.floor(this.translate(i, j) / this.cache.width)
    }

    // gets all the indices of squares that reside in the cache
    get_indices_in_cache() {
        var indices = []
        for (var i = 0; i < this.cache.lru.length; i++) {
            for (var j = 0; j < this.cache.width; j++) {
                var coords = this.reverse_translate(this.cache.lru[i] * this.cache.width + j)
                indices.push(coords[0] * this.size + coords[1])
            }
        }
        return indices
    }

    /*
    //gets the desired array to be displayed in App.js
    // all entries in the cache are set to 0.5
    // the current entry whose pointer is store is set to 0
    */
    get_cache_visual() {
        var arr = Array(this.size * this.size).fill(1)
        var indices = this.get_indices_in_cache();
        for (var i = 0; i < indices.length; i++) {
            arr[indices[i]] = 0.5
        }
        arr[this.i * this.size + this.j] = 0
        return arr
    }

    /**
     * 
     * returns a simple div summarizing cache hits and misses
     */
    get_cache_stats() {
        if (this.cache.cache_accesses === 0) {
            return (
                <div>
                    <div>
                        Cache of width {this.cache.width} and height {this.cache.height}
                    </div>
                    <div>
                        Total of {this.cache.cache_accesses} cache accesses and {this.cache.cache_hits} cache hits
                    </div>
                    <div>
                        Cache hit percentage: N/A
                    </div>
                </div>
            )
        }
        return (
            <div>
                <div>
                    Cache of width {this.cache.width} and height {this.cache.height}
                </div>
                <div>
                    Total of {this.cache.cache_accesses} cache accesses and {this.cache.cache_hits} cache hits
                </div>
                <div>
                    Cache hit percentage: {this.cache.cache_hits/this.cache.cache_accesses}
                </div>
            </div>
        )
    }
}

/**
 * implementation of naive row major order matrix walker
 */
class NaiveWalker extends MatrixWalker {

    // translate method translates from row i and col j location to the index in the underlying stored array
    translate(i, j) {
        return this.size * i + j
    }

    // the inverse of translate
    reverse_translate(n) {
        var i = Math.floor(n / this.size)
        return [i, n - this.size * i]
    }
}

/**
 * implementation of z order matrix walker
 */
class ZWalker extends MatrixWalker {

    // translate method translates from row i and col j location to the index in the underlying stored array
    translate(i, j) {
        return interleave(j, i);
    }

    // the inverse of translate
    reverse_translate(n) {
        var n_str = (n).toString(2)
        if (n_str.length % 2 !== 0) n_str = "0" + n_str
        var pow = 1
        var i = 0
        var j = 0
        for (var k = 0; k < n_str.length/2; k++) {
            i += pow * parseInt(n_str.slice(n_str.length - 1 - 2*k - 1, n_str.length - 1 - 2*k)) 
            j += pow * parseInt(n_str.slice(n_str.length - 1 - 2*k, n_str.length - 2*k))
            pow *= 2
        }
        return [i,j]
    }
}

/**
 * implementation of hilbert order matrix walker
 */
class HilbertWalker extends MatrixWalker {

    // translate method translates from row i and col j location to the index in the underlying stored array
    translate(i, j) {
        var base_case = [0, 1, 3, 2]
        var ret = 0
        for (var mask = this.n_pw2 / 2; mask >= 1; mask /= 2) {
            var quadrant = (((i & mask) === mask) << 1) + ((j & mask) === mask)
            ret += base_case[quadrant]
            i &= mask - 1
            j &= mask - 1
            if (quadrant === 0) {
                [i, j] = [j, i]
            } 
            else if (quadrant === 2) {
                [i, j] = [mask - 1 - j, mask - 1 - i]
            }
            if (mask > 1) {
                ret <<= 2
            }
        }
        return ret
    }

    // the inverse of translate
    reverse_translate(n) {
        // n: a positive int less than size of this.arr
        var n_str = (this.n_pw2**2 + n).toString(2).slice(1)
        // un-interleave n
        var i_arr = ''
        var j_arr = ''
        for (var idx = 0; idx < n_str.length; idx += 2) {
            i_arr += n_str.charAt(idx)
            j_arr += n_str.charAt(idx + 1)
        }
        i_arr = parseInt(i_arr, 2)
        j_arr = parseInt(j_arr, 2)
        // start from innermost recursion (least significant bit)
        var i_mat = 0
        var j_mat = 0
        for (var mask = 1; mask < this.n_pw2; mask *= 2) {
            var i_cur = ((i_arr & mask) === mask)
            var j_cur = i_cur ^ ((j_arr & mask) === mask)
            var quadrant = (i_cur << 1) + (j_cur)
            if (mask > 1) {
                if (quadrant === 0) {
                    [i_mat, j_mat] = [j_mat, i_mat]
                } 
                else if (quadrant === 2) {
                    [i_mat, j_mat] = [mask - 1 - j_mat, mask - 1 - i_mat]
                }
            }
            i_mat += i_cur * mask
            j_mat += j_cur * mask
        }
        return [i_mat, j_mat]
    }
}


/**
 * class representing a LRU cache
 */
class Cache {

    constructor(width, height){
        this.width = width
        this.height = height
        this.lru = []
        this.cache_accesses = 0
        this.cache_hits = 0
        this.cache_misses = 0
    }

    /**
     * simulates accessing a cache line in a LRU cache
     * @param {cache line index} line 
     */
    access(line) {
        if (this.lru.includes(line)) { // line resident in cache
            this.cache_accesses++
            this.cache_hits++
            this.lru.splice(this.lru.indexOf(line), 1);
            this.lru.unshift(line)
        }
        else if (this.lru.length >= this.height) { // capacity miss
            this.cache_accesses++
            this.cache_misses++
            this.lru.pop()
            this.lru.unshift(line)
        }
        else { // cold miss
            this.cache_accesses++
            this.cache_misses++
            this.lru.unshift(line)
        }
    }

    /**
     * logs basic cache statistics for hits and misses
     */
    stats() {
        console.log(`Cache of width ${this.width} and height ${this.height}`)
        console.log(`Total of ${this.cache_accesses} cache accesses and ${this.cache_hits} cache hits`)
        if (this.cache_accesses === 0) {
            console.log("No cache accesses")
        }
        else {
            console.log(`Cache hit percentage: ${this.cache_hits/this.cache_accesses}`)
        }
    }
}

export {NaiveWalker, ZWalker, HilbertWalker};