# Cache Lab — Complete Tutorial & Concepts Guide

This guide teaches you **everything you need** to understand and complete Lab 3. It covers fundamental concepts, then walks through each of the 9 lab tasks with the **exact commands** to run in your terminal.

---

## Part 1: The Fundamentals — Why Caches Exist

### The Memory Speed Problem

Your CPU can execute an instruction in **~1 nanosecond**, but reading from RAM takes **~100 nanoseconds**. That means the CPU would sit idle for **99% of the time** if it accessed RAM directly. This gap is called the **processor-memory gap**.

```
┌──────────┐        ┌──────────┐        ┌──────────┐
│   CPU    │◄──────►│  Cache   │◄──────►│   RAM    │
│  ~1 ns   │  ~1 ns │ (SRAM)   │ ~100ns │ (DRAM)   │
└──────────┘        └──────────┘        └──────────┘
     Fast ◄───────────────────────────────► Slow
     Small ◄──────────────────────────────► Large
```

### What is a Cache?

A **cache** is a small, fast memory placed between the CPU and RAM. It stores **copies** of frequently used data so the CPU doesn't have to wait for slow RAM.

Think of it like this:
- **RAM** = A library's warehouse (huge, but takes time to fetch books)
- **Cache** = Your desk (tiny, but instant access to the books on it)

### What is SRAM vs DRAM?

| Feature | SRAM (Cache) | DRAM (RAM) |
|---------|-------------|------------|
| **Speed** | Very fast (~1-10 ns) | Slow (~100 ns) |
| **Size** | Small (KB to MB) | Large (GB) |
| **Cost** | Expensive | Cheap |
| **How it works** | 6 transistors per bit (flip-flop) | 1 transistor + 1 capacitor per bit |
| **Needs refresh?** | No | Yes (every ~64 ms) |
| **Used for** | L1, L2, L3 caches | Main memory (your 8GB/16GB RAM) |

DRAM needs refreshing because capacitors leak charge — SRAM holds its value as long as power is on.

---

## Part 2: How Caches Work

### Key Cache Parameters

Every cache has three fundamental parameters:

#### 1. Block Size (Line Size)
The **smallest unit** of data transferred between cache and RAM. Typically **64 bytes**.

Why not 1 byte at a time? Because of **spatial locality** — if you access address `0x1000`, you'll likely access `0x1001`, `0x1002`, etc. soon after. Fetching a whole block is more efficient.

#### 2. Cache Size
The **total storage capacity** of the cache. e.g., 32 KB for L1.

```
Number of blocks = Cache Size / Block Size
Example: 32768 / 64 = 512 blocks
```

#### 3. Associativity
**How many places** a block of data can be stored in the cache.

| Type | Associativity | Description |
|------|--------------|-------------|
| **Direct-mapped** | 1-way | Each block maps to exactly ONE location |
| **2-way set associative** | 2-way | Each block maps to a SET of 2 locations |
| **4-way set associative** | 4-way | Each block maps to a SET of 4 locations |
| **Fully associative** | N-way | Block can go ANYWHERE |

```
Number of sets = Cache Size / (Block Size × Associativity)
Example (32KB, 64B blocks, 4-way): 32768 / (64 × 4) = 128 sets
```

### How an Address is Decoded

When the CPU wants data at address `0x1A3C`, the cache splits this address into three fields:

```
┌──────────────────┬──────────┬──────────────┐
│     Tag          │  Index   │ Block Offset │
│ (which block?)   │(which    │ (which byte  │
│                  │ set?)    │  in block?)  │
└──────────────────┴──────────┴──────────────┘
```

- **Block Offset** = `log₂(Block Size)` bits → tells which byte within the block
- **Index** = `log₂(Number of Sets)` bits → tells which set to look in
- **Tag** = remaining bits → uniquely identifies the block

#### Example: 32KB cache, 64-byte blocks, 4-way associative, 32-bit addresses
- Offset = log₂(64) = **6 bits**
- Sets = 32768 / (64×4) = 128 → Index = log₂(128) = **7 bits**
- Tag = 32 - 6 - 7 = **19 bits**

### Cache Hit vs Miss

- **Hit**: The data is found in the cache → Fast! (~1-10 cycles)
- **Miss**: The data is NOT in the cache → Must fetch from RAM (~100 cycles)

