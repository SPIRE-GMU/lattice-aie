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

| Parameter Set | Operation | Baseline Total (µs) | Instrumented Total (µs) | NTT (µs, %) | INTT (µs, %) | Other (µs) |
|---|---|---:|---:|---:|---:|---:|
| 44 | Key Generation | 36.61 | 36.98 | 3.24 (8.8%) | 3.75 (10.1%) | 29.99 |
| 44 | Signing | 139.03 | 140.80 | 26.88 (19.1%) | 40.30 (28.6%) | 73.63 |
| 44 | Verification | 39.65 | 40.13 | 7.30 (18.2%) | 3.73 (9.3%) | 29.10 |
| 65 | Key Generation | 70.25 | 70.62 | 4.10 (5.8%) | 5.59 (7.9%) | 60.92 |
| 65 | Signing | 223.16 | 225.63 | 39.18 (17.4%) | 69.98 (31.0%) | 116.47 |
| 65 | Verification | 62.57 | 63.34 | 9.86 (15.6%) | 5.62 (8.9%) | 47.86 |
| 87 | Key Generation | 102.69 | 103.36 | 5.83 (5.6%) | 7.57 (7.3%) | 89.96 |
| 87 | Signing | 285.57 | 293.11 | 46.22 (15.8%) | 79.41 (27.1%) | 167.47 |
| 87 | Verification | 104.60 | 105.87 | 13.41 (12.7%) | 7.57 (7.2%) | 84.89 |

### Average NTT / INTT Call Counts

Each transform processes one polynomial with **256 coefficients**.

| Parameter Set | Key Generation NTT / INTT | Signing NTT / INTT | Verification NTT / INTT |
|---|---:|---:|---:|
| 44 | 4 / 4 | 33.16 / 43.09 | 9 / 4 |
| 65 | 5 / 6 | 47.83 / 75.21 | 12 / 6 |
| 87 | 7 / 8 | 55.46 / 84.04 | 16 / 8 |

The signing call counts are fractional because ML-DSA signing uses rejection sampling, so the number of NTT/INTT invocations varies across signatures.



These measurements show that NTT and INTT account for a significant fraction of the signing latency. For example, in ML-DSA-65 signing, NTT and INTT together account for approximately **48.4%** of the instrumented runtime.

### Signing Latency Distribution

Signing exhibits a noticeable long-tail latency due to rejection sampling.

| Parameter Set | P95 Signing Latency (µs) |
|---|---:|
| ML-DSA-44 | 312.56 |
| ML-DSA-65 | 499.63 |
| ML-DSA-87 | 599.37 |

This variability is expected because the number of signing iterations, and therefore the number of NTT/INTT calls, depends on the rejection-sampling process.   

## TFHE Application

TFHE reference：https://docs.zama.org/tfhe-rs/get-started/benchmarks/hpu/hpu-integer-operations
