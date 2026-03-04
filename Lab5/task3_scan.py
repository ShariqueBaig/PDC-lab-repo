import copy
import time

def sequential_scan(data):
    res = []
    current_sum = 0
    for x in data:
        current_sum += x
        res.append(current_sum)
    return res

def parallel_scan_sim(data):
    """Hillis-Steele simulated parallel steps."""
    res = copy.deepcopy(data)
    n = len(data)
    step = 1
    while step < n:
        temp = copy.deepcopy(res)
        for i in range(step, n):
            res[i] = temp[i] + temp[i - step]
        step *= 2
    return res

def main():
    # Use a size where O(log N) would be visible (e.g. 1000 items)
    # Note: Simulated parallel step logic in Python has overhead
    size = 1000
    numbers = [1] * size
    
    print(f"--- Task 3: Scan Skeleton (Prefix Sum on {size} items) ---")
    
    # Timing Sequential
    start = time.perf_counter()
    seq_res = sequential_scan(numbers)
    end = time.perf_counter()
    seq_time = end - start
    print(f"Sequential Time: {seq_time:.6f} seconds")
    
    # Timing Parallel (Simulated)
    start = time.perf_counter()
    par_res = parallel_scan_sim(numbers)
    end = time.perf_counter()
    par_time = end - start
    print(f"Parallel (Hillis-Steele Sim) Time: {par_time:.6f} seconds")
    
    # Verification
    assert seq_res == par_res, "Results don't match!"
    print("Verification Successful!")

if __name__ == "__main__":
    main()
