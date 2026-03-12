#!/bin/bash
# Run Local Beam Search for all combinations of n and k
# Each combination is executed 5 times with different random seeds
# Results (excluding board) are saved to results.txt

N_VALUES=(8 12 16 20)
K_VALUES=(1 3 5 7)
RUNS=30
OUTPUT_FILE="results.txt"

> "$OUTPUT_FILE"

for n in "${N_VALUES[@]}"; do
    for kval in "${K_VALUES[@]}"; do
        echo "============================================"
        echo "  N = $n, k = $kval"
        echo "============================================"

        echo "============================================" >> "$OUTPUT_FILE"
        echo "  N = $n, k = $kval" >> "$OUTPUT_FILE"
        echo "============================================" >> "$OUTPUT_FILE"

        g++ -o main_test main.cpp -std=c++17 -DN="$n" -Dk="$kval"

        for run in $(seq 1 $RUNS); do
            echo "--- Run $run ---"
            output=$(./main_test)
            echo "$output"
            echo ""

            # Extract non-board lines for the results file
            echo "--- Run $run ---" >> "$OUTPUT_FILE"
            echo "$output" | grep -v '^ [01 ]*$' >> "$OUTPUT_FILE"
            echo "" >> "$OUTPUT_FILE"
        done
    done
done

rm -f main_test
echo "Results saved to $OUTPUT_FILE"
