# Traces Directory

This directory holds generated memory access trace files used as input to the Cache Simulator.

## Why are trace files not in the repo?

Trace files are excluded from git (via `.gitignore`) because they are large — each file is 5–8 MB, totaling ~60 MB for all 10 files. They are easily regenerable.

## How to regenerate

From the `CacheSimulator/build/` directory, run:

```powershell
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

See the [Lab 3 README](../README.md) for the full reproduction guide.
