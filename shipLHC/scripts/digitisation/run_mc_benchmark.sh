#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "Usage: $0 <n> <output_csv>"
    exit 1
fi

n=$1
output_csv=$2
specs_file="${output_csv%.csv}_specs.txt"

command=(python "$ADVSNDSW_ROOT/shipLHC/run_digiSND.py" \
    -f /eos/experiment/sndlhc/Run4/testbeam2026/digi_benchmark/mc_benchmark/electron_100/sndLHC.PG_11-TGeant4.root \
    -g /eos/experiment/sndlhc/Run4/testbeam2026/digi_benchmark/mc_benchmark/electron_100/geofile_full.PG_11-TGeant4.root \
    -n 10)

# Collect system specifications
{
    echo "========================================="
    echo "BENCHMARK SYSTEM SPECIFICATIONS"
    echo "========================================="
    echo "Date: $(date)"
    echo ""
    echo "vCPUs: $(nproc)"
    echo "Total RAM (GB): $(echo "scale=1; $(grep MemTotal /proc/meminfo | awk '{print $2}') / 1048576" | bc)"
    echo "Hypervisor: $(systemd-detect-virt)"
    echo "Kernel: $(uname -r)"
    echo "OS: $(grep PRETTY_NAME /etc/os-release | cut -d'"' -f2)"
    echo "Python: $(python --version 2>&1)"
    echo ""
    echo "CPU Model:"
    lscpu | grep "Model name"
    echo ""
    current_mhz=$(grep -m1 "cpu MHz" /proc/cpuinfo | awk '{print $NF}')
    max_mhz=$(cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq 2>/dev/null | awk '{printf "%.2f", $1/1000}')
    echo "CPU Frequency:"
    echo "  Current: ${current_mhz} MHz"
    [[ -n $max_mhz ]] && echo "  Max: ${max_mhz} MHz"
    echo ""
    echo "--- Benchmark Command ---"
    echo "${command[@]}"
    echo ""
} | tee "$specs_file"

# Run benchmark
echo "iteration,runtime_seconds,max_ram_mb,exit_code" > "$output_csv"

echo "Running $n iterations..."
for ((i = 1; i <= n; i++)); do
    echo "[$(date '+%H:%M:%S')] Iteration $i/$n..."
    
    start_time=$(date +%s.%N)
    
    "${command[@]}" > /tmp/snd_run_$i.log 2>&1 &
    pid=$!
    
    max_ram=0
    while kill -0 $pid 2>/dev/null; do
        [[ -r /proc/$pid/status ]] && \
        rss=$(grep "^VmRSS:" /proc/$pid/status | awk '{print $2}') && \
        [[ $rss -gt $max_ram ]] && max_ram=$rss
        sleep 0.5
    done
    
    wait $pid
    exit_code=$?
    
    end_time=$(date +%s.%N)
    elapsed=$(echo "$end_time - $start_time" | bc)
    max_ram_mb=$(echo "scale=2; $max_ram / 1024" | bc)
    
    echo "$i,$elapsed,$max_ram_mb,$exit_code" >> "$output_csv"
    
    [[ $exit_code -eq 0 ]] && \
        echo "  ✓ ${elapsed}s, ${max_ram_mb}MB" || \
        echo "  ✗ Exit code: $exit_code"
done

echo ""
echo "✓ Specifications: $specs_file"
echo "✓ Results: $output_csv"
echo ""
cat "$output_csv"

