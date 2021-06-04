CC := g++
TARGET := benchmark
BINARIES := benchmark_matrix benchmark_tensor test_matrix_walker test_tensor_walker

DEBUG := 0
FLTO := 0

ifeq ($(DEBUG),1)
	CFLAGS += -O0 -DDEBUG
else
	CFLAGS += -O3 -DNDEBUG
endif

ifeq ($(FLTO),1)
	CFLAGS += -flto
endif

benchmark: benchmark.cpp
	$(CC) $(CFLAGS) $^ -o $@ 

test_matrix_walker: test_matrix_walker.cpp matrix_walker.cpp
	$(CC) $(CFLAGS) $^ -o $@ 

test_tensor_walker: test_tensor_walker.cpp tensor_walker.cpp
	$(CC) $(CFLAGS) $^ -o $@

benchmark_matrix: benchmark_matrix.cpp matrix_walker.cpp
	$(CC) $(CFLAGS) $^ -o $@

benchmark_tensor: benchmark_tensor.cpp tensor_walker.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o *.d* *~ $(BINARIES)
