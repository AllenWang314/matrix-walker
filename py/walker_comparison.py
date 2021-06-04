from matrix_walker import ZWalker, HilbertWalker, NaiveWalker
import random, time

def initialize(dim, cache_width=8, cache_height=128):
    """
    initializes z_sim and hilbert_sim with identical matrices of size 2**dim x 2**dim
    typical cache size is 64 bytes by 8 KB with each double taking 8 bytes
    """
    z_sim_matrix = []
    hilbert_sim_matrix = []
    naive_sim_matrix = []
    # generate random matrices
    for _ in range(2**dim):
        z_list = []
        hilbert_list = []
        naive_list = []
        for _ in range(2**dim):
            z_list.append(random.random())
            hilbert_list.append(random.random())
            naive_list.append(random.random())
        z_sim_matrix.append(z_list)
        hilbert_sim_matrix.append(hilbert_list)
        naive_list.append(naive_list)
    # initialize simulators
    z_sim = ZWalker(z_sim_matrix, cache_width, cache_height)
    hilbert_sim = HilbertWalker(hilbert_sim_matrix, cache_width, cache_height)
    naive_sim = NaiveWalker(hilbert_sim_matrix, cache_width, cache_height)
    z_sim.i = 2**dim - 1
    hilbert_sim.i = 2**dim - 1
    naive_sim.i = 2**dim - 1
    return z_sim, hilbert_sim, naive_sim

default_probabilities = {"left": 0.25, "right": 0.25, "up": 0.25, "down":0.25}

def random_walker(simulations):
    failed = True
    while failed:
        r = 2
        if r == 0:
            try:
                for matrix in simulations:
                    matrix.left()
                failed = False
            except:
                pass
        elif r == 1:
            try:
                for matrix in simulations:
                    matrix.right()
                failed = False
            except:
                pass
        elif r == 2:
            try:
                for matrix in simulations:
                    matrix.up()
                failed = False
            except:
                failed = False
                pass
        elif r == 3:
            try:
                for matrix in simulations:
                    matrix.down()
                failed = False
            except:
                pass

def run_random_simulation(num_trials=5, moves_per_trial=10000, matrix_size=10, cache_width=8, cache_height=32):
    print('Initializing...')
    z_sim, hilbert_sim, naive_sim = initialize(matrix_size, cache_width, cache_height)
    print('Simulating...')
    start = 0

    for i in range(num_trials):
        for j in range(moves_per_trial):
            now = time.time()
            if now - start > 1:
                start = now
                print(f'trial {i}/{num_trials}, iteration {j}/{moves_per_trial}...')
            random_walker([z_sim, hilbert_sim, naive_sim])
        
        x = random.randrange(2**matrix_size)
        y = random.randrange(2**matrix_size)
        z_sim.teleport(x,y)
        hilbert_sim.teleport(x,y)
        naive_sim.teleport(x,y)

    print("\n################ Z index ################")
    z_sim.cache.stats()
    print("\n################ Hilbert ################")
    hilbert_sim.cache.stats()
    print("\n################ Naive ################")
    naive_sim.cache.stats()

def run_up_down_simulation(num_trials=5, moves_per_trial=10000, matrix_size=10, cache_width=8, cache_height=32):
    print('Initializing...')
    z_sim, hilbert_sim, naive_sim = initialize(10)
    print('Simulating...')
    start = 0

    for i in range(num_trials):
        for j in range(moves_per_trial):
            now = time.time()
            if now - start > 1:
                start = now
                print(f'trial {i}/{num_trials}, iteration {j}/{moves_per_trial}...')
            random_walker([z_sim, hilbert_sim, naive_sim])
        
        x = random.randrange(2**matrix_size)
        y = random.randrange(2**matrix_size)
        z_sim.teleport(x,y)
        hilbert_sim.teleport(x,y)
        naive_sim.teleport(x,y)

    print("\n################ Z index ################")
    z_sim.cache.stats()
    print("\n################ Hilbert ################")
    hilbert_sim.cache.stats()
    print("\n################ Naive ################")
    naive_sim.cache.stats()

if __name__ == "__main__":
    run_random_simulation()

    
