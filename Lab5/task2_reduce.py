import multiprocessing
import time
import math

# A heavy operation to make parallelization worth it
def heavy_worker(chunk):
    return sum(math.sqrt(x**2) + math.sin(x) for x in chunk)

def parallel_reduce(numbers, pool):
    # Split data into chunks based on number of processes
    n_procs = multiprocessing.cpu_count()
    chunk_size = len(numbers) // n_procs
    chunks = [numbers[i : i + chunk_size] for i in range(0, len(numbers), chunk_size)]
    
    # Map: Compute partial sums of chunks in parallel
    partial_sums = pool.map(heavy_worker, chunks)
    
    # Reduce: Combine partial results sequentially (very small number of items now)
    return sum(partial_sums)

def sequential_reduce(numbers):
    return sum(math.sqrt(x**2) + math.sin(x) for x in numbers)

def main():
    size = 5_000_000
    numbers = list(range(size))
    
    print(f"--- Task 2: Reduce Skeleton (Processing {size:,} numbers) ---")
    
    # Timing Sequential
    start = time.perf_counter()
    seq_sum = sequential_reduce(numbers)
    end = time.perf_counter()
    seq_time = end - start
    print(f"Sequential Time: {seq_time:.4f} seconds")
    
    # Timing Parallel
    start = time.perf_counter()
    with multiprocessing.Pool() as pool:
        par_sum = parallel_reduce(numbers, pool)
    end = time.perf_counter()
    par_time = end - start
    print(f"Parallel Time:   {par_time:.4f} seconds")
    
    # Verification
    # Use math.isclose for floats
    assert math.isclose(seq_sum, par_sum), f"Results don't match! Seq: {seq_sum}, Par: {par_sum}"
    print(f"Total Result: {seq_sum:.2f}")
    print("Verification Successful!")

if __name__ == "__main__":
    main()
