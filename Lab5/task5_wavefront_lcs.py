import time
import multiprocessing
import math

# Heavily intensive work to make parallelization worth it
def compute_cell(args):
    char1, char2, prev_val = args
    # Synthetic heavy work
    res = 0
    for i in range(5000):
        res += math.sin(i)
    
    if char1 == char2:
        return prev_val + 1
    return 0

def sequential_lcs_heavy(s1, s2):
    n, m = len(s1), len(s2)
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    max_len = 0
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            # Heavy work simulation
            res = 0
            for k in range(5000):
                res += math.sin(k)
                
            if s1[i-1] == s2[j-1]:
                dp[i][j] = dp[i-1][j-1] + 1
                max_len = max(max_len, dp[i][j])
    return max_len

def parallel_wavefront_lcs(s1, s2, pool):
    n, m = len(s1), len(s2)
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    max_len = 0
    
    # Process by diagonals
    # k = i + j
    for k in range(2, n + m + 1):
        # Collect all cells in this diagonal
        tasks = []
        indices = []
        for i in range(1, n + 1):
            j = k - i
            if 1 <= j <= m:
                tasks.append((s1[i-1], s2[j-1], dp[i-1][j-1]))
                indices.append((i, j))
        
        # Process the entire diagonal in parallel
        if tasks:
            results = pool.map(compute_cell, tasks)
            # Update the DP table with results from this diagonal
            for (idx_i, idx_j), val in zip(indices, results):
                dp[idx_i][idx_j] = val
                if val > max_len:
                    max_len = val
                    
    return max_len

def main():
    # Smaller size but heavier work to see speedup without waiting forever
    size = 150
    s1 = "A" * size + "BCDE" + "F" * size
    s2 = "X" * size + "BCDE" + "Y" * size
    
    print(f"--- Task 5: Wavefront LCS (Heavy Comparison on length {len(s1)}) ---")
    
    # Timing Sequential
    start = time.perf_counter()
    seq_res = sequential_lcs_heavy(s1, s2)
    end = time.perf_counter()
    seq_time = end - start
    print(f"Sequential Time: {seq_time:.4f} seconds")
    
    # Timing Parallel Wavefront
    start = time.perf_counter()
    # Use a chunksize to reduce IPC overhead if needed
    with multiprocessing.Pool() as pool:
        par_res = parallel_wavefront_lcs(s1, s2, pool)
    end = time.perf_counter()
    par_time = end - start
    print(f"Parallel Wavefront Time: {par_time:.4f} seconds")
    
    # Verification
    assert seq_res == par_res, f"Results don't match! Seq: {seq_res}, Par: {par_res}"
    print(f"Max LCS: {seq_res}")
    print("Verification Successful!")

if __name__ == "__main__":
    main()
