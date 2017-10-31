#ifndef UNSHARP_H_
#define UNSHARP_H_

#include <fstream>
#include <hls_stream.h>
#include <cmath>
#include <ap_fixed.h>
#include <ap_int.h>

#define R 1916
#define C 1076

typedef float data_t1;
typedef float data_t2;
typedef float data_t3;
typedef float data_t4;

typedef float data_tc;  // this seems the quality deciding factor
typedef float data_tc1;
typedef float data_c1;
typedef float int8;
typedef float int12;
typedef int int_t;
typedef int int16;

void pipeline_mask(data_tc1 thresh, data_tc  weight, hls::stream<int8> &input, hls::stream<data_t3> &mask);

#endif
