#ifndef HARRIS_H_
#define HARRIS_H_

#include <fstream>
#include <hls_stream.h>
#include <cmath>
#include <ap_fixed.h>
#include <ap_int.h>

#define R 1916
#define C 1076

typedef   ap_fixed<18,8,AP_RND,AP_SAT> data_t1;
typedef   ap_fixed<24,14,AP_RND,AP_SAT> data_t2;
typedef   ap_fixed<27,17,AP_RND,AP_SAT> data_t3;
typedef   ap_fixed<44,34,AP_RND,AP_SAT> data_t4;
typedef   ap_fixed<28,18,AP_RND,AP_SAT> data_t5; 
typedef   ap_fixed<41,31,AP_RND,AP_SAT> data_t6;


typedef ap_ufixed<16,0,AP_RND,AP_SAT> data_tc;
typedef ap_int<16> int_4;
typedef ap_int<16> int_12;
typedef ap_int<16> int_16;
typedef ap_int<16> int_24;
typedef unsigned char int8;

void harris_corner(hls::stream<int8> &input,hls::stream<int8> &output);
#endif