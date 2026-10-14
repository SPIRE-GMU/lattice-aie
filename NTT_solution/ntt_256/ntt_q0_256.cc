#include "ntt_q0_256.h"

#include <cstdint>

// q0=167772161, N=256, omega0=26238939

// Input is already: signed -> mod q0 -> negacyclic twist -> bit reversal.

// Architecture: 8 concurrent radix-2 stages connected by streams.

static ap_uint<32> add_mod_q0(ap_uint<32> a, ap_uint<32> b) {

#pragma HLS INLINE

    ap_uint<33> s=(ap_uint<33>)a+(ap_uint<33>)b;

    if(s>=Q0) s-=Q0;

    return (ap_uint<32>)s;

}

static ap_uint<32> sub_mod_q0(ap_uint<32> a, ap_uint<32> b) {

#pragma HLS INLINE

    return (a>=b)?(ap_uint<32>)(a-b):(ap_uint<32>)((ap_uint<33>)a+Q0-b);

}

static ap_uint<32> mul_mod_q0_shoup(ap_uint<32> a, ap_uint<32> w, ap_uint<32> w_shoup) {

#pragma HLS INLINE

    ap_uint<64> p0=a*w;

#pragma HLS BIND_OP variable=p0 op=mul impl=dsp

    ap_uint<64> p1=a*w_shoup;

#pragma HLS BIND_OP variable=p1 op=mul impl=dsp

    ap_uint<32> qhat=p1.range(63,32);

    ap_uint<64> qprod=qhat*Q0;

#pragma HLS BIND_OP variable=qprod op=mul impl=dsp

    ap_uint<64> r=p0-qprod;

    if(r>=Q0) r-=Q0;

    if(r>=Q0) r-=Q0;

    return (ap_uint<32>)r;

}

// Stage 0: LEN=2, HALF=1, WLEN=167772160

static const ap_uint<32> W_STAGE0[1] = {

    1u

};

static const ap_uint<32> WS_STAGE0[1] = {

    25u

};

// Stage 1: LEN=4, HALF=2, WLEN=65249968

static const ap_uint<32> W_STAGE1[2] = {

    1u, 65249968u

};

static const ap_uint<32> WS_STAGE1[2] = {

    25u, 1670399170u

};

// Stage 2: LEN=8, HALF=4, WLEN=71493608

static const ap_uint<32> W_STAGE2[4] = {

    1u, 71493608u, 65249968u, 30406922u

};

static const ap_uint<32> WS_STAGE2[4] = {

    25u, 1830236353u, 1670399170u, 778417198u

};

// Stage 3: LEN=16, HALF=8, WLEN=116878283

static const ap_uint<32> W_STAGE3[8] = {

    1u, 116878283u, 71493608u, 131850885u, 65249968u, 26998229u, 30406922u, 40136459u

};

static const ap_uint<32> WS_STAGE3[8] = {

    25u, 2992084026u, 1830236353u, 3375382635u, 1670399170u, 691154658u, 778417198u, 1027493344u

};

// Stage 4: LEN=32, HALF=16, WLEN=17334830

static const ap_uint<32> W_STAGE4[16] = {

    1u, 17334830u, 116878283u, 64106361u, 71493608u, 110675021u, 131850885u, 14835157u,

    65249968u, 10337336u, 26998229u, 39437715u, 30406922u, 133555027u, 40136459u, 159924886u

};

static const ap_uint<32> WS_STAGE4[16] = {

    25u, 443771645u, 2992084026u, 1641122831u, 1830236353u, 2833280520u, 3375382635u, 379780016u,

    1670399170u, 264635800u, 691154658u, 1009605497u, 778417198u, 3419008670u, 1027493344u, 4094077057u

};

// Stage 5: LEN=64, HALF=32, WLEN=54112143