**Miss Rate** = Misses / Total Accesses  
**Hit Rate** = 1 - Miss Rate

### Three Types of Cache Misses (The "3 C's")

| Type | Cause | Fix |
|------|-------|-----|
| **Compulsory** (Cold) | First-ever access to a block — it was never in the cache before | Prefetching |
| **Capacity** | Cache is too small to hold all needed data | Larger cache |
| **Conflict** | Multiple blocks map to the same set, evicting each other | Higher associativity, victim cache |

---

## Part 3: Cache Policies

### Replacement Policies (Who Gets Evicted?)

When the cache is full and a new block must come in, which existing block gets thrown out?

| Policy | How It Works | Pros | Cons |
|--------|-------------|------|------|
| **LRU** | Evict the **Least Recently Used** block | Best hit rate overall | Expensive hardware for high associativity |
| **FIFO** | Evict the **oldest** block (first in, first out) | Simple | Doesn't consider recency |
| **PLRU** (Pseudo-LRU) | Approximation of LRU using a tree | Simpler than true LRU | Slightly worse hit rate |
| **Random** | Evict a **random** block | Simplest, no state needed | Unpredictable performance |

### Write Policies (What Happens on a Write?)

#### Write-Hit Policy
- **Write-Back**: Write only to cache; mark block "dirty"; write to RAM only when evicted → **Faster, less traffic**
- **Write-Through**: Write to BOTH cache AND RAM simultaneously → **Simpler, always consistent**

#### Write-Miss Policy
- **Write-Allocate**: On a write miss, bring the block into cache, then write → pairs with Write-Back
- **No-Write-Allocate**: On a write miss, write directly to RAM, don't cache → pairs with Write-Through

---

## Part 4: Multi-Level Caches

Modern CPUs have a **hierarchy** of caches:

```
┌──────┐     ┌──────┐     ┌──────┐     ┌──────┐
│ CPU  │────►│  L1  │────►│  L2  │────►│  L3  │────► RAM
│      │1-4  │32KB  │5-12 │256KB │12-40│ 8MB  │100+ns
│      │ ns  │      │ ns  │      │ ns  │      │
└──────┘     └──────┘     └──────┘     └──────┘
  Fastest ◄──────────────────────────────────► Slowest
  Smallest ◄─────────────────────────────────► Largest
```

- **L1 Miss**: Data is looked up in L2
- **L2 Miss**: Data is looked up in L3 (or RAM)

---

## Part 5: Advanced Features

### Prefetching
The cache **guesses** what data you'll need next and fetches it before you ask. Works great for **sequential** access patterns but can **pollute** the cache for random access.

### Victim Cache
A small fully-associative cache that holds **recently evicted blocks**. Reduces conflict misses because a block kicked out of L1 might be found in the victim cache before going to L2.

### MESI Coherence Protocol (Multi-Processor)

When multiple CPUs each have their own cache, they might hold **different copies of the same data**. The MESI protocol keeps them consistent:

| State | Meaning |
|-------|---------|
| **M** (Modified) | This cache has the ONLY copy, and it's been changed (dirty) |
| **E** (Exclusive) | This cache has the ONLY copy, and it's clean (matches RAM) |
| **S** (Shared) | Multiple caches may have this block; it's clean |
| **I** (Invalid) | This block is not valid in this cache |

**Key transitions:**
- CPU reads a block nobody else has → **Exclusive**
- CPU writes to an Exclusive block → **Modified** (no bus traffic needed — "silent upgrade")
- Another CPU reads a Modified block → Modified cache writes back, both go to **Shared**
- CPU writes to a Shared block → All other copies become **Invalid**, this one becomes **Modified**

---

## Part 6: Your Lab Tasks — Step-by-Step Commands

> [!IMPORTANT]
> All commands below assume you are in: `d:\Sharique\sem 6\PDC\CacheSimulator\build`

### Task 1: Generate 10 Trace Files

