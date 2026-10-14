#ifndef NTT_Q0_256_H
#define NTT_Q0_256_H

#include <ap_int.h>
#include <hls_stream.h>

static constexpr int NTT_N = 256;
static const ap_uint<32> Q0 = 167772161u;

void ntt_q0_8stage(hls::stream<ap_int<32>>& in, hls::stream<ap_int<32>>& out);

#endif