static const ap_uint<32> W_STAGE5[32] = {

    1u, 54112143u, 17334830u, 78043547u, 116878283u, 30983387u, 64106361u, 43105334u,

    71493608u, 154093581u, 110675021u, 59415281u, 131850885u, 163613908u, 14835157u, 29033016u,

    65249968u, 139268124u, 10337336u, 34310991u, 26998229u, 68357634u, 39437715u, 70107821u,

    30406922u, 17714435u, 133555027u, 145321691u, 40136459u, 128975965u, 159924886u, 150252285u

};

static const ap_uint<32> WS_STAGE5[32] = {

    25u, 1385270852u, 443771645u, 1997914791u, 2992084026u, 793174702u, 1641122831u, 1103496543u,

    1830236353u, 3944795650u, 2833280520u, 1521031184u, 3375382635u, 4188516019u, 379780016u, 743245205u,

    1670399170u, 3565263953u, 264635800u, 878361364u, 691154658u, 1749955419u, 1009605497u, 1794760206u,

    778417198u, 453489533u, 3419008670u, 3720235267u, 1027493344u, 3301784684u, 4094077057u, 3846458473u

};

// Stage 6: LEN=128, HALF=64, WLEN=370529

static const ap_uint<32> W_STAGE6[64] = {

    1u, 370529u, 54112143u, 2816859u, 17334830u, 67813346u, 78043547u, 19984242u,

    116878283u, 100947099u, 30983387u, 97740976u, 64106361u, 83280589u, 43105334u, 34346647u,

    71493608u, 69717537u, 154093581u, 86414990u, 110675021u, 91087201u, 59415281u, 21687229u,

    131850885u, 162145770u, 163613908u, 66200787u, 14835157u, 136577210u, 29033016u, 23422144u,

    65249968u, 30360006u, 139268124u, 20753699u, 10337336u, 44335114u, 34310991u, 113912303u,

    26998229u, 43921355u, 68357634u, 90394377u, 39437715u, 29650296u, 70107821u, 146031035u,

    30406922u, 74701944u, 17714435u, 129403473u, 133555027u, 101762884u, 145321691u, 96860233u,

    40136459u, 62121449u, 128975965u, 106363279u, 159924886u, 16363816u, 150252285u, 153863330u

};

static const ap_uint<32> WS_STAGE6[64] = {

    25u, 9485542u, 1385270852u, 72111589u, 443771645u, 1736021647u, 1997914791u, 511596592u,

    2992084026u, 2584245718u, 793174702u, 2502168970u, 1641122831u, 2131983065u, 1103496543u, 879274157u,

    1830236353u, 1784768936u, 3944795650u, 2212223730u, 2833280520u, 2331832331u, 1521031184u, 555193059u,

    3375382635u, 4150931687u, 4188516019u, 1694740137u, 379780016u, 3496376555u, 743245205u, 599606882u,

    1670399170u, 777216148u, 3565263953u, 531294691u, 264635800u, 1134978911u, 878361364u, 2916154939u,

    691154658u, 1124386681u, 1749955419u, 2314096037u, 1009605497u, 759047573u, 1794760206u, 3738394473u,

    778417198u, 1912369755u, 453489533u, 3312728889u, 3419008670u, 2605129814u, 3720235267u, 2479621950u,

    1027493344u, 1590309084u, 3301784684u, 2722899926u, 4094077057u, 418913687u, 3846458473u, 3938901224u

};

// Stage 7: LEN=256, HALF=128, WLEN=26238939

