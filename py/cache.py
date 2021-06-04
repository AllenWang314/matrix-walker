import random
import collections

class Cache:
    """
    class representing Cache that interacts with matrices in matrix_walker
    assumes a fully associative cache with LRU policy
    """

    def __init__(self, width, height, resident_lines=[]):
        """
        constructs a representation of a cache
        width: size of each cache line
        height: number of lines in cache
        resident_lines: list of indices of lines already in the cache
        lru: deque for LRU replacement policy, add newly used to the front
        cache_accesses: number of accesses to the cache
        cache_hits: number of cache hits
        cache_misses: number of cache misses
        """
        self.width = width
        self.height = height
        self.resident_lines = set(resident_lines)
        self.lru = collections.deque()
        self.cache_accesses = 0
        self.cache_hits = 0
        self.cache_misses = 0
        for line in resident_lines:
            self.lru.append(line)

    def access(self, line):
        """
        changing the cache by accessing line indexed by line
        """
        # check if line is resident
        if line in self.resident_lines:
            self.cache_accesses += 1
            self.cache_hits += 1
            self.lru.remove(line)
            self.lru.appendleft(line)
        # line is not resident, check if capacity miss
        elif len(self.resident_lines) >= self.height:
            self.cache_accesses += 1
            self.cache_misses += 1
            evicted_line = self.lru.pop()
            self.resident_lines.remove(evicted_line)
            self.resident_lines.add(line)
            self.lru.appendleft(line)
        # compulsory miss
        else:
            self.cache_accesses += 1
            self.cache_misses += 1
            self.resident_lines.add(line)
            self.lru.appendleft(line)

    def stats(self):
        """"prints simple statistics for cache hits and misses"""
        print(f"Cache of width {self.width} and height {self.height}")
        print(f"Total of {self.cache_accesses} cache accesses and {self.cache_hits} cache hits")
        if self.cache_accesses == 0:
            print("No cache accesses")
        else:
            print(f"Cache hit percentage: {self.cache_hits/self.cache_accesses}")


if __name__ == "__main__":
    # a couple of example and test usages
    test_cache = Cache(8, 4) # creates a cache with 4 lines of length 8
    test_cache.access(0)
    test_cache.access(1)
    test_cache.access(2)
    test_cache.access(3)
    test_cache.access(4)
    print(test_cache.lru) # expect deque([4, 3, 2, 1])
    for _ in range(1000):
        r = random.randrange(0, 5, 1)
        test_cache.access(r)
    test_cache.stats() # should be around 0.8




    
