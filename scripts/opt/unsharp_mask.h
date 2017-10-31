#ifndef UNSHARP_H_
#define UNSHARP_H_

#include <fstream>
#include <hls_stream.h>
#include <cmath>
#include <ap_fixed.h>
#include <ap_int.h>

#define R 1916//240//479// by 4
#define C 1000//68//269// next will be by 2 // next * 4

typedef ap_ufixed<14,8,AP_RND,AP_SAT> data_t1;
typedef ap_ufixed<14,8,AP_RND,AP_SAT> data_t2;
typedef ap_fixed<16,10,AP_RND,AP_SAT> data_t3;
typedef ap_fixed<30,9,AP_RND,AP_SAT> data_t4;

typedef ap_ufixed<16,0,AP_RND,AP_SAT> data_tc;  // this seems the quality deciding factor
typedef ap_ufixed<8,0,AP_RND,AP_SAT> data_tc1;
typedef ap_ufixed<4,0,AP_RND,AP_SAT> data_c1;
typedef ap_uint<8> int8;
typedef ap_uint<12> int12;
typedef ap_int<16> int_t;
typedef ap_int<16> int16;

void pipeline_mask(data_tc1 thresh, data_tc  weight, hls::stream<int> &input, hls::stream<data_t3> &mask);

#endif