static const ap_uint<32> W_STAGE7[128] = {

    1u, 26238939u, 370529u, 58870942u, 54112143u, 158211581u, 2816859u, 37032695u,

    17334830u, 105713948u, 67813346u, 149237661u, 78043547u, 15887874u, 19984242u, 128480178u,

    116878283u, 113418251u, 100947099u, 5832372u, 30983387u, 157531108u, 97740976u, 63610461u,

    64106361u, 48465784u, 83280589u, 149682779u, 43105334u, 24981033u, 34346647u, 39281926u,

    71493608u, 18931299u, 69717537u, 41235761u, 154093581u, 34585299u, 86414990u, 83051669u,

    110675021u, 114320120u, 91087201u, 140078522u, 59415281u, 152318212u, 21687229u, 95358070u,

    131850885u, 113212430u, 162145770u, 79516318u, 163613908u, 129282529u, 66200787u, 115462638u,

    14835157u, 19196180u, 136577210u, 40613625u, 29033016u, 34104569u, 23422144u, 132680481u,

    65249968u, 25150941u, 30360006u, 80562883u, 139268124u, 22729182u, 20753699u, 161911561u,

    10337336u, 120594584u, 44335114u, 24342840u, 34310991u, 129014839u, 113912303u, 81901779u,

    26998229u, 20245089u, 43921355u, 131491610u, 68357634u, 83662968u, 90394377u, 125909941u,

    39437715u, 40858714u, 29650296u, 81947549u, 70107821u, 34369158u, 146031035u, 23863877u,

    30406922u, 162279750u, 74701944u, 146529672u, 17714435u, 72726634u, 129403473u, 98013888u,

    133555027u, 19303726u, 101762884u, 127523302u, 145321691u, 65687040u, 96860233u, 78075729u,

    40136459u, 32525089u, 62121449u, 78833129u, 128975965u, 156136497u, 106363279u, 58047122u,

    159924886u, 86571660u, 16363816u, 112285745u, 150252285u, 145463520u, 153863330u, 135931381u

};

static const ap_uint<32> WS_STAGE7[128] = {

    25u, 671716834u, 9485542u, 1507096106u, 1385270852u, 4050216449u, 72111589u, 948036986u,

    443771645u, 2706277052u, 1736021647u, 3820484098u, 1997914791u, 406729571u, 511596592u, 3289092537u,

    2992084026u, 2903507208u, 2584245718u, 149308722u, 793174702u, 4032796340u, 2502168970u, 1628427791u,

    1641122831u, 1240724063u, 2131983065u, 3831879119u, 1103496543u, 639514440u, 879274157u, 1005617299u,

    1830236353u, 484641251u, 1784768936u, 1055635475u, 3944795650u, 885383649u, 2212223730u, 2126122713u,

    2833280520u, 2926595054u, 2331832331u, 3586010141u, 1521031184u, 3899346203u, 555193059u, 2441166577u,

    3375382635u, 2898238190u, 4150931687u, 2035617728u, 4188516019u, 3309632722u, 1694740137u, 2955843515u,

    379780016u, 491422205u, 3496376555u, 1039708793u, 743245205u, 873076961u, 599606882u, 3396620293u,

    1670399170u, 643864085u, 777216148u, 2062409792u, 3565263953u, 581867055u, 531294691u, 4144935936u,

    264635800u, 3087221331u, 1134978911u, 623176700u, 878361364u, 3302779858u, 2916154939u, 2096685529u,

    691154658u, 518274275u, 1124386681u, 3366185195u, 1749955419u, 2141771968u, 2314096037u, 3223294470u,

    1009605497u, 1045983072u, 759047573u, 2097857241u, 1794760206u, 879850439u, 3738394473u, 610915247u,

    778417198u, 4154361575u, 1912369755u, 3751159580u, 453489533u, 1861801819u, 3312728889u, 2509155517u,

    3419008670u, 494175382u, 2605129814u, 3264596511u, 3720235267u, 1681588213u, 2479621950u, 1998738650u,

    1027493344u, 832642273u, 1590309084u, 2018128090u, 3301784684u, 3997094299u, 2722899926u, 1486006314u,

    4094077057u, 2216234482u, 418913687u, 2874515054u, 3846458473u, 3723866089u, 3938901224u, 3479843332u

};


