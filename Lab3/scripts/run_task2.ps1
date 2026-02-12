$cachesim = "d:\Sharique\sem 6\PDC\CacheSimulator\build\bin\cachesim.exe"
$config = "d:\Sharique\sem 6\PDC\CacheSimulator\configs\task2_baseline.json"
$tracesDir = "d:\Sharique\sem 6\PDC\CacheSimulator\build\traces"
$resultsDir = "d:\Sharique\sem 6\PDC\CacheSimulator\results\task2"

New-Item -ItemType Directory -Path $resultsDir -Force | Out-Null

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

foreach ($trace in $traces) {
    $tracePath = Join-Path $tracesDir $trace
    $resultName = $trace -replace '\.txt$', '_result.txt'
    $resultPath = Join-Path $resultsDir $resultName
    
    Write-Host "Running: $trace ..."
    & $cachesim --config $config $tracePath 2>&1 | Out-File -FilePath $resultPath -Encoding utf8
    Write-Host "  Saved to: $resultPath"
}

Write-Host "`nAll done! Results in $resultsDir"