```powershell
# 1. Sequential access pattern
.\bin\tools\trace_generator.exe -p sequential -n 500000 -o traces\sequential_500k.txt

# 2. Random access pattern
.\bin\tools\trace_generator.exe -p random -n 500000 -o traces\random_500k.txt

# 3. Sequential with stride = 64
.\bin\tools\trace_generator.exe -p strided --stride 64 -n 500000 -o traces\strided64_500k.txt

# 4. Looping: 2 loops, 4 repetitions
.\bin\tools\trace_generator.exe -p looping --loop-size 2 --repetitions 4 -n 500000 -o traces\looping_2x4_500k.txt

# 5. Mixed with HIGH locality (0.8)
.\bin\tools\trace_generator.exe -p mixed --locality 0.8 -n 500000 -o traces\mixed_high_loc_500k.txt

# 6. Mixed with LOW locality (0.2)
.\bin\tools\trace_generator.exe -p mixed --locality 0.2 -n 500000 -o traces\mixed_low_loc_500k.txt

# 7. Write intensive (0.8 write ratio)
.\bin\tools\trace_generator.exe -p mixed -w 0.8 -n 500000 -o traces\write_intensive_500k.txt

# 8. Low write intensity (0.2 write ratio)
.\bin\tools\trace_generator.exe -p mixed -w 0.2 -n 500000 -o traces\low_write_500k.txt

# 9. Sequential with 4 processors
.\bin\tools\trace_generator.exe -p sequential --processors 4 -n 500000 -o traces\seq_4proc_500k.txt

# 10. Random with 4 processors
.\bin\tools\trace_generator.exe -p random --processors 4 -n 500000 -o traces\rand_4proc_500k.txt
```

---

### Task 2: Baseline L1 Analysis (Direct-Mapped)

Config: 32KB, **1-way** (direct-mapped), 64-byte blocks, LRU, Write-Back.

The simulator uses positional arguments: `BLOCKSIZE L1_SIZE L1_ASSOC L2_SIZE L2_ASSOC PREF PREF_DIST trace_file`

To run with **L1 only** (no L2 effect), set L2 very large or use defaults without L2 config:

```powershell
# Run for each trace (example for sequential):
.\bin\cachesim.exe 64 32768 1 262144 8 0 0 ..\traces\sequential_500k.txt
```

Repeat the above command for each of the 10 traces, replacing the trace filename. Record: **reads, writes, hits, misses, write-backs, hit ratio, and miss types** (compulsory, capacity, conflict).

---

### Task 3: Vary Associativity (2, 4, 8-way)

```powershell
# 2-way associative
.\bin\cachesim.exe 64 32768 2 262144 8 0 0 ..\traces\sequential_500k.txt

# 4-way associative
.\bin\cachesim.exe 64 32768 4 262144 8 0 0 ..\traces\sequential_500k.txt

# 8-way associative
.\bin\cachesim.exe 64 32768 8 262144 8 0 0 ..\traces\sequential_500k.txt
```

**What to observe:** Higher associativity → fewer **conflict misses**, but **diminishing returns**. Most improvement is 1→2 and 2→4. The 4→8 jump is smaller.

---

### Task 4: Vary Block Size (16, 32, 128)

```powershell
# Block size 16
.\bin\cachesim.exe 16 32768 1 262144 8 0 0 ..\traces\sequential_500k.txt

# Block size 32
.\bin\cachesim.exe 32 32768 1 262144 8 0 0 ..\traces\sequential_500k.txt

# Block size 128
.\bin\cachesim.exe 128 32768 1 262144 8 0 0 ..\traces\sequential_500k.txt
```

**What to observe:**
- Larger blocks → fewer **compulsory** misses (better spatial locality capture)
- BUT: fewer total blocks → more **capacity/conflict** misses
- Sweet spot is usually **64 bytes** for general workloads

---

### Task 5: Vary Replacement Policy

You'll need JSON config files for this. Create these configs:

**`configs/lru.json`:**
```json
{
  "l1": {
    "size": 32768, "associativity": 1, "blockSize": 64,
    "replacementPolicy": "LRU", "writePolicy": "WriteBack"
  }
}
```

**`configs/fifo.json`:** (same but `"replacementPolicy": "FIFO"`)  
**`configs/plru.json`:** (same but `"replacementPolicy": "PLRU"`)  
**`configs/random.json`:** (same but `"replacementPolicy": "Random"`)

