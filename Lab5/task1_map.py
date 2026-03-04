import multiprocessing
import time
import math

def complex_operation(n):
    # Making the task slightly heavier to overcome IPC overhead
    return math.sqrt(n**2) + math.sin(n)

def sequential_map(numbers):
    return [complex_operation(n) for n in numbers]

def parallel_map(numbers):
    with multiprocessing.Pool() as pool:
        return pool.map(complex_operation, numbers)

def main():
    # Larger dataset and heavier task for speedup
    size = 5_000_000
    numbers = list(range(size))
    
    print(f"--- Task 1: Map Skeleton (Squaring {size:,} numbers) ---")
    
    # Timing Sequential
    start = time.perf_counter()
    seq_res = sequential_map(numbers)
    end = time.perf_counter()
    seq_time = end - start
    print(f"Sequential Time: {seq_time:.4f} seconds")
    
    # Timing Parallel
    start = time.perf_counter()
    par_res = parallel_map(numbers)
    end = time.perf_counter()
    par_time = end - start
    print(f"Parallel Time:   {par_time:.4f} seconds")
    
    # Verification
    assert seq_res == par_res, "Results don't match!"
    print("Verification Successful!")

if __name__ == "__main__":
    main()
