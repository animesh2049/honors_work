#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "unsharp_mask.h"

using namespace cv;

int main(int argc, char** argv)
{
	data_tc1 threshold = 0.01f;
	data_tc weight = 0.2;
	Mat img, output;
	img = imread("/home/vinamra/Desktop/Power/HD.jpg", CV_LOAD_IMAGE_COLOR);
	Rect roi(0, 0, C+4, R+4);
	Mat img_region = img(roi).clone();

	Mat out_region(R,C,CV_MAKETYPE(img_region.depth(),img_region.channels()));

	hls::stream<int> src_img_strm("src_img_strm");
	hls::stream<data_t3> dst_img_strm("dst_img_strm");

	for (int c = 0; c < 3; c++)
		for (int i = 0; i < R+4; i++)
			for (int j = 0; j < C+4; j++){
				int val = img_region.at<Vec3b>(i, j)[c];
				src_img_strm << val;
			}

	pipeline_mask(threshold,weight,src_img_strm,dst_img_strm);

	for (int c = 0; c < 3; c++){
		for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					data_t3 dst_val = dst_img_strm.read();
					dst_val = (dst_val > 255)? (data_t3)255:dst_val;
					out_region.at<Vec3b>(i,j)[c] = dst_val;
				}
		}
	}

	while(!dst_img_strm.empty())dst_img_strm.read();
	
	imwrite("/home/vinamra/Desktop/USM-resolve/fresh/test.jpg",out_region);

return 0;
}