# Lab 3 — Cache Simulation

## Objective

Analyze how different cache parameters affect performance using a Cache Simulator. This lab explores the impact of **associativity**, **block size**, **replacement policies**, **write policies**, **prefetching**, **multi-level caches**, and **coherence protocols** on cache hit rates and memory traffic.

## Tasks

| Task | Experiment | Key Parameter |
|------|-----------|---------------|
| 1 | Generate 10 trace files (500K accesses each) | Various access patterns |
| 2 | Baseline L1 analysis | 32KB, 1-way, 64B, LRU, Write-Back |
| 3 | Vary associativity | 2-way, 4-way, 8-way |
| 4 | Vary block size | 16B, 32B, 128B |
| 5 | Vary replacement policy | LRU, FIFO, PLRU, Random |
| 6 | Change write policy | Write-Through vs Write-Back |
| 7 | Enable prefetching | Stride prefetcher, distance=4 |
| 8 | Add L2 cache | 256KB, 16-way, PLRU |
| 9 | MESI coherence assessment | 4-processor traces |

## Folder Structure

```
Lab3/
├── CacheSimulator/         # Simulator source code (build with build.ps1)
├── report/
│   └── cache_lab_report.tex    # Complete LaTeX report with all tables & analysis
├── docs/
│   ├── cache-lab.md            # Lab instructions
│   ├── configuration.md        # Simulator config reference
│   ├── coherence-protocols.md  # MESI / MSI / MOESI
│   ├── examples.md             # Usage examples
│   ├── cache_lab_tutorial.md   # Step-by-step tutorial
│   └── ...
├── configs/                # JSON config files for each task
│   ├── task2_baseline.json     # 32KB, 1-way, 64B, LRU, WB
│   ├── task3_assoc*.json       # 2/4/8-way associativity
│   ├── task4_block*.json       # 16/32/128B block size
│   ├── task5_*.json            # LRU/FIFO/PLRU/Random replacement
│   ├── task6_wt.json           # Write-Through
│   └── task8_l2.json           # L1 + L2 cache
├── scripts/                # Automation scripts
│   ├── run_all_tasks.ps1       # Runs all Tasks 3-9
│   ├── run_task2.ps1           # Runs Task 2 baseline
│   └── extract_results.ps1    # Extracts metrics to summary
├── results/                # Raw simulation output
│   ├── task2/                  # Baseline (10 trace results)
│   ├── task3_assoc*/           # Associativity variants
│   ├── task4_block*/           # Block size variants
│   ├── task5_*/                # Replacement policy variants
│   ├── task6_wt/               # Write-Through
│   ├── task7_prefetch/         # Prefetching
│   ├── task8_l2/               # L2 cache
│   ├── task9_mesi/             # MESI coherence
│   └── all_results.txt         # Summary of all metrics
└── traces/                 # Generated memory access trace files
    └── README.md               # Instructions to regenerate
```

## How to Reproduce

### 1. Build the Simulator

```powershell
cd CacheSimulator
.\build.ps1
```

### 2. Generate Traces (Task 1)

```powershell
cd CacheSimulator\build
New-Item -ItemType Directory -Path traces -Force

.\bin\tools\trace_generator.exe -p sequential -n 500000 -o traces\sequential_500k.txt
.\bin\tools\trace_generator.exe -p random -n 500000 -o traces\random_500k.txt
.\bin\tools\trace_generator.exe -p strided --stride 64 -n 500000 -o traces\strided64_500k.txt
.\bin\tools\trace_generator.exe -p looping --loop-size 2 --repetitions 4 -n 500000 -o traces\looping_2x4_500k.txt
.\bin\tools\trace_generator.exe -p mixed --locality 0.8 -n 500000 -o traces\mixed_high_loc_500k.txt
.\bin\tools\trace_generator.exe -p mixed --locality 0.2 -n 500000 -o traces\mixed_low_loc_500k.txt
.\bin\tools\trace_generator.exe -p mixed -w 0.8 -n 500000 -o traces\write_intensive_500k.txt
.\bin\tools\trace_generator.exe -p mixed -w 0.2 -n 500000 -o traces\low_write_500k.txt
.\bin\tools\trace_generator.exe -p sequential --processors 4 -n 500000 -o traces\seq_4proc_500k.txt
.\bin\tools\trace_generator.exe -p random --processors 4 -n 500000 -o traces\rand_4proc_500k.txt
```

### 3. Run Simulations (Tasks 2-9)

```powershell
# Single run example:
.\bin\cachesim.exe --config ..\..\configs\task2_baseline.json traces\sequential_500k.txt

# All tasks at once:
powershell -File ..\..\scripts\run_all_tasks.ps1
```

## Key Findings

1. **Direct-mapped caches** produce 99.9%+ conflict misses for most workloads
2. **128B blocks** improve sequential hit rate from 0% → 50%
3. **LRU** outperforms FIFO, Random, and PLRU by 1–4%
4. **Prefetching** boosts sequential access from 0% → 41%
5. **L2 cache** catches 44–62% of L1 misses
6. **Write ratio** doesn't affect hit rate but impacts write-back traffic

## Author

**Sharique** — PDC Lab 3
