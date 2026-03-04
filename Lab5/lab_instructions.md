# Lab 5: Execution Guide & Code Explanation

Here is how to run each task and what the code is actually doing.

## Task 1: Map Skeleton (Squaring Numbers)
**The Goal**: Square a list of numbers $[1, 2, ..., 10]$ in parallel.
**How to Run**:
```powershell
python task1_map.py
```
**Code Breakdown**:
```python
with multiprocessing.Pool() as pool:
    result = pool.map(square, numbers)
```
- `Pool()`: Creates a group of worker processes (usually equal to your CPU cores).
- `pool.map(square, numbers)`: This is the **Map Skeleton**. It splits the `numbers` list into chunks, sends them to different workers, applies the `square` function, and reassembles the results.

---

## Task 2: Reduce Skeleton (Summing Numbers)
**The Goal**: Sum up $[5, 12, 8, 20, ...]$.
**How to Run**:
```powershell
python task2_reduce.py
```
**Code Breakdown**:
```python
pairs = [(data[i], data[i+1]) for i in range(0, len(data), 2)]
results = parallel_sum_step(pairs)
```
- We don't just loop `sum += x`. That's sequential.
- Instead, we pair numbers $(5+12), (8+20)...$ and sum them in parallel.
- We repeat this recursively until only one number is left. This forms a **Reduction Tree**.

---

## Task 3: Scan Skeleton (Prefix Sum)
**The Goal**: Turn $[1, 2, 3]$ into $[1, 3, 6]$.
**How to Run**:
```powershell
python task3_scan.py
```
**Code Breakdown**:
```python
for i in range(step, n):
    res[i] = temp[i] + temp[i - step]
```
- This uses the **Hillis-Steele algorithm**.
- In step 1, everyone adds the neighbor 1 spot away.
- In step 2, everyone adds the neighbor 2 spots away.
- In step 3, 4 spots away...
- This allows us to calculate the running total in $O(\log n)$ steps instead of $O(n)$.

---

## Task 4: Divide & Conquer (Merge Sort)
**The Goal**: Sort a mixed up list.
**How to Run**:
```powershell
python task4_merge_sort.py
```
**Code Breakdown**:
```python
f_left = executor.submit(merge_sort_parallel, arr[:mid])
f_right = executor.submit(merge_sort_parallel, arr[mid:])
```
- `executor.submit`: This sends the "sort the left half" task to a background process.
- We do the same for the right half.
- Both halves are sorted **at the same time**.
- Then we `merge` them together (which is the sequential part).

---

## Task 5: Wavefront (Longest Common Substring)
**The Goal**: Find common text between "PARALLELISM" and "PDC_PARALLEL".
**How to Run**:
```powershell
python task5_wavefront_lcs.py
```
**Code Breakdown**:
```python
for k in range(2, n + m + 1):
    # k is the diagonal number
    for i in range(1, n + 1):
        j = k - i
        # internal logic...
```
- We iterate through diagonals `k`.
- Inside the diagonal loop, every valid `(i, j)` pair is independent of others in the *same* diagonal.
- This allows us to process the grid in "waves" moving from the top-left to the bottom-right.