// Each radix-2 stage processes one input coefficient per cycle.
// First HALF inputs of a group are delayed. During the second HALF,
// butterflies produce the SUM half while the DIFF half is buffered.
// During the first HALF of the following group, buffered DIFF values
// are emitted as the next group's first operands are accepted.
// Thus no make_pairs/unpack_pairs passes or group-level pipeline drain.
// Data order is unchanged from the original DIT NTT.

template<int LEN>
static void ntt_stage_stream(
    hls::stream<ap_int<32>>& in,
    hls::stream<ap_int<32>>& out,
    const ap_uint<32> W[LEN/2],
    const ap_uint<32> WS[LEN/2])
{
#pragma HLS INLINE off
    const int HALF = LEN / 2;
    const int TOTAL = NTT_N + HALF;

    // Separate registers per butterfly lane. Avoid inferred single-port RAM
    // and ambiguous read/write scheduling across pipelined iterations.
    ap_uint<32> a_buf[LEN/2];
    ap_uint<32> diff_buf[LEN/2];
#pragma HLS ARRAY_PARTITION variable=a_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=diff_buf complete dim=1

STAGE_STREAM:
    for (int t = 0; t < TOTAL; ++t) {
#pragma HLS PIPELINE II=1
        const int phase = t & (LEN - 1);
        const int j = phase & (HALF - 1);

        if (phase < HALF) {
            // Emit the differences from the previous group while reading
            // the first operands of the current group.
            if (t < NTT_N) {
                a_buf[j] = (ap_uint<32>)in.read();
            }
            if (t >= LEN) {
                out.write((ap_int<32>)diff_buf[j]);
            }
        } else {
            ap_uint<32> b = (ap_uint<32>)in.read();
            ap_uint<32> a = a_buf[j];
            ap_uint<32> v;
            if (j == 0) {
                v = b;
            } else {
                v = mul_mod_q0_shoup(b, W[j], WS[j]);
            }
            out.write((ap_int<32>)add_mod_q0(a, v));
            diff_buf[j] = sub_mod_q0(a, v);
        }
    }
}

void ntt_q0_8stage(
    hls::stream<ap_int<32>>& in,
    hls::stream<ap_int<32>>& out)
{
#pragma HLS INTERFACE axis port=in
#pragma HLS INTERFACE axis port=out
#pragma HLS INTERFACE ap_ctrl_hs port=return
#pragma HLS DATAFLOW

    // Precisely eight butterfly engines, seven direct FIFO edges.
    hls::stream<ap_int<32>> s01("s01");
    hls::stream<ap_int<32>> s12("s12");
    hls::stream<ap_int<32>> s23("s23");
    hls::stream<ap_int<32>> s34("s34");
    hls::stream<ap_int<32>> s45("s45");
    hls::stream<ap_int<32>> s56("s56");
    hls::stream<ap_int<32>> s67("s67");

    // Larger FIFOs isolate stages during functional RTL verification.
#pragma HLS STREAM variable=s01 depth=256
#pragma HLS STREAM variable=s12 depth=256
#pragma HLS STREAM variable=s23 depth=256
#pragma HLS STREAM variable=s34 depth=256
#pragma HLS STREAM variable=s45 depth=256
#pragma HLS STREAM variable=s56 depth=256
#pragma HLS STREAM variable=s67 depth=256

    ntt_stage_stream<2>  (in,  s01, W_STAGE0, WS_STAGE0);
    ntt_stage_stream<4>  (s01, s12, W_STAGE1, WS_STAGE1);
    ntt_stage_stream<8>  (s12, s23, W_STAGE2, WS_STAGE2);
    ntt_stage_stream<16> (s23, s34, W_STAGE3, WS_STAGE3);
    ntt_stage_stream<32> (s34, s45, W_STAGE4, WS_STAGE4);
    ntt_stage_stream<64> (s45, s56, W_STAGE5, WS_STAGE5);
    ntt_stage_stream<128>(s56, s67, W_STAGE6, WS_STAGE6);
    ntt_stage_stream<256>(s67, out, W_STAGE7, WS_STAGE7);
}
