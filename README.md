# ASCENT

This repository implement ASCENT framework to accelerate NTT in different degrees and modulus on spatial architecture.

## Setup

CPU: i9-14900K / i3-4160K

OS: Ubuntu 2024.04 TLS

Tools: Vitis 2025.1

Hardware: XCVC1902-VSVA2179-2MP-ES

## C-profile

  We provide C file as a benchmark running NTT with different degrees on CPUs. To execute, open terminal and type

    ./ntt_profile 10000

  This testbench implements 256-, 512-, and 1024-degree NTT as they satisfy most of the PQC applications built on LWE problems. If you want to try more test case, feel free to change the twiddle factors, degrees, and  modular. 

## AIE/HLS Examples

  We also provide 512- and 1024-degree implementations of NTT on both AI engines and FPGA, for reproducible research.  

## MLDSA Application

MLDSA application profiling is referred to https://github.com/pq-code-package/mldsa-native/tree/main
The following measurements were collected using the current project's **pure C backend**, including the SHAKE implementation.



All timing results are reported in **microseconds per operation (µs/op)**.

The baseline total latency is measured using the uninstrumented implementation.  
The NTT/INTT breakdown and percentages are obtained from a separately instrumented build so that profiling overhead is not included in the baseline measurements.
### Runtime Breakdown

| Parameter Set | Operation | Total (µs) | NTT+INTT (µs, %) | Other (µs) |
|---|---|---:|---:|---:|
| 44 | Key Generation | 72.98 | 12.99 (17.8%) | 59.99 |
| 44 | Signing | 280.80 | 134.18 (47.8%) | 146.62 |
| 44 | Verification | 80.13 | 22.03 (27.5%) | 58.10 |
| 65 | Key Generation | 140.62 | 18.69 (13.3%) | 121.93 |
| 65 | Signing | 450.63 | 218.16 (48.4%) | 232.47 |
| 65 | Verification | 126.34 | 30.48 (24.1%) | 95.86 |
| 87 | Key Generation | 206.36 | 26.40 (12.8%) | 179.96 |
| 87 | Signing | 585.11 | 250.63 (42.8%) | 334.48 |
| 87 | Verification | 210.87 | 40.98 (19.4%) | 169.89 |

### Average NTT / INTT Call Counts

Each transform processes one polynomial with **256 coefficients**.

| Parameter Set | Key Generation NTT / INTT | Signing NTT / INTT | Verification NTT / INTT |
|---|---:|---:|---:|
| 44 | 4 / 4 | 33.16 / 43.09 | 9 / 4 |
| 65 | 5 / 6 | 47.83 / 75.21 | 12 / 6 |
| 87 | 7 / 8 | 55.46 / 84.04 | 16 / 8 |

The signing call counts are fractional because ML-DSA signing uses rejection sampling, so the number of NTT/INTT invocations varies across signatures.



These measurements show that NTT and INTT account for a significant fraction of the signing latency. For example, in ML-DSA-65 signing, NTT and INTT together account for approximately **48.4%** of the instrumented runtime.

 

## TFHE Application

TFHE reference：https://docs.zama.org/tfhe-rs/get-started/benchmarks/hpu/hpu-integer-operations
