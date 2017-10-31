#ifndef HARRIS_H_
#define HARRIS_H_
#include <hls_stream.h>
#include <cmath>
#include <ap_fixed.h>
#include <ap_int.h>

#define R 1916
#define C 1076


typedef ap_ufixed<18,8,AP_RND,AP_SAT>   data_t1;
typedef ap_ufixed<18,8,AP_RND,AP_SAT>   data_t2;
typedef   ap_ufixed<18,8,AP_RND,AP_SAT> data_t3;
typedef   ap_ufixed<18,8,AP_RND,AP_SAT>   data_t4;

typedef unsigned char uint8;
typedef ap_ufixed<4,0,AP_RND,AP_SAT> data_tc;

typedef ap_int<16> int_t;


void upsample(hls::stream<uint8> &img_colour, hls::stream<uint8> &U_upsampled_L);
#endif
