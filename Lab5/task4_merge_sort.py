import time
import random
from concurrent.futures import ProcessPoolExecutor

def merge(left, right):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result

def sequential_merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = sequential_merge_sort(arr[:mid])
    right = sequential_merge_sort(arr[mid:])
    return merge(left, right)

def parallel_merge_sort(arr, executor=None):
    if len(arr) <= 1:
        return arr
    
    # Threshold for stopping parallelism to avoid excessive overhead
    # and to prevent passing executor down (which causes pickling errors)
    if executor is None or len(arr) < 20_000:
        return sequential_merge_sort(arr)
    
    mid = len(arr) // 2
    
    # We only spawn parallel tasks for the first few levels of the recursion tree
    # to avoid the "cannot pickle executor" issue and process spawning overhead.
    # We call sequential_merge_sort inside the submitted tasks.
    f_left = executor.submit(sequential_merge_sort, arr[:mid])
    f_right = executor.submit(sequential_merge_sort, arr[mid:])
    
    return merge(f_left.result(), f_right.result())

def main():
    # Larger size to show speedup
    size = 500_000
    data = [random.randint(0, size) for _ in range(size)]
    
    print(f"--- Task 4: D&C (Merge Sort of {size:,} items) ---")
    
    # Timing Sequential
    start = time.perf_counter()
    seq_res = sequential_merge_sort(data)
    end = time.perf_counter()
    seq_time = end - start
    print(f"Sequential Time: {seq_time:.4f} seconds")
    
    # Timing Parallel
    start = time.perf_counter()
    with ProcessPoolExecutor() as executor:
        par_res = parallel_merge_sort(data, executor)
    end = time.perf_counter()
    par_time = end - start
    print(f"Parallel Time:   {par_time:.4f} seconds")
    
    # Verification
    assert seq_res == par_res, "Results don't match!"
    print("Verification Successful!")

if __name__ == "__main__":
    main()
