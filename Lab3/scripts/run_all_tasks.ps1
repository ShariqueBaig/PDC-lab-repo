$cachesim = "d:\Sharique\sem 6\PDC\CacheSimulator\build\bin\cachesim.exe"
$configDir = "d:\Sharique\sem 6\PDC\CacheSimulator\configs"
$tracesDir = "d:\Sharique\sem 6\PDC\CacheSimulator\build\traces"
$resultsBase = "d:\Sharique\sem 6\PDC\CacheSimulator\results"

$traces = @(
    "sequential_500k.txt",
    "random_500k.txt",
    "strided64_500k.txt",
    "looping_2x4_500k.txt",
    "mixed_high_loc_500k.txt",
    "mixed_low_loc_500k.txt",
    "write_intensive_500k.txt",
    "low_write_500k.txt",
    "seq_4proc_500k.txt",
    "rand_4proc_500k.txt"
)

# Define all tasks with their configs
$tasks = @{
    "task3_assoc2" = "task3_assoc2.json"
    "task3_assoc4" = "task3_assoc4.json"
    "task3_assoc8" = "task3_assoc8.json"
    "task4_block16" = "task4_block16.json"
    "task4_block32" = "task4_block32.json"
    "task4_block128" = "task4_block128.json"
    "task5_lru" = "task5_lru.json"
    "task5_fifo" = "task5_fifo.json"
    "task5_plru" = "task5_plru.json"
    "task5_random" = "task5_random.json"
    "task6_wt" = "task6_wt.json"
    "task8_l2" = "task8_l2.json"
}

# Task 7: prefetching uses command-line args
# Task 9: MESI uses just multiprocessor traces with --verbose

foreach ($taskName in $tasks.Keys | Sort-Object) {
    $configFile = Join-Path $configDir $tasks[$taskName]
    $resultDir = Join-Path $resultsBase $taskName
    New-Item -ItemType Directory -Path $resultDir -Force | Out-Null
    
    Write-Host "=== Running $taskName ==="
    foreach ($trace in $traces) {
        $tracePath = Join-Path $tracesDir $trace
        $resultName = $trace -replace '\.txt$', '_result.txt'
        $resultPath = Join-Path $resultDir $resultName
        
        Write-Host "  $trace ..."
        & $cachesim --config $configFile $tracePath 2>&1 | Out-File -FilePath $resultPath -Encoding utf8
    }
    Write-Host "  Done!"
}

# Task 7: Prefetching (use baseline config + --config NOT available for prefetch, use positional args)
$task7Dir = Join-Path $resultsBase "task7_prefetch"
New-Item -ItemType Directory -Path $task7Dir -Force | Out-Null
Write-Host "=== Running task7_prefetch ==="
foreach ($trace in $traces) {
    $tracePath = Join-Path $tracesDir $trace
    $resultName = $trace -replace '\.txt$', '_result.txt'
    $resultPath = Join-Path $task7Dir $resultName
    
    Write-Host "  $trace ..."
    # BLOCKSIZE L1_SIZE L1_ASSOC L2_SIZE L2_ASSOC PREF PREF_DIST trace
    & $cachesim 64 32768 1 262144 8 1 4 $tracePath 2>&1 | Out-File -FilePath $resultPath -Encoding utf8
}
Write-Host "  Done!"

# Task 9: MESI (just the multiprocessor traces with verbose)
$task9Dir = Join-Path $resultsBase "task9_mesi"
New-Item -ItemType Directory -Path $task9Dir -Force | Out-Null
Write-Host "=== Running task9_mesi ==="
$mpTraces = @("seq_4proc_500k.txt", "rand_4proc_500k.txt")
foreach ($trace in $mpTraces) {
    $tracePath = Join-Path $tracesDir $trace
    $resultName = $trace -replace '\.txt$', '_result.txt'
    $resultPath = Join-Path $task9Dir $resultName
    
    Write-Host "  $trace ..."
    & $cachesim --config (Join-Path $configDir "task2_baseline.json") --verbose $tracePath 2>&1 | Out-File -FilePath $resultPath -Encoding utf8
}
Write-Host "  Done!"

Write-Host "`n=== ALL TASKS COMPLETE ==="
