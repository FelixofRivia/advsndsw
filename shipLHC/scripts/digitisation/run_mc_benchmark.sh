#!/bin/bash

##############################################################################
# Script: profile_snd.sh
# Purpose: Run sndLHC digitization n times, measure runtime and peak RAM usage
##############################################################################

if [[ $# -lt 2 ]]; then
    echo "Usage: $0 <n> <output_csv>"
    exit 1
fi

n=$1
output_csv=$2

# The command to run (with environment variable)
command=(python "$ADVSNDSW_ROOT/shipLHC/run_digiSND.py" \
    -f /eos/experiment/sndlhc/Run4/testbeam2026/digi_benchmark/mc_benchmark/electron_100/sndLHC.PG_11-TGeant4.root \
    -g /eos/experiment/sndlhc/Run4/testbeam2026/digi_benchmark/mc_benchmark/electron_100/geofile_full.PG_11-TGeant4.root \
    -n 10000)

# Initialize CSV with headers
echo "iteration,runtime_seconds,max_ram_mb,exit_code" > "$output_csv"

echo "Running command $n times..."
echo "Command: ${command[@]}"
echo ""

# Run command n times
for ((i = 1; i <= n; i++)); do
    echo "[$(date '+%H:%M:%S')] Iteration $i/$n..."
    
    # Create temp file for time output
    temp_time_output=$(mktemp)
    
    # Run command and capture timing/memory info
    /usr/bin/time -v -o "$temp_time_output" "${command[@]}" > /dev/null 2>&1
    exit_code=$?
    
    # Extract metrics
    elapsed=$(grep "Elapsed (wall clock) time" "$temp_time_output" | awk -F'[m:]' '{print ($1 * 60) + $2}')
    max_ram=$(grep "Maximum resident set size" "$temp_time_output" | awk '{print $6}')
    
    # Convert max_ram from KB to MB
    max_ram_mb=$(echo "scale=2; $max_ram / 1024" | bc)
    
    # Append to CSV
    echo "$i,$elapsed,$max_ram_mb,$exit_code" >> "$output_csv"
    
    if [[ $exit_code -eq 0 ]]; then
        echo "  ✓ Completed: ${elapsed}s runtime, ${max_ram_mb}MB peak RAM"
    else
        echo "  ✗ Exit code: $exit_code"
    fi
    
    rm "$temp_time_output"
done

echo ""
echo "✓ Results saved to: $output_csv"
echo ""
echo "Summary:"
cat "$output_csv"
