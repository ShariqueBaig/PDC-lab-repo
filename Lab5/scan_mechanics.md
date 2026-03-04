# How the Scan Skeleton Works (The "Hillis-Steele" Algorithm)

The **Scan Skeleton** (Prefix Sum) is tricky because calculating `Sum[i]` usually depends on `Sum[i-1]`. This recursive dependency seems to make parallelism impossible. 

**The Trick**: instead of waiting for the person *immediately* to your left, you look further back in "jumps" (1, 2, 4, 8...).

Let's trace it with a list of 8 numbers: `[1, 1, 1, 1, 1, 1, 1, 1]`
**Goal**: The result should be `[1, 2, 3, 4, 5, 6, 7, 8]`

---

## Step 0: Initial State
Everyone holds their own value.
```
Index:  0  1  2  3  4  5  6  7
Value: [1, 1, 1, 1, 1, 1, 1, 1]
```

## Step 1: Distance = 1
**Rule**: Look at element `i - 1`. Add its value to yours.
- Index 0 has no `i-1`. Stays `1`.
- Index 1 adds Index 0: `1 + 1 = 2`
- Index 2 adds Index 1: `1 + 1 = 2` (Note: It looks at the *old* value of Index 1)
- ...Everyone adds their immediate left neighbor parallelly.

**Result after Step 1**:
```
Index:  0  1  2  3  4  5  6  7
Value: [1, 2, 2, 2, 2, 2, 2, 2]
```
*Meaning: Now every cell `i` holds the sum of `Original[i] + Original[i-1]`.* (Sum of 2 items)

## Step 2: Distance = 2
**Rule**: Look at element `i - 2`. Add its value to yours.
- Index 0, 1: No `i-2`. Stay same.
- Index 2 adds Index 0: `2 + 1 = 3`
- Index 3 adds Index 1: `2 + 2 = 4`
- Index 4 adds Index 2: `2 + 2 = 4`

**Result after Step 2**:
```
Index:  0  1  2  3  4  5  6  7
Value: [1, 2, 3, 4, 4, 4, 4, 4]
```
*Meaning: Now every cell `i` holds the sum of 4 items.*

## Step 3: Distance = 4
**Rule**: Look at element `i - 4`. Add its value to yours.
- Indices 0,1,2,3: No `i-4`. Stay same.
- Index 4 adds Index 0: `4 + 1 = 5`
- Index 5 adds Index 1: `4 + 2 = 6`
- Index 6 adds Index 2: `4 + 3 = 7`
- Index 7 adds Index 3: `4 + 4 = 8`

**Result after Step 3**:
```
Index:  0  1  2  3  4  5  6  7
Value: [1, 2, 3, 4, 5, 6, 7, 8]
```
*Meaning: Now every cell `i` holds the sum of 8 items.*

---

## Why is this faster?
If we had 1,000,000 items:
- **Sequential**: 1,000,000 steps. (Index 999999 waits for 999998...)
- **Parallel Scan**: only 20 steps! ($\log_2 1,000,000 \approx 20$).

We double our reach every step ($1 \to 2 \to 4 \to 8 \to 16...$). This "doubling" is why we can solve a massively sequential-looking problem in very few parallel moves.
