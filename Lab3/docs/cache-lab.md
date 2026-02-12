## Lab 3: Cache Simulator

### Introduction

Caches bridge the performance gap between CPU and main memory. In this lab, we explore cache behavior using a trace-driven cache simulator.


#### Learning Objectives

By the end of this lab, you should be able to:

- Understand cache parameters (size, associativity, block size)
- Analyze cache hit/miss behavior
- Evaluate multi-level caches
- Study prefetching and victim caches
- Analyze MESI coherence protocols

---

### Getting Started

Clone and build the simulator (use Powershell):

```bash
git clone https://github.com/muditbhargava66/CacheSimulator.git
cd CacheSimulator
.\build.ps1
```

To better understand how the simulator works, please read the following documents attached with this manual:
- traces.md (how to create trace to simulate read/write operations)
- examples.md (how to run the simulator with different configurations)
- configurations.md (how to run the simulator by configuring the cache using a JSON file)
- multiprocessor.md (how to run cache simulator for multiple processors)
- coherence-protocols (how to use coherence protocols with cache simulator)

There are existing traces and configurations that you can explore in the Cache Simulator folder.

---

## Cache and Multiprocessor Architecture Analysis Task

In this task, you will analyze the behavior and performance of a memory hierarchy under different workloads and architectural enhancements. You will progressively move from a simple single-level cache to more advanced features such as multi-level caches, prefetching, victim caches, and finally multiprocessor systems with coherence protocols.

The goal is **not just to collect numbers**, but to **reason about why performance changes** under different configurations and access patterns.

---

## Tasks

1. Generate the following traces with 500000 steps:
   - Sequential access pattern
   - Random access pattern 
   - Sequential access pattern with stride = 64
   - Looping access pattern with 2 loops and 4 repetitions
   - Mixed pattern with high locality (0.8)
   - Mixed pattern with low locality (0.2)
   - Write intensive mixed patterns (0.8)
   - Low write intensity mixed patterns (0.2)
   - Sequential access pattern with 4 processors
   - Random access pattern with 4 processors

2. Report the behaviour of L1 cache where the cache is defined as
   - size: 32768
   - associativity: 1
   - block_size: 64
   - replacement_policy: LRU
   - write_policy: write back with write allocate

   Report reads, writes, hits, misses, write backs, hit ratio, and latency for all the traces created above. Also report the counts of different miss types.

3. Repeat step 2 with the following associativity: 2,4,8. State your observation regarding any change in the statistics

4. Repeat step 2 with the following block size: 16,32,128. State your observation regarding any change in the statistics

5. Repeat step 2 with the following replacement policy: LRU, FIFO, PLRU, Random. State your observation regarding any change in the statistics

6. Repeat step 2 with the following write policy: Write Through. State your observation regarding any change in the statistics

7. Introduce prefetching in step 2. State your observation regarding any change in the statistics.

8. Introduce L2 cache in step 2 with the following configuration:
   - size: 262144
   - associativity: 16
   - block_size: 64
   - replacement_policy: PLRU
   - write_policy: write back with write allocate
State your observation regarding any change in the statistics.

9. For any one configuration, try to assess the correctness of MESI statistics.
