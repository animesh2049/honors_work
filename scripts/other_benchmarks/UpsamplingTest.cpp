#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "Upsampling.h"

using namespace cv;

int main(int argc, char** argv)
{
	Mat img, imgf, output;
	img = imread("/home/vinamra/Desktop/Power/HD.jpg", CV_LOAD_IMAGE_COLOR);	
	Rect roi(0, 0, C+4, R+4);
	Mat img_region = img(roi).clone();
	int numChannels = img_region.channels();
	Mat* out_region= new Mat(R,C,CV_MAKETYPE(img_region.depth(),numChannels));

	hls::stream<uint8> src_img_strm("src_img_strm");
	hls::stream<uint8> dst_img_strm("dst_img_strm");

	for(int c=0;c<numChannels;c++){
		for (int i = 0; i < R+4; i++)
			for (int j = 0; j < C+4; j++){
				uint8 val = img_region.at<Vec3b>(i, j)[c];
				src_img_strm << val;
			}
	}

	upsample(src_img_strm,dst_img_strm);

	for(int c=0;c<numChannels;c++){
		for (int i = 0; i < (R-4); i++) {
			for (int j = 0; j < (C-4); j++) {
				uint8 dst_val = dst_img_strm.read();
				out_region->at<Vec3b>(i,j)[c] = dst_val;
			}
		}
	}

	while(!dst_img_strm.empty()) {
		dst_img_strm.read();
	}

//imwrite("/home/vinamra/Desktop/Reports/DUS_NEW/error_theoritical/images/DUS000100_20.jpg", *out_region);
	return 0;
}