> [!NOTE]
> With 1-way (direct-mapped) associativity, the replacement policy has NO effect because there's only one place each block can go. Use **4-way** associativity to see a difference!

```powershell
.\bin\cachesim.exe --config ..\configs\lru.json ..\traces\sequential_500k.txt
.\bin\cachesim.exe --config ..\configs\fifo.json ..\traces\sequential_500k.txt
.\bin\cachesim.exe --config ..\configs\plru.json ..\traces\sequential_500k.txt
.\bin\cachesim.exe --config ..\configs\random.json ..\traces\sequential_500k.txt
```

**What to observe:** LRU ≥ PLRU > FIFO > Random, but differences are usually small (1-3%).

---

### Task 6: Write-Through Policy

```json
{
  "l1": {
    "size": 32768, "associativity": 1, "blockSize": 64,
    "replacementPolicy": "LRU", "writePolicy": "WriteThrough"
  }
}
```

```powershell
.\bin\cachesim.exe --config ..\configs\write_through.json ..\traces\sequential_500k.txt
```

**What to observe:** Write-Through produces **zero write-backs** (data always goes to memory immediately) but generates **more memory traffic** than Write-Back.

---

### Task 7: Enable Prefetching

```powershell
# Prefetch enabled with distance=4
.\bin\cachesim.exe 64 32768 1 262144 8 1 4 ..\traces\sequential_500k.txt
```

**What to observe:**
- **Sequential traces**: Huge improvement — prefetcher correctly predicts next blocks
- **Random traces**: Little or no improvement — prefetched blocks are useless
- Check **prefetch accuracy** in the output

---

### Task 8: Add L2 Cache

Config: L2 = 256KB, 16-way, 64-byte blocks, PLRU, Write-Back.

```json
{
  "l1": {
    "size": 32768, "associativity": 1, "blockSize": 64,
    "replacementPolicy": "LRU", "writePolicy": "WriteBack"
  },
  "l2": {
    "size": 262144, "associativity": 16, "blockSize": 64,
    "replacementPolicy": "PLRU", "writePolicy": "WriteBack"
  }
}
```

```powershell
.\bin\cachesim.exe --config ..\configs\l2_config.json ..\traces\sequential_500k.txt
```

**What to observe:** L2 catches most L1 misses → the **effective miss rate** (data going all the way to RAM) drops dramatically.

---

### Task 9: Assess MESI Coherence

Use a **multiprocessor trace** (from Task 1) and look at the MESI statistics:

```powershell
.\bin\cachesim.exe --verbose ..\traces\seq_4proc_500k.txt
```

**What to look for in the output:**
- **I → E**: First read by a single processor (exclusive)
- **I → M**: First write (goes straight to Modified)
- **E → M**: Write to an exclusive block (silent upgrade — no bus traffic)
- **M → S**: Another processor reads this modified data (force write-back)
- **S → I**: Another processor writes to shared data (invalidation)

---

## Part 7: Quick Reference

### Simulator Command Format
```
.\bin\cachesim.exe BLOCKSIZE L1_SIZE L1_ASSOC L2_SIZE L2_ASSOC PREF PREF_DIST trace_file
```

| Argument | Meaning | Lab Default |
|----------|---------|-------------|
| BLOCKSIZE | Block size in bytes | 64 |
| L1_SIZE | L1 cache size in bytes | 32768 (32KB) |
| L1_ASSOC | L1 associativity | 1 (Task 2), 2/4/8 (Task 3) |
| L2_SIZE | L2 cache size in bytes | 262144 (256KB) |
| L2_ASSOC | L2 associativity | 8 |
| PREF | Enable prefetching (0/1) | 0 (off) |
| PREF_DIST | Prefetch distance | 4 |

### Key Output Metrics to Record

| Metric | Where to Find | What It Means |
|--------|--------------|---------------|
| Hits | Access Statistics | Times data was found in cache |
| Misses | Access Statistics | Times data was NOT found |
| Hit Ratio | Access Statistics | Hits / Total × 100 |
| Write-backs | Access Statistics | Dirty evictions |
| Compulsory Misses | Miss Type Statistics | Cold misses, first access |
| Capacity Misses | Miss Type Statistics | Working set > cache size |
| Conflict Misses | Miss Type Statistics | Blocks fighting for same set |
