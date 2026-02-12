$resultsBase = "d:\Sharique\sem 6\PDC\CacheSimulator\results"

function Extract-Stats($filePath) {
    $content = Get-Content $filePath -Raw
    $stats = @{}
    
    if ($content -match 'Hits:\s+(\d+)') { $stats['Hits'] = $matches[1] }
    if ($content -match 'Misses:\s+(\d+)') { $stats['Misses'] = $matches[1] }
    if ($content -match 'Reads:\s+(\d+)') { $stats['Reads'] = $matches[1] }
    if ($content -match 'Writes:\s+(\d+)') { $stats['Writes'] = $matches[1] }
    if ($content -match 'Write-backs:\s+(\d+)') { $stats['WriteBacks'] = $matches[1] }
    if ($content -match 'Hit Ratio:\s+([\d.]+)%') { $stats['HitRatio'] = $matches[1] }
    if ($content -match 'Compulsory Misses:\s+(\d+)') { $stats['Compulsory'] = $matches[1] }
    if ($content -match 'Capacity Misses:\s+(\d+)') { $stats['Capacity'] = $matches[1] }
    if ($content -match 'Conflict Misses:\s+(\d+)') { $stats['Conflict'] = $matches[1] }
    if ($content -match 'Coherence Misses:\s+(\d+)') { $stats['Coherence'] = $matches[1] }
    
    return $stats
}

$traceNames = @(
    @("sequential_500k", "Sequential"),
    @("random_500k", "Random"),
    @("strided64_500k", "Strided-64"),
    @("looping_2x4_500k", "Looping"),
    @("mixed_high_loc_500k", "Mixed Hi-Loc"),
    @("mixed_low_loc_500k", "Mixed Lo-Loc"),
    @("write_intensive_500k", "Write-Heavy"),
    @("low_write_500k", "Read-Heavy"),
    @("seq_4proc_500k", "Seq 4-Proc"),
    @("rand_4proc_500k", "Rand 4-Proc")
)

$taskDirs = @(
    @("task2", "Task 2 Baseline (1-way)"),
    @("task3_assoc2", "Task 3 (2-way)"),
    @("task3_assoc4", "Task 3 (4-way)"),
    @("task3_assoc8", "Task 3 (8-way)"),
    @("task4_block16", "Task 4 (16B)"),
    @("task4_block32", "Task 4 (32B)"),
    @("task4_block128", "Task 4 (128B)"),
    @("task5_lru", "Task 5 (LRU)"),
    @("task5_fifo", "Task 5 (FIFO)"),
    @("task5_plru", "Task 5 (PLRU)"),
    @("task5_random", "Task 5 (Random)"),
    @("task6_wt", "Task 6 (Write-Through)"),
    @("task7_prefetch", "Task 7 (Prefetch)"),
    @("task8_l2", "Task 8 (L2 Cache)")
)

$output = ""
foreach ($taskInfo in $taskDirs) {
    $taskDir = $taskInfo[0]
    $taskLabel = $taskInfo[1]
    $dirPath = Join-Path $resultsBase $taskDir
    
    if (-not (Test-Path $dirPath)) { continue }
    
    $output += "`n=== $taskLabel ===`n"
    $output += "Trace,Reads,Writes,Hits,Misses,WriteBack,HitRatio,Compulsory,Capacity,Conflict,Coherence`n"
    
    foreach ($traceInfo in $traceNames) {
        $traceName = $traceInfo[0]
        $traceLabel = $traceInfo[1]
        $resultFile = Join-Path $dirPath "${traceName}_result.txt"
        
        if (Test-Path $resultFile) {
            $s = Extract-Stats $resultFile
            $output += "$traceLabel,$($s['Reads']),$($s['Writes']),$($s['Hits']),$($s['Misses']),$($s['WriteBacks']),$($s['HitRatio']),$($s['Compulsory']),$($s['Capacity']),$($s['Conflict']),$($s['Coherence'])`n"
        }
    }
}

$output | Out-File -FilePath "d:\Sharique\sem 6\PDC\CacheSimulator\all_results.txt" -Encoding utf8
Write-Host "Results extracted to all_results.txt"
Write-Host $output
